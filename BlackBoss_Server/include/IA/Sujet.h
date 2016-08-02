/* 
 * File:   Sujet.h
 * Author: mblack
 *
 * Created on 11 octobre 2015, 20:07
 */

#ifndef SUJET_H
#define	SUJET_H

#include <string>
#include "../Langage/Verbe.h"
#include "phrase.h"
#include "../Langage/Nom.h"

class Sujet {
public:
    std::string type; //Personne, objet
    std::string nom;
    std::string nom2;
    bool knowNom;
    bool knowNom2;
    bool isMale;
    bool knowSexe;
    int numPers; //1=je, 2=tu etc..
    Verbe* action;
    bool hasAction;
    Verbe* etat;
    bool hasEtat;
    std::string def;
    Sujet();
    void makeSujet(Phrase* ph);
    Sujet(const Sujet& orig);
    virtual ~Sujet();
private:

};

#endif	/* SUJET_H */

