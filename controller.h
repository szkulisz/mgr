#ifndef PENDULUMCONTROLLER_H
#define PENDULUMCONTROLLER_H

#include <QObject>
#include <QThread>
#include <pthread.h>
#include "profiler.h"
#include "pendulum.h"

#define CHECK(sts,msg)  \
  if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

class Controller : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;
public:
    void setPeriod(int period);
    float getCartPosition() { return mPendulum.getCartPosition(); }
    float getPendulumAngle() { return mPendulum.getPendulumAngle(); }


public slots:
    void stop();
    void finish();
    void quit();

signals:

private:
    bool mRunController = true;
    bool mRunPendulum = true;
    int mPeriod;
    Profiler mProfiler;
    Pendulum mPendulum;

//    void swingUp();
    void timespecAddUs(struct timespec *t, long us);
};

#endif // PENDULUMCONTROLLER_H
