/* 
 * File:   InBlackIa.h
 * Author: mblack
 *
 * Created on 20 octobre 2015, 22:21
 */

#ifndef INBLACKIA_H
#define	INBLACKIA_H

#include "../Database/Database.h"
#include "../Environnement/Personne.h"
#include <string>
#include "comprehension.h"
#include "../Database/DatabaseFun.h"
#include "../Database/DatabaseMot.h"
#include "../Database/DatabasePers.h"
#include "../Database/DatabaseUser.h"
#include "../Database/DatabaseVerb.h"
#include "../Database/DatabaseModel.h"
#include "../Langage/PronomPers.h"
#include "../Langage/Pronom.h"
#include "../Langage/Adjectif.h"
#include "../Environnement/User.h"
#include "comprehension.h"
#include "phrase.h"

#include "Poco/Activity.h"
#include "Poco/Thread.h"

class DatabaseVerb;

class InBlackIa {
public:
    bool isOk;
    bool isAlive;
    bool doNothing;
    bool initIa(std::fstream* dataF);
    std::string waitForUser();
    std::string toTalk;
    bool hasTalk;
    std::string thinkCons; //message conscient
    bool hasCons; //a un message conscient
    std::string thinkIncons;// message inconscient
    DatabaseModel* models;
    void toCons(std::string s);
    bool hasIncons;// a un message inconscient
    void start();
    void stop();
    bool modeApp;
    bool modeApprentissage(std::string);
    void Talk(std::string);
    bool auRevoir(std::string s);
    void funHandler();
    DatabasePers* pers;
    DatabaseMot* mots;
    DatabaseUser* users;
    DatabaseVerb* verbs;
    DatabasePers* persPre;
    DatabaseFun* fun;
    std::queue<Event*> events;
    void hasEvents();
    bool hasEvent;
    Personne* findPersonne(std::string fname);
    Mot* findMot(std::string mot);
    Verbe* findVerb(std::string verb);
    std::string comprehension(std::string);
    void Behaviour();
    void basicNames(std::fstream* dataF);
    void basicVerbs(std::fstream* dataF);
    void basicAdj(std::fstream* dataF);
    void basicDet(std::fstream* dataF);
    void initPers(std::fstream * file, std::fstream* dataF);
    void initMot(std::fstream * file);
    void basicLearning();
    void basicNouns(std::fstream* dataF);
    void initIa(std::fstream* file,std::fstream* file2,std::fstream* dataF);
    void quiEst(std::string rep);
    bool learn(std::string);
    void apprendrePers(std::fstream* file);
    void apprendreMot(std::string s, std::fstream* file);
    void Think(void);
    void startThink();
    void stopThink();
    
    Prenom* askPrenom(bool masc);
    bool askSexe();
    std::string askDefine(Prenom* pre);
    NomPropre* askNom();
    Prenom* askPseudo(bool masc );
    int askAge();
    int askTaille();
    int askPoids();
    std::string askVille();
    std::string askHairColor();
    std::string askSkinColor();
    std::string askEyeColor();
    std::string askDate();
    
    InBlackIa();
    virtual ~InBlackIa();
private:
    Poco::Activity<InBlackIa> _activity;
    Poco::Activity<InBlackIa> _activThink;
    int lvlPers;

};

#endif	/* INBLACKIA_H */

