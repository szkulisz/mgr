#ifndef DEKORATOR_H
#define DEKORATOR_H

#include <memory>
#include <random>

/**
 * @brief The Sygnal class
 *      Definicja interfejsu dla generatora sygnałów
 */
class Sygnal
{
public:
    Sygnal() {}
    virtual ~Sygnal() {}
    virtual float generuj (void) = 0;
};

/**
 * @brief The Sygnal_0 class
 *      bazowy, zerowy sygnał
 */
class Sygnal_0 : public Sygnal
{
public:
    Sygnal_0 () {}
    ~Sygnal_0 () {}
    float generuj () { return 0.0; }
};

/**
 * @brief The Dekorator class
 *      dekorator do generowania sygnałów
 */
class Dekorator : public Sygnal
{
public:
    Dekorator (Sygnal * p);
    virtual ~Dekorator ();
    virtual float generuj (void) = 0;
protected:
    Sygnal * s_proc;
    int s_kroki_symulacji;
};

/**
 * @brief The Sygnal_prost class
 *      klasa generująca sygnał prostokątny
 */
class Sygnal_prost : public Dekorator
{
public:
    Sygnal_prost (float amp, int okres, float fill, Sygnal * p);
    float generuj (void);
private:
    float s_amp;
    int s_okres;
    float s_wypelnienie;
};

/**
 * @brief The Sygnal_skok class
 *      klasa generująca sygnał skokowy
 */
class Sygnal_skok : public Dekorator
{
public:
    Sygnal_skok (float w, Sygnal * p);
    float generuj (void);
private:
    float s_wartosc;
};

/**
 * @brief The Sygnal_trojkat class
 *      klasa generująca sygnał trójkątny
 */
class Sygnal_trojkat : public Dekorator
{
public:
    Sygnal_trojkat (int okres, float amplituda, Sygnal * p);
    float generuj (void);
private:
    int s_okres;
    float s_amp;
};

/**
 * @brief The Sygnal_sinus class
 *      klasa generująca sygnał sinusoidalny
 */
class Sygnal_sinus : public Dekorator
{
public:
    Sygnal_sinus (int okres, float amplituda, Sygnal * p);
    float generuj (void);
private:
    int s_okres;
    float s_amp;

};

/**
 * @brief The Sygnal_impuls class
 *      klasa generująca impuls Kroneckera
 */
class Sygnal_impuls : public Dekorator
{
public:
    Sygnal_impuls (float amplituda, Sygnal * p);
    float generuj (void);
private:
    float s_amp;
};

class Sygnal_szum : public Dekorator
{
public:
    Sygnal_szum (float war, Sygnal * p);
    float generuj (void);
private:
    float s_wariancja;
    std::random_device rd;
    std::unique_ptr< std::mt19937 > mt;
    std::unique_ptr< std::normal_distribution< float > > dist;
};

#endif // DEKORATOR_H
