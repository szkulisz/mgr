#include "pendulum.h"

Pendulum::Pendulum()
{
    mEncoder = new Encoder();
    mDAC = new AD5313();
}

Pendulum::~Pendulum()
{
    delete mEncoder;
    delete mDAC;
}

int Pendulum::control(float u)
{
    return mDAC->writeVoltage(u + 2.5f, 1);
}

void Pendulum::getPositions(int &cart, int &pendulum)
{
    mEncoder->readValues(cart, pendulum);
}

void Pendulum::getPositions()
{
    mEncoder->readValues(mCartPosition, mPendulumAngle);
}

int Pendulum::getCartPosition() const
{
    return mCartPosition;
}

int Pendulum::getPendulumAngle() const
{
    return mPendulumAngle;
}

