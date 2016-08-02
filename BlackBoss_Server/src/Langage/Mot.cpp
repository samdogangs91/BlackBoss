#include "../../include/Langage/Mot.h"
//#include "../../include/IA/BlackIa.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/DatabaseFun.h"
#include "../../include/IA/comprehension.h"
#include "../../include/Environnement/User.h"
#include "../../include/IA/phrase.h"
#include <iostream>
#include <string>


using namespace std;

std::string Adj="adjectif";
std::string Adv="adverbe";
std::string Conj="conjonction";
std::string Det="determinant";
std::string Nom="nom";
std::string NomP="nomPropre";
std::string Pre="prenom";
std::string Pro="pronom";
std::string ProP="pronomPersonnel";
std::string Verb="verbe";

//extern InBlackIa* InBoss;
extern User* Vous;
extern DatabaseMot* Dmots;
extern DatabaseFun* Dfun;

Mot::Mot()
{

}


Mot::Mot(string s)
{
    bool rep=false;
    string repVous="";
    string str;
    while(!rep)
        {
        str="Quel est le type du mot '";
        str+=s;
        str+="'?";
        //InBoss->Talk(str);
        repVous=Vous->Talk();
        rep=((repVous.compare(Adj)==0)||(repVous.compare(Adv)==0)||(repVous.compare(Conj)==0)||(repVous.compare(Nom)==0)||(repVous.compare(NomP)==0)||(repVous.compare(Pre)==0)||(repVous.compare(Pro)==0)||(repVous.compare(ProP)==0)||(repVous.compare(Verb)==0)||(repVous.compare(Adj)==0));
        }
    if(repVous.compare(Adj)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un adjectif";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(Adv)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un adverbe";
        //InBoss->Talk(str);

        }
    else if(repVous.compare(Conj)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est une conjonction";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(Det)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un determinant";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(Nom)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un nom";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(NomP)==0)
        {
        str="ok donc '";
        str+=s;
        str+="\' ";
        str+="est un nom propre";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(Pre)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un prenom";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(Pro)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un pronom";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(ProP)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un pronom personnel";
        //InBoss->Talk(str);
        }
    else if(repVous.compare(Verb)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un verbe";
        //InBoss->Talk(str);
        }

}

Mot::Mot(std::string fname, std::string ftype, std::string fun)
{
    basicName=fname;
    ident=fname;
    catMot=ftype;
    Phrase* ph=new Phrase(fun);
    int k;
    for(k=0;k<ph->size;k++)
    {
        Pf actualFun=Dfun->find(ph->exp[k]);
        foncs.push_back(actualFun);
    }
    Dmots->insert(this);
}

Mot::Mot(Mot* m)
{
    if(m->cat.compare(Adj)==0)
        {

        }
    else if(m->cat.compare(Adv)==0)
        {

        }
    else if(m->cat.compare(Conj)==0)
        {

        }
    else if(m->cat.compare(Det)==0)
        {

        }
    else if(m->cat.compare(Nom)==0)
        {

        }
    else if(m->cat.compare(NomP)==0)
        {

        }
    else if(m->cat.compare(Pre)==0)
        {

        }
    else if(m->cat.compare(Pro)==0)
        {

        }
    else if(m->cat.compare(ProP)==0)
        {

        }
    else if(m->cat.compare(Verb)==0)
        {

        }

}



Mot::~Mot()
{
    //dtor
}
