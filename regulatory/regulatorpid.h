#ifndef REGULATORPID_H
#define REGULATORPID_H

#include "regulatory/regulator.h"
class Odczyt_xml;
/*!
 * \brief The RegulatorPID class
 * Implementacja regulatora PID.
 */
class RegulatorPID : public Regulator
{
public:

    RegulatorPID(Odczyt_xml *x);
    RegulatorPID(std::map<std::string, float> parametry);
    ~RegulatorPID();

    float symuluj(float y);

private:
    float s_Tp=1;
    float s_b=1;
    float s_k=1;
    float s_Ti=2;
    float s_e=0;
    float s_N=1;
    float s_Td=1;
//    float s_w=0;

    bool s_ograniczone=false;
    float s_max=0;
    float s_min=0;

    float s_y_poprzednie=0;
    float s_I_poprzedni = 0;
    float s_D_poprzedni = 0;

    float proportional(float w, float y);
    float integral(float e);
    float differential(float y, float y_poprzedni);
    void sprawdz_parametry(void);
    void ustaw_parametry();
    void ustaw_parametry(std::map<std::string, float> parametry);
    void ustaw_parametry_PID(void);
};

#endif // REGULATORPID_H
