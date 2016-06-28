#include "petla.h"
#include <iostream>


/**
 * @brief Petla::Petla
 *      wymaga przekazania utworzonych regulatora i obiektu!!!
 * @param reg - wskaźnik do gotowego regulatora
 * @param ob - wskaźnik do gotowego obiektu dyskretnego
 */
Petla::Petla(Regulator *reg) {
    //nowy_regulator(reg);
    s_reg1 = reg;
    wahadlo = new Pendulum();
}

Petla::~Petla()
{
}


/**
 * @brief Petla::krok
 *      jeden krok symulacji.
 *      przekazuje aktualne wyjście do regulatora
 *      wyjście regulatora do wejścia obiektu
 */
void Petla::krok(){
    int temp;
    static float anglePrev, uMax = 0.5;
    float angle, deriv;
    int sgn;
    wahadlo->readEncoderValues();
    switch (mPhase) {
    case 0:
        angle = (-wahadlo->getPendulumAngle()*M_PI/1000) + M_PI;
        deriv = (angle - anglePrev)/0.01;
        anglePrev = angle;
        sgn = (deriv*cos(angle) >= 0) ? 1 : -1;
        wahadlo->control(sgn*uMax);
        if ((angle < M_PI/4) || (angle > (2*M_PI - M_PI/4)))
            uMax = 0.25;
        if (wahadlo->getCartPosition() > 0) {
            if ((angle < M_PI/8) || (angle > (2*M_PI - 0.02)))
                mPhase=1;
        } else {
            if ((angle > (2*M_PI - M_PI/8)) || (angle < (0 + 0.02)))
                mPhase=1;
        }

        break;
    default:
        wahadlo->control(0);
        break;
    }
    auto temp2 = s_reg1->symuluj(wahadlo->getCartPosition());

//    std::cout << s_aktualne_wyjscie << std::endl;
//    std::cout << temp << std::endl;
//    std::cout << temp2 << std::endl;
    //s_aktualne_wyjscie = 0;

//    wahadlo->steruj(temp2);
}


/**
 * @brief Petla::nowy_regulator
 *      resetuje wskaźnik na regulator,
 *      przypisując do niego przekazany wskaźnik
 * @param reg - obiekt regulatora
 */

void Petla::nowy_regulator(Regulator *reg){
    delete s_reg1;
    s_reg1 = reg;
}

