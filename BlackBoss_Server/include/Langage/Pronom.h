#ifndef PRONOM_H
#define PRONOM_H


#include <iostream>
#include <string>

#include "Mot.h"

class Pronom: public Mot
{
    public:
        Pronom();
        Pronom(Mot*);
        Pronom(std::string, Pf);
        Pronom(std::string fname, std::fstream* dataF, std::string fun="rien");
        virtual ~Pronom();
    protected:
    private:
};

#endif // PRONOM_H
