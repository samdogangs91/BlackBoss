#ifndef CONJONCTION_H
#define CONJONCTION_H


#include <iostream>
#include <string>

#include "Mot.h"

class Conjonction: public Mot
{
    public:
        Conjonction();
        Conjonction(Mot*);
        virtual ~Conjonction();
    protected:
    private:
};

#endif // CONJONCTION_H
