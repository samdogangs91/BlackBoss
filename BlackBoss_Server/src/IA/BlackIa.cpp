#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
//#include "Mot.h"
//#include "Database.h"
#include "../../include/IA/BlackIa.h"
#include "../../include/IA/Sujet.h"
#include "../../include/Langage/Determinant.h"
#include "../../include/Database/Database.h"
#include "../../include/Environnement/Event.h"
#include "../../include/IA/other.h"

using namespace std;

extern string msgTalk;
extern string msgThink;
extern unsigned int sigCons;
extern string userStr;
extern string Incons;
extern string cons;
extern User* Vous;
Sujet* sjt=new Sujet();

string nameIa;
string defIa;

BlackIa::BlackIa(string fname): _activity(this, &BlackIa::Behaviour)
{
    nom=fname;
    nameIa=nom;;
    isAlive=true;
    hasRep=true;
    incons=new InBlackIa();
    def="Je suis BlackBoss, une intelligence artificielle créée par Mathieu Black afin d'aider à la résolution de problème";
    defIa=def;
}

bool BlackIa::bye(std::string s)
{
    return (s.compare("au revoir")==0)||(s.compare("Au revoir")==0); 
}

void BlackIa::Talk(string s)
{
    string rep;

    rep=nom+":/> "+s+"\n";

    event(userStr,cons,rep,msgTalk,sigCons);
    hasRep=true;
    if(bye(s))
    {
        isAlive=false;
    }
    Vous->hasTalk=false;
    
}

bool BlackIa::accept(string s)
{
    bool res=false;
    jeu=s;
    return res;
}

Modele* BlackIa::findModel(std::string fname)
{
    return realMod->find(fname);
}


void BlackIa::hasEvents()
{
    //cout<<"hasEvents cons"<<endl;
    unsigned int size=events.size();
    if(size>0)
    {
        //cout<<"while"<<endl;
        Event* ev=events.front();
        if(ev->src.compare(Incons)==0)
        {
            if(ev->type.compare(msgTalk)==0)
            {
                hasTalk=true;
                hasEvent=(events.size()-1)>0;
            }
            else if(ev->type.compare(msgThink)==0)
            {
                hasTalk=false;
            }
            else
            {
                hasTalk=false;
            }
        }
        else if(ev->src.compare(userStr)==0)
        {
            if(ev->type.compare(msgTalk)==0)
            {
                //cout<<"event from user"<<endl;
                string _cont=ev->cont;
                string _type=ev->type;
                events.pop();
                event(Incons,cons,_cont,_type,sigCons);
            }
        }
        //events.pop();
        size=events.size();
    }
    else
    {
        hasTalk=false;
        hasEvent=false;
    }
    //cout<<"hasEvent"<<endl;
    //hasTalk=false;
    //hasEvent=false;
}

void BlackIa::pushEvent(Event* ev)
{
    cout<<"push"<<endl;
    events.push(ev);
}

void BlackIa::Behaviour()
{
    while(!_activity.isStopped())
    {
        incons->start();
        while(!incons->isOk)
        {
            Poco::Thread::sleep(1);
        }
        while(isAlive)
        {
            if(!incons->isAlive)
            {
               isAlive=false;
            }
            if(events.size()==0)
            {
               hasEvent=false; 
            }
            if(hasEvent)
            {
                hasEvents();
            }
            if(hasTalk) //a modifier
            {
                if(events.size()>0)
                {
                    if(events.front()->type.compare(msgTalk)==0)
                    {
                        Talk(events.front()->cont);
                        events.pop();
                        hasTalk=false;
                    }
                    else
                    {
                        hasTalk=false;
                    }
                }
                else
                {
                    hasTalk=false;
                }
            }
            else
            {
                incons->doNothing=true; //signale à l'inconscience qu'il ne fait rien
                if(incons->hasIncons)// si l'inconscient a un message
                {
                   if(accept(incons->thinkIncons)) //si la proposition inconsciente est acceptée
                   {
                       string nameMod=incons->thinkIncons;
                       Modele* mod=findModel(nameMod);
                       if(mod!=NULL)
                       {
                            mod->start();
                            while(!Vous->hasTalk)// tant que l'utilisateur ne dit rien
                            {

                            }
                            mod->stop();
                       }
                   }
                }
            }


            Poco::Thread::sleep(1);
        }
        stop();
    }
    
    
}

void BlackIa::start()
{
    _activity.start();
}

void BlackIa::stop()
{

    _activity.stop();
    _activity.wait();
}

BlackIa::~BlackIa()
{
    delete incons;
}


