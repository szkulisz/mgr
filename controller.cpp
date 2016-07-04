#include "controller.h"
#include <iostream>

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
        mProfiler.startLogging(mPeriod, 1000, false, "logs/test.txt");
        clock_gettime(CLOCK_MONOTONIC, &next);
        mProfiler.startProfiling();
        while (mRunPendulum) {
            timespecAddUs(&next, mPeriod);
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
            mProfiler.updatePeriodProfiling();
    //        emit timeout();
            mPendulum.readEncoderValues();
            mPendulum.getPositions(mCartPosition, mPendulumAngle);
            switch (mPhase) {
            case Phase::SWING_UP:
                swingUp();
                break;
            default:
                mPendulum.control(0);
                break;
            }

            mProfiler.updateHandlerTimeProfiling();
        }
        usleep(1000);
    }
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

void Controller::startController()
{
    mNextPeriod = mPeriod;
    mPhase = Phase::SWING_UP;
    mPeriod = mSwingPeriod;
    mRunPendulum = true;
}

void Controller::stopController()
{
    mRunPendulum = false;
}

void Controller::finish()
{
    mRunController = false;
}

void Controller::quit()
{
    stopController();
    finish();
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
            setPeriod(mNextPeriod);
        }
    } else {
        if ((mPendulumAngle > (2*M_PI - M_PI/8)) || (mPendulumAngle < (0 + 0.02))){
            mPhase=Phase::CONTROL;
            setPeriod(mNextPeriod);
        }
    }
}

void Controller::setPeriod(int period)
{
    if ((mPhase == Phase::SWING_UP) || (mPhase == Phase::SWING_DOWN)) {
        mNextPeriod = period;
    } else {
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
