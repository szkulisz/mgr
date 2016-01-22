#ifndef OBJEKT_SISO_H
#define OBJEKT_SISO_H


class Objekt_SISO
{
public:
    Objekt_SISO();
    virtual ~Objekt_SISO();

    virtual float symuluj(float u) = 0;
};

#endif // OBJEKT_SISO_H
