#ifndef PENDULUMCONTROLLER_H
#define PENDULUMCONTROLLER_H

#include <QObject>
#include <QThread>
#include <pthread.h>
#include <QMutex>
#include <map>
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
    Controller();
    float getCartPosition();
    float getPendulumAngle();
    void startControlling();
    std::map<std::string, float>  getCartPIDParams();
    std::map<std::string, float>  getPendulumPIDParams();
    void setPendulumPIDParams(std::map<string, float> params);
    void setCartPIDParams(std::map<string, float> params);

    void setPeriod(int period);
    int getPeriod() const;
    int getSamplingFrequency() const;
    void setSamplingFrequency(int freq);
    void setCartSetpoint(float cartSetpoint);

    float getCartSetpoint() const;

    float getControlValue();
    float getElapsedTime();

public slots:
    void stopControlling();
    void finish();
    void quit();

signals:
    void paramsChanged();

private:
    bool mRunController = true;
    bool mRunPendulum = false;
    bool mRunPendulumInit = false;
    int mSwingPeriod = 10000;
    float mSwingCVMax = 0.7;
    int mControlPeriod = 1000;
    int mPeriod = 1000;
    float mElapsedTime = 0;
    int mPhase = Phase::NOTHING;
    float mPendulumAngle = 0;
    float mCartPosition = 0;
    QMutex mPositionsMutex;
    QMutex mParamsMutex;
    float mCartSetpoint = 0;
    float mPendulumSetpoint = 0;
    float mControlValue = 0;
    Pendulum mPendulum;
    PID mCartPID;
    PID mPendulumPID;
    map<string, float> mCartFutureParams;
    map<string, float> mPendulumFutureParams;
    int mFuturePeriod;
    bool mChangeParams = false;


    void swingUp();
    void control();
    void timespecAddUs(struct timespec *t, long us);

};

#endif // PENDULUMCONTROLLER_H
