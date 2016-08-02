#ifndef BLACKIA_H
#define BLACKIA_H

#include "InBlackIa.h"


class BlackIa
{
public:
    std::string nom;
    std::string def;
    bool isAlive;
    bool hasRep;
    bool hasTalk;
    std::queue<Event*> events;
    bool hasEvent;
    void hasEvents();
    std::string userTalk;
    DatabaseModel* realMod;
    InBlackIa* incons;
    std::string jeu;
    std::string action;
    void pushEvent(Event* ev);
    Modele* findModel(std::string fname);
    bool accept(std::string);
    bool bye(std::string s);
    void start();
    void stop();
    void ia();
    void Talk(std::string s);
    BlackIa();
    void Behaviour();
    BlackIa(std::string name);
    virtual ~BlackIa();
private:
    std::string thinkOut;
    Poco::Activity<BlackIa> _activity;
};

#endif // BLACKIA_H
