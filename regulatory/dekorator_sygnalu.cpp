#include "dekorator_sygnalu.h"
#include <cmath>
/**
 * @brief Dekorator::Dekorator
 * @param p - obiekt klasy bazowej
 */
Dekorator::Dekorator(Sygnal * p)  : s_proc(p)
{
    s_kroki_symulacji = 0;
}

/**
 * @brief Dekorator::~Dekorator
 */
Dekorator::~Dekorator()
{
    delete s_proc;
}

// --------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Sygnal_prost::Sygnal_prost - konstruktor
 * @param amp - amplituda sygnału prostokątnego
 * @param okres
 * @param wypelnienie - w zakresie (0 - 1)
 * @param p - obiekt klasy bazowej
 */
Sygnal_prost::Sygnal_prost (float amp, int okres, float wypelnienie, Sygnal * p)  : Dekorator(p), s_amp(amp), s_okres(okres), s_wypelnienie(wypelnienie)
{
    if(s_okres <= 0)
    {
        s_okres = 1;
    }

}

/**
 * @brief Sygnal_prost::generuj
 *          generacja sygnału prostokątnego
 * @return sygnał wzbogacony o swój udział
 */
float Sygnal_prost::generuj ()
{
    float prostokat = 0.0;

    prostokat = ((s_kroki_symulacji % s_okres) < (s_wypelnienie * s_okres)) ? s_amp : 0;
    ++s_kroki_symulacji;
    return s_proc->generuj() + prostokat;
}

// --------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Sygnal_skok::Sygnal_skok
 * @param w - wartość skoku
 * @param p - obiekt klasy bazowej
 */
Sygnal_skok::Sygnal_skok (float w, Sygnal * p)  : Dekorator(p), s_wartosc(w)
{
}

/**
 * @brief Sygnal_skok::generuj
 *          generacja sygnału skokowego
 * @return sygnał wzbogacony o swój udział
 */
float Sygnal_skok::generuj ()
{
    ++s_kroki_symulacji;
    return s_proc->generuj() + s_wartosc;
}

// --------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Sygnal_trojkat::Sygnal_trojkat - konstruktor
 * @param okres
 * @param amplituda
 * @param p - obiekt klasy bazowej
 */
Sygnal_trojkat::Sygnal_trojkat (int okres, float amplituda, Sygnal * p)  : Dekorator(p), s_okres(okres), s_amp(amplituda)
{
}

/**
 * @brief Sygnal_trojkat::generuj
 *          generacja sygnału trójkątnego
 * @return sygnał wzbogacony o swój udział
 */
float Sygnal_trojkat::generuj ()
{
    float trojkat = 0.0;
    float pol_okresu = s_okres/2.0;
    if (( s_kroki_symulacji % s_okres ) <= pol_okresu)
        trojkat = static_cast<float>(( s_kroki_symulacji % s_okres ) / pol_okresu) * s_amp;
    else
        trojkat = (1 - ( s_kroki_symulacji % static_cast<int>(pol_okresu)) / pol_okresu) * s_amp;

    ++s_kroki_symulacji;
    return s_proc->generuj() + trojkat;
}



// --------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Sygnal_sinus::Sygnal_sinus
 * @param okres
 * @param amplituda
 * @param p - obiekt klasy bazowej
 */
Sygnal_sinus::Sygnal_sinus (int okres, float amplituda, Sygnal * p)  : Dekorator(p), s_okres(okres), s_amp(amplituda)
{
}

/**
 * @brief Sygnal_sinus::generuj
 *          generacja sygnału sinusoidalnego
 * @return sygnał wzbogacony o swój udział
 */
float Sygnal_sinus::generuj ()
{
    float sinus = 0.0;

    sinus = s_amp*std::sin((static_cast<float>(s_kroki_symulacji % s_okres) / s_okres) * 6.28);

    ++s_kroki_symulacji;
    return s_proc->generuj() + sinus;
}

// --------------------------------------------------------------------------------------------------------------------------


/**
 * @brief Sygnal_impuls::Sygnal_impuls
 * @param amplituda
 * @param p - obiekt klasy bazowej
 */
Sygnal_impuls::Sygnal_impuls (float amplituda, Sygnal * p)  : Dekorator(p), s_amp(amplituda)
{
}

/**
 * @brief Sygnal_impuls::generuj
 *          generacja impulsu Kroneckera
 * @return sygnał wzbogacony o swój udział
 */
float Sygnal_impuls::generuj ()
{
    float wartosc = (s_kroki_symulacji == 0 ) ? s_amp : 0;

    ++s_kroki_symulacji;
    return s_proc->generuj() + wartosc;
}

Sygnal_szum::Sygnal_szum (float war, Sygnal *p) : Dekorator(p), s_wariancja(war)
{
    mt = std::unique_ptr< std::mt19937 >( new std::mt19937(rd()) );
    dist = std::unique_ptr< std::normal_distribution< float > >( new std::normal_distribution<float>(0.0, sqrt(s_wariancja)) );
}

float Sygnal_szum::generuj(){
    return dist->operator ()(*mt);
}

//wariancja_zaklocenia=0.1;

