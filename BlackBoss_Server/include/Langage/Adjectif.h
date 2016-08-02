#ifndef ADJECTIF_H
#define ADJECTIF_H


#include <iostream>
#include <string>

#include "Mot.h"


class Adjectif: public Mot
{
    public:
        Adjectif(Mot*);
        Adjectif(std::string fname); 
        Adjectif(std::string fname1,std::string fname2,std::string fname3,std::string fname4,bool isMascu, bool isFemi,bool fIsSing,bool fIsPlur,std::fstream *dataF,bool fIsDenm = true,bool fCanSing = true, bool fCanMasc = true, bool fCanFem = true, std::string fun = "rien");
        virtual ~Adjectif();
    protected:
    private:
};

#endif // ADJECTIF_H
