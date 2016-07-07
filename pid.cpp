#include "pid.h"
#include <iostream>
using namespace std;

PID::PID()
{

}

PID::PID(map<string, float> params)
{
    setParameters(params);
}

float PID::control(float SP, float PV)
{
    if (mParamsChanged) {
        mPVPrev = PV;
        mParamsChanged = false;
    }
    float error = SP-PV;
    cout << "error " << error << endl;
    float p = mKp*error;
    cout << "p " << p << endl;
    float i = mIPrev + mKi*mTs*error;
    cout << "i " << i << endl;
    float d = (mKd*mN*(PV - mPVPrev) + mDPrev) / (1 + mN*mTs);
    cout << "d " << d << endl;

    mPVPrev = PV;
    mDPrev = d;
    mIPrev = i;
    return p+i+d;
}

void PID::setParameters(map<string, float> params)
{
    mParamsChanged = true;
    mKp = params["Kp"];
    mKi = params["Ki"];
    mKd = params["Kd"];
    mN = params["N"];
    map<string, float>::iterator it = params.find("Ts");
    if (it != params.end())
        mTs = params["Ts"];
    it = params.find("constr");
    if (it != params.end())
        mIsConstrained = params["constr"];
    it = params.find("CVmax");
    if (it != params.end())
        mCVmax = params["CVmax"];
    it = params.find("CVmin");
    if (it != params.end())
        mCVmin = params["CVmin"];
}

map<string, float> PID::getParameters()
{
    map<string, float> params;
    params["Kp"] = mKp;
    params["Ki"] = mKi;
    params["Kd"] = mKd;
    params["N"] = mN;
    params["Ts"] = mTs;
    params["constr"] = mIsConstrained;
    params["CVmax"] = mCVmax;
    params["CVmin"] = mCVmin;

    return params;
}

void PID::stop()
{
    mIPrev = 0;
    mDPrev = 0;
    mPVPrev = 0;
    mParamsChanged = true;
}

float PID::getSamplingTime() const
{
    return mTs;
}

void PID::setSamplingTime(float ts)
{
    mTs = ts;
}

