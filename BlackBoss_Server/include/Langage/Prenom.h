#ifndef PRENOM_H
#define PRENOM_H


#include <iostream>
#include <string>

#include "Mot.h"

//class Mot;

class Prenom: public Mot
{
    public:
        Prenom(std::string fname);
        Prenom(std::string,bool fIsMasc,bool fIsFem=false);
        Prenom(std::string, bool sexe, std::fstream* dataF);
        Prenom(std::string,bool fIsMasc, bool fIsFem, std::fstream* dataF);
        Prenom(std::string, bool, std::string);
        Prenom(std::string fname, std::fstream* dataF, bool fIsMasc=true, bool fIsFem=true );
        virtual ~Prenom();
    protected:
    private:
};

#endif // PRENOM_H
