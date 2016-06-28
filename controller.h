#ifndef PENDULUMCONTROLLER_H
#define PENDULUMCONTROLLER_H

#include <QObject>
#include <QThread>
#include <pthread.h>
#include "profiler.h"
#include "pendulum.h"
#include "pid.h"

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

enum Phase{SWING_UP,CONTROL,SWING_DOWN,NOTHING};

class Controller : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;
public:
    void setPeriod(int period);
    float getCartPosition() { return mCartPosition; }
    float getPendulumAngle() { return mPendulumAngle; }
    void startController();


public slots:
    void stopController();
    void finish();
    void quit();

signals:

private:
    bool mRunController = true;
    bool mRunPendulum = true;
    int mSwingPeriod = 10000;
    int mNextPeriod;
    int mPeriod;
    int mPhase = Phase::NOTHING;
    float mPendulumAngle = 0;
    float mCartPosition = 0;
    Profiler mProfiler;
    Pendulum mPendulum;
    PID mCartPID;
    PID mPendulumPID;

    void swingUp();
    void timespecAddUs(struct timespec *t, long us);
};

#endif // PENDULUMCONTROLLER_H