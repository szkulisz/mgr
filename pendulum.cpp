#include "pendulum.h"
#include <math.h>

Pendulum::Pendulum()
{
    mEncoder = new Encoder();
    mDAC = new AD5313();
}

Pendulum::~Pendulum()
{
    mDAC->writeVoltage(2.5f,1);
    delete mEncoder;
    delete mDAC;
}

int Pendulum::control(float u)
{
    return mDAC->writeVoltage(u + 2.5f, 1);
}

void Pendulum::getPositions(float &cart, float &pendulum)
{
    cart = mCartPosition;
    pendulum = mPendulumAngle;
}

void Pendulum::readEncoderValues()
{
    mEncoder->readValues(mCartEncoder, mPendulumEncoder);
    mPendulumAngle = (-mPendulumEncoder*M_PI/1000.f) + M_PI;
    mCartPosition = mCartEncoder/12844.f;
}

void Pendulum::resetEncoders()
{
    mEncoder->reset();
}

float Pendulum::getCartPosition() const
{
    return mCartPosition;
}

float Pendulum::getPendulumAngle() const
{
    return mPendulumAngle;
}

