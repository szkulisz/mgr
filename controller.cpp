#include "controller.h"
#include <iostream>
#include <cmath>
#include <QCoreApplication>

void Controller::run()
{
#define PROFILING_LOOPS 10000
    static int profile_counter;

    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = (sched_get_priority_max(SCHED_FIFO));
    sts = sched_setscheduler(0, SCHED_FIFO, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << "PendulumController has ID: " << QThread::currentThreadId() << " and priority: " << param.sched_priority << std::endl;
    while (mRunController) {
        mParamsMutex.lock();
        if (mChangeParams) {
            mCartPID.setParameters(mCartFutureParams);
            mPendulumPID.setParameters(mPendulumFutureParams);
            setPeriod(mFuturePeriod);
            emit paramsChanged();
            mChangeParams = false;
        }
        mParamsMutex.unlock();

        struct timespec next;
        if (mRunPendulum && mRunPendulumInit) {
            mProfiler.startLogging(1000, PROFILING_LOOPS, true, "logs/test.txt");
            mRunPendulumInit = false;
            mPendulum.resetEncoders();
            mElapsedTime = 0;
            clock_gettime(CLOCK_MONOTONIC, &next);
        }
        mPeriod = 1000;
        mPhase = Phase::CONTROL;
        while (mRunPendulum) {
            timespecAddUs(&next, mPeriod);
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);

            mPinProfile->setValue(HIGH);
            if (profile_counter++ < PROFILING_LOOPS)
                mProfiler.updatePeriodProfiling();

            mPositionsMutex.lock();
            mPendulum.readEncoderValues();
            mElapsedTime += mPeriod;
            mPendulum.getPositions(mCartPosition, mPendulumAngle);
            mPositionsMutex.unlock();

            switch (mPhase) {
            case
            Phase::SWING_UP:
                swingUp();
                break;
            case Phase::CONTROL:
                control();
                break;
            case Phase::SWING_DOWN:
            default:
                mPendulum.control(0);
                break;
            }

            mParamsMutex.lock();
            if (mChangeParams) {
                mCartPID.setParameters(mCartFutureParams);
                mPendulumPID.setParameters(mPendulumFutureParams);
                setPeriod(mFuturePeriod);
                emit paramsChanged();
                mChangeParams = false;
            }
            mParamsMutex.unlock();

            if (profile_counter < PROFILING_LOOPS) {
                mProfiler.updateHandlerTimeProfiling();
            } else {
                QCoreApplication::quit();
            }
            mPinProfile->setValue(LOW);
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
    params["Kp"] = 40.8113f;
    params["Ki"] = 29.6349f;
    params["Kd"] = 2.3004f;
    params["N"] = 10;
    params["Ts"] = 0.001;
    params["constr"] = 1;
    params["CVmax"] = 2.5f;
    params["CVmin"] = -2.5f;
    mCartPID.setParameters(params);
    params["Kp"] = 118.5897f;
    params["Ki"] = 241.0771f;
    params["Kd"] = 10.f;
    mPendulumPID.setParameters(params);

    mPinProfile = new GPIO(36);
    mPinProfile->setDirection(OUTPUT);
}

float Controller::getCartPosition()
{
    static float temp;
    mPositionsMutex.lock();
    temp = mCartPosition;
    mPositionsMutex.unlock();
    return temp;
}

float Controller::getPendulumAngle()
{
    static float temp;
    mPositionsMutex.lock();
    temp = mPendulumAngle;
    mPositionsMutex.unlock();
    return temp;
}

void Controller::startControlling()
{
    mPhase = Phase::SWING_UP;
    mSwingCVMax = 0.5;
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
    mParamsMutex.lock();
    mCartFutureParams = params;
    mChangeParams = true;
    mParamsMutex.unlock();
}

void Controller::setPendulumPIDParams(std::map<string, float> params)
{
    mParamsMutex.lock();
    mPendulumFutureParams = params;
    mChangeParams = true;
    mParamsMutex.unlock();
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

float Controller::getElapsedTime()
{
    float temp;
    mPositionsMutex.lock();
    temp =  mElapsedTime/1000000;
    mPositionsMutex.unlock();
    return temp;
}

float Controller::getControlValue()
{
    float temp;
    mParamsMutex.lock();
    temp =  mControlValue;
    mParamsMutex.unlock();
    return temp;
}

float Controller::getCartSetpoint() const
{
    return mCartSetpoint;
}

void Controller::setCartSetpoint(float cartSetpoint)
{
    mParamsMutex.lock();
    mCartSetpoint = cartSetpoint;
    mParamsMutex.unlock();
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
    mFuturePeriod = std::round( (1.f/freq) * 1000000 );
    mChangeParams = true;
}

void Controller::swingUp()
{
    static float anglePrev;
    static float deriv;
    static int sgn;
    deriv = (mPendulumAngle - anglePrev)/0.01;
    anglePrev = mPendulumAngle;
    sgn = (deriv*cos(mPendulumAngle) >= 0) ? 1 : -1;
    mParamsMutex.lock();
    mControlValue = sgn*mSwingCVMax;
    mParamsMutex.unlock();
    mPendulum.control(sgn*mSwingCVMax);
    if ((mPendulumAngle < M_PI/4) || (mPendulumAngle > (2*M_PI - M_PI/4)))
        mSwingCVMax = 0.25;
    if (mPendulum.getCartPosition() > 0) {
        if ((mPendulumAngle < M_PI/8) || (mPendulumAngle > (2*M_PI - 0.02))) {
            mPhase=Phase::CONTROL;
            setPeriod(mControlPeriod);
            mProfiler.startProfiling();
            if (mPendulumAngle < M_PI) {
                mPendulumSetpoint = 0;
            } else {
                mPendulumSetpoint = 2*M_PI;
            }
        }
    } else {
        if ((mPendulumAngle > (2*M_PI - M_PI/8)) || (mPendulumAngle < (0 + 0.02))){
            mPhase=Phase::CONTROL;
            setPeriod(mControlPeriod);
            mProfiler.startProfiling();
            if (mPendulumAngle < M_PI) {
                mPendulumSetpoint = 0;
            } else {
                mPendulumSetpoint = 2*M_PI;
            }
        }
    }
}

void Controller::control()
{
    mParamsMutex.lock();
    static float cartPidCV = mCartPID.control(mCartSetpoint,mCartPosition);
    static float pendulumPidCV = mPendulumPID.control(mPendulumSetpoint,mPendulumAngle);
    mControlValue = pendulumPidCV - cartPidCV;
    if (mControlValue > 2.5)
        mControlValue = 2.5;
    if (mControlValue < -2.5)
        mControlValue = -2.5;
    mPendulum.control(mControlValue);
    mParamsMutex.unlock();
}

void Controller::setPeriod(int period)
{
    mControlPeriod = period;
//    mCartPID.setSamplingTime(period);
//    mPendulumPID.setSamplingTime(period);
    if (!(mPhase == Phase::SWING_UP) && !(mPhase == Phase::SWING_DOWN)) {
        mPeriod = period;
        mProfiler.changePeriod(period);
    }
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
