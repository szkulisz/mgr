#include "controller.h"
#include <iostream>
#include <cmath>
#include <QCoreApplication>

void Controller::run()
{
    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = (sched_get_priority_max(SCHED_FIFO));
    sts = sched_setscheduler(0, SCHED_FIFO, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << "PendulumController has ID: " << QThread::currentThreadId() << " and priority: " << param.sched_priority << std::endl;
    while (mRunController) {
        struct timespec next;
        if (mRunPendulum && mRunPendulumInit) {
//          mProfiler.startLogging(mPeriod, 1000, false, "logs/test.txt");
            clock_gettime(CLOCK_MONOTONIC, &next);
//          mProfiler.startProfiling();
            mRunPendulumInit = false;
//            mPendulum.resetEncoders();
        }
        while (mRunPendulum) {
            timespecAddUs(&next, mPeriod);
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
//            mProfiler.updatePeriodProfiling();
            mPendulum.readEncoderValues();
//            mPositionsMutex.lock();
            mPendulum.getPositions(mCartPosition, mPendulumAngle);
//            mPositionsMutex.unlock();
            switch (mPhase) {
            case
            Phase::SWING_UP:
//                swingUp();
                break;
            case Phase::CONTROL:
                control();
                break;
            case Phase::SWING_DOWN:
            default:
                mPendulum.control(0);
                break;
            }

//            mProfiler.updateHandlerTimeProfiling();
        }
        mPendulum.control(0);
        usleep(1000);
    }
    mPendulum.control(0);
    mProfiler.saveLogFile();
}

Controller::Controller()
{
    std::map<std::string, float> params;
    params["Kp"] = 40.8f;
    params["Ki"] = 29.6f;
    params["Kd"] = 2.3f;
    params["N"] = 10;
    params["Ts"] = 0.001;
    params["constr"] = 1;
    params["CVmax"] = 2.5f;
    params["CVmin"] = -2.5f;
    mCartPID.setParameters(params);
    params["Kp"] = 119.f;
    params["Ki"] = 241.f;
    params["Kd"] = 10.f;
    mPendulumPID.setParameters(params);
}

float Controller::getCartPosition()
{
    float temp;
//    mPositionsMutex.lock();
    temp = mCartPosition;
//    mPositionsMutex.unlock();
    return temp;
}

float Controller::getPendulumAngle()
{
    float temp;
//    mPositionsMutex.lock();
    temp = mPendulumAngle;
//    mPositionsMutex.unlock();
    return temp;
}

void Controller::startControlling()
{
    mPhase = Phase::SWING_UP;
    mPeriod = mSwingPeriod;
    mRunPendulumInit = true;
    mRunPendulum = true;
}

std::map<string, float> Controller::getCartPIDParams()
{
    return mCartPID.getParameters();
}

std::map<string, float> Controller::getPendulumPIDParams()
{
    return mPendulumPID.getParameters();
}

void Controller::setCartPIDParams(std::map<string, float> params)
{
    mControlMutex.lock();
    mCartPID.setParameters(params);
    mControlMutex.unlock();
}

void Controller::setPendulumPIDParams(std::map<string, float> params)
{
    mControlMutex.lock();
    mPendulumPID.setParameters(params);
    mControlMutex.unlock();
}

void Controller::stopControlling()
{
    mRunPendulum = false;
}

void Controller::finish()
{
    mRunController = false;
}

void Controller::quit()
{
    stopControlling();
    finish();
}

float Controller::getControlValue()
{
    float temp;
    mControlMutex.lock();
    temp =  mControlValue;
    mControlMutex.unlock();
    return temp;
}

float Controller::getCartSetpoint() const
{
    return mCartSetpoint;
}

void Controller::setCartSetpoint(float cartSetpoint)
{
    mControlMutex.lock();
    mCartSetpoint = cartSetpoint;
    mControlMutex.unlock();
}

int Controller::getPeriod() const
{
    return mControlPeriod;
}

int Controller::getSamplingFrequency() const
{
    return std::round(1 / (mControlPeriod/1000000.f));
}

void Controller::setSamplingFrequency(int freq)
{
    setPeriod( std::round( (1.f/freq) * 1000000 ));
}

void Controller::swingUp()
{
    static float anglePrev;
    static float uMax = 0.5;
    float deriv;
    int sgn;
    deriv = (mPendulumAngle - anglePrev)/0.01;
    anglePrev = mPendulumAngle;
    sgn = (deriv*cos(mPendulumAngle) >= 0) ? 1 : -1;
    mPendulum.control(sgn*uMax);
    if ((mPendulumAngle < M_PI/4) || (mPendulumAngle > (2*M_PI - M_PI/4)))
        uMax = 0.25;
    if (mPendulum.getCartPosition() > 0) {
        if ((mPendulumAngle < M_PI/8) || (mPendulumAngle > (2*M_PI - 0.02))) {
            mPhase=Phase::CONTROL;
            setPeriod(mControlPeriod);
            if (mPendulumAngle < M_PI) {
                mPendulumSetpoint = 0;
            } else {
                mPendulumSetpoint = 2*M_PI;
            }
            std::cout << "SETPOINT " << mPendulumSetpoint << std::endl;
        }
    } else {
        if ((mPendulumAngle > (2*M_PI - M_PI/8)) || (mPendulumAngle < (0 + 0.02))){
            mPhase=Phase::CONTROL;
            setPeriod(mControlPeriod);
            if (mPendulumAngle < M_PI) {
                mPendulumSetpoint = 0;
            } else {
                mPendulumSetpoint = 2*M_PI;
            }
            std::cout << "SETPOINT " << mPendulumSetpoint << std::endl;
        }
    }
}

void Controller::control()
{
    static int counter;
    if (++counter > 10)
        QCoreApplication::quit();
    mControlMutex.lock();
    std::cout << "mCartPosition " << mCartPosition << std::endl;
    float cartPidCV = mCartPID.control(mCartSetpoint,mCartPosition);
    std::cout << "cartPidCV " << cartPidCV << std::endl;
    std::cout << "mPendulumAngle " << mPendulumAngle << std::endl;
    float pendulumPidCV = mPendulumPID.control(mPendulumSetpoint,mPendulumAngle);
    std::cout << "pendulumPidCV " << pendulumPidCV << std::endl;
    mControlValue = pendulumPidCV - cartPidCV;
    std::cout << "mControlValue " << mControlValue << std::endl;
    if (mControlValue > 2.5)
        mControlValue = 2.5;
    if (mControlValue < -2.5)
        mControlValue = -2.5;
    mPendulum.control(mControlValue);
    mControlMutex.unlock();
}

void Controller::setPeriod(int period)
{
    mControlMutex.lock();
    mControlPeriod = period;
//    mCartPID.setSamplingTime(period);
//    mPendulumPID.setSamplingTime(period);
    if (!(mPhase == Phase::SWING_UP) && !(mPhase == Phase::SWING_DOWN)) {
        mPeriod = period;
        mProfiler.changePeriod(period);
    }
    mControlMutex.unlock();
}


void Controller::timespecAddUs(struct timespec *t, long us)
{
    t->tv_sec += us/1000000;
    t->tv_nsec += (us%1000000)*1000;
    if (t->tv_nsec > 1000000000) {
        t->tv_nsec = t->tv_nsec - 1000000000;// + ms*1000000;
        t->tv_sec += 1;
    }
}
