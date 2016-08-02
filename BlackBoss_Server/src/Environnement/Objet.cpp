#include "../../include/Environnement/Objet.h"
#include "../../include/IA/BlackIa.h"
#include "../../include/Environnement/Personne.h"
#include "../../include/Langage/Mot.h"



using namespace std;


string perso="personne";
string mot="mot";

extern BlackIa* BlackBoss;
extern User* Vous;

Objet::Objet()
{

}


Objet::Objet(string s)
{
    string str;
str="'";
str+=s;
str+="' ";
str+="est une personne ou un mot?";
string rep;
bool wh=false;
while(!wh)
    {
    BlackBoss->Talk(str);
    rep=Vous->Talk();
    //cout<<"'"<<rep<<"'"<<endl;
    wh=(rep.compare(perso)==0)||(rep.compare(mot)==0);
    }
//cout<<"'"<<rep<<"'"<<endl;
if(rep.compare(perso)==0)
    {
    new Personne(s);
    }
else if(rep.compare(mot)==0)
    {
    //cout<<"cr�ation d'un mot"<<endl;
    new Mot(s);
    }
}





Objet::~Objet()
{
    //dtor
}
