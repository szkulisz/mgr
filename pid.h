#ifndef PID_H
#define PID_H

#include <map>

using namespace std;

class PID
{
public:
    PID();
    PID(map<string, float> params);

    float control(float SP, float PV);
    void setParameters(map<string, float> params);
    void stop();

private:
    float mKp = 1;
    float mKd = 0;
    float mKi = 0;
    float mN = 10;
    float mTs = 0.001;

    float mIPrev = 0;
    float mPVPrev = 0;
    float mDPrev = 0;

    bool mIsConstrained=false;
    float mCVmin;
    float mCVmax;

    bool mParamsChanged = true;
};

#endif // PID_H
