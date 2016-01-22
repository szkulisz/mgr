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
    wahadlo = new Wahadlo();
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
    wahadlo->odczytaj_pozycje();
    auto temp2 = s_reg1->symuluj(wahadlo->getS_pozycja_wozka());
//    std::cout << s_aktualne_wyjscie << std::endl;
//    std::cout << temp << std::endl;
//    std::cout << temp2 << std::endl;
    //s_aktualne_wyjscie = 0;
    wahadlo->steruj(temp2);
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
