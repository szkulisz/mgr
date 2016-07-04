#include "pid.h"

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
    float p = mKp*error;
    float i = mIPrev + mKi*mTs*error;
    float d = (mKd*mN*(PV - mPVPrev) + mDPrev) / (1 + mN*mTs);

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
    mTs = params["Ts"];
    mIsConstrained = params["constr"];
    mCVmax = params["CVmax"];
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

