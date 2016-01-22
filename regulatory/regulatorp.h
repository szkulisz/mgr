#ifndef REGULATORP_H
#define REGULATORP_H

#include "regulatory/regulator.h"

/**
 * @brief The RegulatorP class
 *      klasa regulatora typu P
 */
class RegulatorP : public Regulator
{
public:
    RegulatorP( Odczyt_xml *x );
    RegulatorP(std::map<std::string, float> parametry);
    ~RegulatorP();

    void ustaw_wzmocnienie(float k);
    float symuluj(float u);

};

#endif // REGULATORP_H
