#include "Instruction.h"
#include <fstream>
#include <sstream>
#include <csignal>

using namespace std;

unsigned int sig=SIGUNUSED;

string cons="Cons";
string incons="Incons";

/*
 * Emission d'un event par un thread
 */
void Event(string src, string dest, string cont, string type)
{
    Event* ev= new Event(src,dest,cont,type);
    deliverEvent(ev);
    raise(sig);
    delete ev;
}

void deliverEvent(Event* ev)
{
    if(ev!=NULL)
    {
        //dest est de la forme "Cons_'id'" ou "Incons_'id'" où id est un nombre indiquant le numéro du thread
        string dest=ev->dest;
        unsigned int k;
        bool okUnder;
        string name="";
        string idstr="";
        for(k=0;k<dest.size();k++)
        {
            if(dest[k]=='_')
            {
                okUnder;
                continue;
            }
            if(okUnder)
            {
                name+=dest[k];
            }
            else
            {
                idstr=dest[k];
            }
        }

        unsigned int id;
        stringstream ss;
        ss<<idstr;
        ss>>id;

        if(name.compare(cons)==0)
        {
            //send(ev,cons,id);
        }
        else if(name.compare(incons)==0)
        {
            //send(ev,incons,id);
        }
        else
        {
            cout<<"Erreur: le nom "+name+" ne correspond à aucun thread"<<endl;;
        }
    }
}
