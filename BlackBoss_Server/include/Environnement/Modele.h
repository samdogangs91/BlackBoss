/* 
 * File:   Modele.h
 * Author: mblack
 *
 * Created on 29 octobre 2015, 01:46
 */

#ifndef MODELE_H
#define	MODELE_H

#include "../IA/comprehension.h"
#include "Variable.h"
#include <vector>

typedef Variable<std::string>* var;

typedef void (*Fun) (std::vector<var>,std::vector<var>,std::vector<var>*);

class Modele {
public:
    Modele(std::string fname);
    std::string name;
    void start();
    void stop();
    std::string msgMod; //message issu du modele
    bool hasMod; // a un message issu du modele
    std::string msgCons; //message issu de BlackBoss
    bool hasCons; //a un message conscient
    void game(); //deroulement du modele
    Fun funGame;
    std::vector<Variable<std::string>*> in; //variables d'entree
    std::vector<Variable<std::string>*> intern;// variables internes
    std::vector<Variable<std::string>*> out;// variables de sorties
    Modele(const Modele& orig);
    virtual ~Modele();
private:
    void formule(); //interraction entre les variables pour générer le modèle
    Fun funForm;
    Poco::Activity<Modele> _activity;
};

#endif	/* MODELE_H */

