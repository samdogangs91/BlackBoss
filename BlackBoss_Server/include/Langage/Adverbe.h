#ifndef ADVERBE_H
#define ADVERBE_H

#include <iostream>
#include <string>

#include "Mot.h"

class Adverbe: public Mot
{
    public:
        Adverbe();
        Adverbe(Mot*m);
        virtual ~Adverbe();
    protected:
    private:
};

#endif // ADVERBE_H
