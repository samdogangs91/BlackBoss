#ifndef NOM_H
#define NOM_H

#include <iostream>
#include <string>

#include "Mot.h"

class NomC: public Mot
{
    public:
        NomC();
        NomC(std::string fname);//recherche les attributs du nom dans la base de données
        NomC(std::string fname1,std::string fname2,std::string fname3,std::string fname4,bool isMascu, bool isFemi,bool fIsSing,bool fIsPlur,std::fstream* dataF, bool fIsDenm = true,bool fCanSing = true, bool fCanMasc = true, bool fCanFem = true, std::string fun = "rien");
        virtual ~NomC();
    protected:
    private:
};

#endif // NOM_H
