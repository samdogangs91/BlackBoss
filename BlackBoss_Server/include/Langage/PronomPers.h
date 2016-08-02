#ifndef PRONOMPERS_H
#define PRONOMPERS_H


#include <iostream>
#include <string>

#include "Mot.h"

class PronomPers: public Mot
{
    public:
        PronomPers();
        PronomPers(Mot*);
        PronomPers(std::string nom, Pf fun);
        PronomPers(std::string nom, std::fstream* dataF, std::string fun="rien");
        virtual ~PronomPers();
    protected:
    private:
};

#endif // PRONOMPERS_H
