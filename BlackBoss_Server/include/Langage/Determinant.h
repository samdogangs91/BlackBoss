#ifndef DETERMINANT_H
#define DETERMINANT_H


#include <iostream>
#include <string>

#include "Mot.h"


class Determinant: public Mot
{
    public:
        Determinant(std::string fname);
        Determinant(std::string fname1,std::string fname2,std::string fname3,std::string fname4,bool isMascu, bool isFemi,bool fIsSing,bool fIsPlur,std::fstream* dataF,bool fIsDenm = true,bool fCanSing = true, bool fCanMasc = true, bool fCanFem = true, std::string fun = "rien");
        Determinant(Mot*);
        virtual ~Determinant();
    protected:
    private:
};

#endif // DETERMINANT_H
