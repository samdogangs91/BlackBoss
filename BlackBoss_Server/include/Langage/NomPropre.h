#ifndef NOMPROPRE_H
#define NOMPROPRE_H


#include <iostream>
#include <string>

#include "Mot.h"

//class Mot;

class NomPropre:public Mot
{
    public:
        NomPropre(std::string fname);
        NomPropre(std::string fname,std::fstream* dataF);
        NomPropre(Mot*m);
        virtual ~NomPropre();
    protected:
    private:
};

#endif // NOMPROPRE_H
