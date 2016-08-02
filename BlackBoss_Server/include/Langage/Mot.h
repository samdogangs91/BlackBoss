#ifndef MOT_H
#define MOT_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "../Environnement/Objet.h"
#include "../IA/comprehension.h"


typedef std::string (*Pf)(std::string);

class Mot:public Objet
{
    public:
        //std::string mot; //orthographe du mot
        std::string def; //définition du mot
        bool isMasc;//masculin
        bool isFem;
        bool isSing;
        bool isPlur;
        bool isDenm;//d�nombrable ou pas
        bool canSing;//peut etre singulier
        bool canFem;//peut etre feminin
        bool canMasc;//peut etre masculin
        std::string sing;
        std::string fem;
        std::string plur;
        std::string plurFem;
        std::string catMot; //catégorie du mot : adjectif, pr�nom, etc...
        std::vector<std::string> sFoncs;
        std::vector<Pf> foncs; //méthodes propres au mot
        
        std::string askDef();
        void askGenre();
        bool askIsDenm();
        bool askIsSing();
        std::string askSing();
        std::string askFem();
        std::string askPlur();
        std::string askPlurFem();
        Mot();
        Mot(std::string);
        Mot(std::string, Pf);
        Mot(std::string fname, std::string ftype,std::string fun);
        Mot(Mot*);
        virtual ~Mot();
    protected:
    private:
};

#endif // MOT_H
