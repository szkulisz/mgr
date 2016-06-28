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
    std::cout << "PendulumController ma ID: " << QThread::currentThreadId() << " i priorytet: " << param.sched_priority << std::endl;

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
            mPendulum.getPositions();

            mProfiler.updateHandlerTimeProfiling();
        }
        usleep(1000);
    }
    mProfiler.saveLogFile();
}


void Controller::stop()
{
    mRunPendulum = false;
}

void Controller::finish()
{
    mRunController = false;
}

void Controller::quit()
{
    std::cout << "finish z: " << QThread::currentThreadId() << std::endl;
    mRunPendulum = false;
    mRunController = false;
}

void Controller::setPeriod(int period)
{
    mPeriod = period;
    mProfiler.changePeriod(period);
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
