#ifndef PREPOSITION_H
#define PREPOSITION_H


#include <iostream>
#include <string>

#include "Mot.h"

class Preposition: public Mot
{
    public:
        Preposition();
        Preposition(Mot*);
        virtual ~Preposition();
    protected:
    private:
};

#endif // PREPOSITION_H
