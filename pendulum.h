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
    void getPositions(int &cart, int &pendulum);


    Encoder *mEncoder;
    AD5313 *mDAC;
    int mCartPosition;
    int mPendulumAngle;

    int getPendulumAngle() const;
    int getCartPosition() const;
    void getPositions();
};

#endif // WAHADLO_H
