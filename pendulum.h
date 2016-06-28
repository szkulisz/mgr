#ifndef WAHADLO_H
#define WAHADLO_H

#include "HARDWARE/encoder.h"
#include "HARDWARE/AD5313_DAC/ad5313.h"

class Pendulum
{
public:
    Pendulum();
    ~Pendulum();
    int control(float u);
    void getPositions(float &cart, float &pendulum);
    float getPendulumAngle() const;
    float getCartPosition() const;
    void readEncoderValues();

private:
    Encoder *mEncoder;
    AD5313 *mDAC;
    int mCartEncoder = 0;
    int mPendulumEncoder = 0;
    float mCartPosition = 0;
    float mPendulumAngle = 0;


};

#endif // WAHADLO_H
