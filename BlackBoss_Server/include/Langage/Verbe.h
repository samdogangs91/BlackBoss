#ifndef VERBE_H
#define VERBE_H


#include <iostream>
#include <string>

#include "Mot.h"
#include "Conjugaison.h"
#include "../IA/InBlackIa.h"

class Conjugaison;
//class Mot;

class Verbe: public Mot 
{
    public:
        std::string inf;
        int groupe;
        int idTemps; //identifiant du temps de conjugaison du verbe dans basicName
        std::string temps;
        bool isEtat;
        bool isAux;
        int pers; //numero du pronom personnel correspodant ex: je=1, tu =2, etc... 0 pour l'infinitif du verbe
        Conjugaison* past; //idTemps=0
        Conjugaison* imparf;//idTemps=1
        Verbe* participe;//idTemps=2
        Conjugaison * cond;//idTemps=3
        Conjugaison * subj;//idTemps=4
        Conjugaison* present;//idTemps=5
        Conjugaison *futur;//idTemps=6
        Conjugaison* imper;
        Verbe* partPresent;
        void addFun(Pf fun);
        void makeVerbReg(std::string infini);
        void makeVerbIrreg();
        Verbe();
        Verbe(std::string fname); //chargement de tous les verbes liés à fname dans la mémoire interne de BlackIa
        Verbe(std::string name, std::string infini, std::string tmps, int numPers, int fgroupe, std::string fun="rien", bool fIsEtat=false, bool fIsAux=false);
        Verbe(std::string name, std::string infini, std::string tmps, int numPers, std::fstream* dataF, int fgroupe, std::string fun="rien", bool fIsEtat=false, bool fIsAux=false);
        Verbe(std::string infini,Conjugaison* pastConj,Conjugaison* impConj, Conjugaison* condConj,Conjugaison* subjConj,Conjugaison* presConj, Conjugaison* imperConj, Conjugaison* futConj,std::string participe, std::string pPres, std::fstream* dataF, std::string fun="rien", bool fIsEtat=false, bool fIsAux=false,int fgroupe=3); //pour les verbes du 3e groupe
        Verbe(std::string infini, std::fstream* dataF, std::string fun="rien", bool fIsEtat=false, bool fIsAux=false); //pour les verbes du 1er et 2e groupe
        Verbe(Mot*);
        virtual ~Verbe();
    protected:
    private:
};

#endif // VERBE_H
