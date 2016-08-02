/* 
 * File:   InBlackIa.cpp
 * Author: mblack
 * 
 * Created on 20 octobre 2015, 22:21
 */

#include "../../include/IA/InBlackIa.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
//#include "Mot.h"
//#include "Database.h"
#include "../../include/IA/Sujet.h"
#include "../../include/Langage/Determinant.h"
#include "../../include/Database/Database.h"
#include "../../include/Environnement/Event.h"
#include "../../include/IA/other.h"

using namespace std;

extern std::string Adj;
extern std::string Adv;
extern std::string Conj;
extern std::string Det;
extern std::string Nom;
extern std::string NomP;
extern std::string Pre;
extern std::string Pro;
extern std::string ProP;
extern std::string Verb;

extern string part;
extern string passe;
extern string imp;
extern string condi;
extern string subjo;
extern string pres;
extern string imperat;
extern string fut;

extern string msgTalk;
extern string msgThink;

extern User* Vous;
extern Sujet* sjt;
extern MYSQL data;
extern Database * memory;

DatabaseFun* Dfun;
DatabaseMot* Dmots;
DatabasePers* Dpers;
DatabasePers* DpersPre;
DatabaseUser* Dusers;
DatabaseVerb* Dverbs;

extern string cons;
extern string Incons;
extern string userStr;
extern unsigned int sigIncons;
extern unsigned int sigCons;


void InBlackIa::start()
{
    isAlive=true;
    _activity.start();
}

void InBlackIa::stop()
{
    _activity.stop();
    _activity.wait();
    isAlive=false;
}


InBlackIa::InBlackIa() :_activity(this,&InBlackIa::Behaviour),_activThink(this,&InBlackIa::Think)
{
    isOk=false;
    mots= new DatabaseMot();
    Dmots=mots;
    pers=new DatabasePers();
    Dpers=pers;
    persPre=new DatabasePers();
    DpersPre=persPre;
    users=new DatabaseUser();
    Dusers=users;
    
    verbs=new DatabaseVerb();
    Dverbs=verbs;
    fun=new DatabaseFun();
    Dfun=fun;
    doNothing=false;
    lvlPers=0;
    
}

void InBlackIa::startThink()
{
    _activThink.start();
}

void InBlackIa::stopThink()
{
    _activThink.stop();
    _activThink.wait();
}


void InBlackIa::toCons(std::string s)
{
    this->hasCons=true;
    thinkIncons=s;
    
}

void InBlackIa::Think()
{
    while(!_activThink.isStopped())
    {
        if(isOk)
        {
            if(doNothing)
            {
                
                Poco::Thread::sleep(1);
            }
        }
    }
}


bool InBlackIa::modeApprentissage(string s)
{
    return (s.compare("je vais t'apprendre quelque chose")==0)||(s.compare("je vais t'apprendre quelque chose!")==0)||(s.compare("je veux t'apprendre quelque chose")==0)||(s.compare("je veux t'apprendre quelque chose!")==0)||(s.compare("je vais t'apprendre quelque chose!")==0)||(s.compare("mode apprentissage")==0)||(s.compare("mode apprentissage!")==0);
}


bool InBlackIa::auRevoir(string s)
{
    return (s.compare("au revoir")==0);
}




void InBlackIa::funHandler()
{
    fun->insert("rien",rien);
    fun->insert("qui_Est",qui_Est);
    fun->insert("que_fait",que_fait);
    fun->insert("est",est);
    
}

void InBlackIa::Talk(std::string s)
{
    event(cons,Incons,s,msgTalk,sigIncons);
    //toTalk=s;
    //hasTalk=true;
}



void InBlackIa::hasEvents()
{
    //cout<<"hasEvents incons"<<endl;
    if(events.size()>0)
    {
        Event* ev=events.front();
        if(ev->type.compare(msgTalk)==0)
        {
            hasTalk=true;
            hasEvent=(events.size()-1)>0;
        }
        else if(ev->type.compare(msgThink)==0)
        {
            
        }
        //events.pop();
    }
    //hasTalk=false;
    
}


void InBlackIa::apprendrePers(fstream* dataF)
{
    string s="présente moi une nouvelle personne";
    Talk(s);
    bool sexe=askSexe();
    Prenom* prenom=askPrenom(sexe);
    NomPropre* nom=askNom();
    Prenom* pseudo=askPseudo(sexe);
    string def=askDefine(prenom);
    int age=askAge();
    int taille=askTaille();
    int poids=askPoids();
    string skinColor=askSkinColor();
    string hairColor=askHairColor();
    string eyeColor=askEyeColor();
    Personne* pers1=new Personne(prenom,nom,pseudo,sexe,def,age,taille,poids,skinColor,hairColor,eyeColor,dataF);


}


void InBlackIa::apprendreMot(string s, fstream* file)
{
 bool rep=false;
    string repVous="";
    string str;
    while(!rep)
        {
        str="Quel est le type du mot '";
        str+=s;
        str+="'?";
        Talk(str);
        repVous=Vous->Talk();
        rep=((repVous.compare(Adj)==0)||(repVous.compare(Adv)==0)||(repVous.compare(Conj)==0)||(repVous.compare(Nom)==0)||(repVous.compare(NomP)==0)||(repVous.compare(Pre)==0)||(repVous.compare(Pro)==0)||(repVous.compare(ProP)==0)||(repVous.compare(Verb)==0)||(repVous.compare(Adj)==0));
        }
    if(repVous.compare(Adj)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un adjectif";
        Talk(str);
        }
    else if(repVous.compare(Adv)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un adverbe";
        Talk(str);

        }
    else if(repVous.compare(Conj)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est une conjonction";
        Talk(str);
        }
    else if(repVous.compare(Det)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un determinant";
        Talk(str);
        }
    else if(repVous.compare(Nom)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un nom";
        Talk(str);
        }
    else if(repVous.compare(NomP)==0)
        {
        str="ok donc '";
        str+=s;
        str+="\' ";
        str+="est un nom propre";
        Talk(str);
        }
    else if(repVous.compare(Pre)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un prenom";
        Talk(str);
        }
    else if(repVous.compare(Pro)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un pronom";
        Talk(str);
        }
    else if(repVous.compare(ProP)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un pronom personnel";
        Talk(str);
        }
    else if(repVous.compare(Verb)==0)
        {
        str="ok donc '";
        str+=s;
        str+="' ";
        str+="est un verbe";
        Talk(str);
        }   
}

Mot* InBlackIa::findMot(string mot)
{
    return mots->findMot(mot);
}

Verbe* InBlackIa::findVerb(std::string fname)
{
    return verbs->findVerb(fname);
}

string InBlackIa::comprehension(std::string s)
{
    string rep="";
   Phrase* ph=new Phrase(s);
   int k;
    std::map<string,Mot*>::iterator itMot;
    for(k=0;k<ph->size;k++)
    {
        string actual=ph->exp[k];
        string query="select basicName,type,fun from mot where basicName=\""+actual+"\";";
        mysql_query(&data,query.c_str());
        MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        result=mysql_store_result(&data);
        if(result!=NULL)
        {
            while(row=mysql_fetch_row(result))
            {
                string type=row[1]? row[1]: "";
                Mot* actualMot;
                Verbe* actualVerb;
                if(type.compare(Verb)!=0)
                {
                    actualMot=findMot(actual);
                    int k;
                    for(k=0;k<actualMot->foncs.size();k++)
                    {
                        rep=actualMot->foncs[k](s);
                        if(rep.compare("")!=0) break;
                    }
                    if(rep.compare("")!=0) break;
                }
                else
                {
                    actualVerb=findVerb(actual);
                    int k;
                    for(k=0;k<actualVerb->foncs.size();k++)
                    {
                        rep=actualVerb->foncs[k](s);
                        if(rep.compare("")!=0) break;
                    }
                    if(rep.compare("")!=0) break;
                }
            }
        }
        if(rep.compare("")!=0) break;
    }
    delete ph;
    return rep;
}


Personne* InBlackIa::findPersonne(string fname)
{
    return pers->findPersonne(fname);
}


void InBlackIa::Behaviour()
{
//BlackIa BlackBoss= new BlackIa();
    while(!_activity.isStopped())
    {
        startThink();
        memory=new Database();
         fstream dataF("data.sql",ios::in|ios::out|ios::ate);
         if(dataF)
         {

             funHandler();
             bool fin=false;
             isOk=initIa(&dataF);
             while (!fin)
             {
                 if(hasEvent)
                 {
                    hasEvents();
                 }
                 if(hasTalk)
                 {
                    if(events.size()>0)
                    {
                        string s=events.front()->cont;
                        doNothing=false;
                        //cout<<"user: "<<s<<endl;
                        modeApp=modeApprentissage(s);
                        fin=auRevoir(s);
                        Phrase* ph=new Phrase(s);
                        sjt->makeSujet(ph);
                        //cout<<"Sujet actuel: "<<sjt->nom<<endl;
                        if(modeApp /*&& Vous->basicName=="MBlack"*/)
                            {
                                events.pop();
                                apprendrePers(&dataF);
                                Talk("merci "+Vous->prenom->basicName+" j'apprends quelque chose");
                                modeApp=false;
                            }

                        else if(fin)
                            {

                            break;
                            }
                        else
                            {
                            string repo="";
                            repo=comprehension(s);
                            if(repo.compare("")==0)
                                {
                                Talk("Ok!");
                                }
                            else
                                {
                                Talk(repo);
                                }
                            events.pop();
                            }
                        
                        delete ph;
                    }
                    else
                    {
                        hasTalk=false;
                    }
                 }
                 Poco::Thread::sleep(1);
             }

             string bye="au revoir";
             Talk(bye);
             stopThink();
             stop();
             //break;
         }
         else
         {
             cout<<"Unable to open dataFile"<<endl;
         }
    }
    

}




bool InBlackIa::initIa(fstream* dataF)
{
    bool res=false;
   if(memory->ok)
   {
       while(1)
        {
            cout<<"Login:"<<endl;
            string login=VousTalk();
            cout<<"Mot de passe:"<<endl;
            bool admin;
            bool superAdmin;
            string mdp=VousTalkMdp();
            string query="select sexe,prenom,nom,pseudo,def,age,taille,poids,hairColor,skinColor,eyeColor,id,login from personne natural join user where personne.id in (select id from user where login=\""+login+"\" and mdp=\""+mdp+"\");";
            string query2="select sexe,prenom,nom,pseudo,def,age,taille,poids,hairColor,skinColor,eyeColor,id,login from personne natural join admin where personne.id in (select admin_id from admin where login=\""+login+"\" and mdp=\""+mdp+"\");";
            string query3="select sexe,prenom,nom,pseudo,def,age,taille,poids,hairColor,skinColor,eyeColor,id,login from personne natural join superAdmin where id in (select supAdmin_id from superAdmin where login=\""+login+"\" and mdp=\""+mdp+"\");";
            mysql_query(&data,query3.c_str());
            MYSQL_RES* result=NULL;
            MYSQL_ROW row;
            result=mysql_store_result(&data);
            if((row=mysql_fetch_row(result)))
            {
                cout<<"Mode super administrateur"<<endl;
                Vous=new User(row,dataF,true,true);
                mysql_free_result(result);
                //cout<<"ok user"<<endl;
                res=true;
                break;
            }
            else
            {
               mysql_query(&data,query2.c_str());
               result=mysql_store_result(&data);
                if((row=mysql_fetch_row(result)))
                {
                    cout<<"Mode administrateur"<<endl;
                    Vous=new User(row,dataF,true);
                    mysql_free_result(result);
                    res=true;
                    break;
                }
                else
                {
                    mysql_query(&data,query.c_str());
                    result=mysql_store_result(&data);
                     if((row=mysql_fetch_row(result)))
                     {
                         cout<<"Mode normal user"<<endl;
                         Vous=new User(row,dataF);
                         mysql_free_result(result);
                         res=true;
                         break;
                     }
                     else
                     {
                        cout<<"Utilisateur inconnu!"<<endl;
                        mysql_free_result(result);
                     }
                }
            }
        }


        string Bienvenue;
        Bienvenue="Bienvenue ";
        Bienvenue+=Vous->prenom->basicName;
        cout<<Bienvenue<<endl;
        mots->insert(Vous->prenom);
        mots->insert(Vous->nom);
        mots->insert(Vous->pseudo);
        pers->insert(Vous);
        users->insert(Vous);

         
   }
   else
   {
       cout<<":/>Fichier memoire (data.sql) non trouvé"<<endl;
   }
   return res;
}

string InBlackIa::waitForUser()
{
    string rep="";
    while(!hasEvent)
    {
        Poco::Thread::sleep(10);
    }
    if(events.size()>0)
    {
        //cout<<"dans waitUser, hasEvent!"<<endl;
        Event* ev=events.front();
        //cout<<"dans waituser, ev.front"<<endl;
        if(ev->type.compare(msgTalk)==0)
        {
            rep=ev->cont;            
            events.pop();
            //cout<<"rep="<<rep<<endl;
            hasEvent=false;
        }
    }
    return rep;
}


bool InBlackIa::askSexe()
{
    bool ok=false;
    while(!ok)
    {
        string sx;
        string rep="Quel est le sexe de la  personne? (homme/femme)";
        Talk(rep);
        sx=waitForUser();
        if(sx.compare("homme")==0)
        {
            //cout<<"L'individu est donc un homme"<<endl;
            return true;
        }
        else if(sx.compare("femme")==0)
        {
            //cout<<"L'individu est donc une femme"<<endl;
            return false;
        }

    }
    return true;

}

Prenom* InBlackIa::askPrenom(bool isMale)
{
    bool ok=false;
    while(!ok)
    {
        string s;
        string rep="Quel est son prénom?";
        Talk(rep);
        //Poco::Thread::sleep(100);
        s=waitForUser();
        //cout<<"s="<<s<<endl;
        if(s.compare("")!=0) //s ne doit etre qu'un mot aussi!
        {
           Prenom* pre=(Prenom*)findMot(s);
           if(pre==NULL)
           {
               return new Prenom(s,isMale);
           }
           else if(pre->basicName.compare("")==0)
           {
              return new Prenom(s,isMale,!isMale); 
           }
           else return pre;
        }
    }
}

NomPropre* InBlackIa::askNom()
{
   bool ok=false;
    while(!ok)
    {
        string s;
        string rep="Quel est son nom?";
        Talk(rep);
        s=waitForUser();
        if(s.compare("")!=0)
        {
            NomPropre* np=(NomPropre*)findMot(s);
            if(np->basicName.compare("")!=0)
            {
                return np;
            }
            else
            {
                return new NomPropre(s);
            }
        }
    }
}

Prenom* InBlackIa::askPseudo(bool isMale)
{
   bool ok=false;
    while(!ok)
    {
        string s;
        string rep="Quel est son pseudo?";
        Talk(rep);
        s=waitForUser();
        if(s.compare("")!=0)
        {
           Prenom* pre=(Prenom*)findMot(s);
           if(pre==NULL)
           {
               return new Prenom(s,isMale);
           }
           else if(pre->basicName.compare("")==0)
           {
              return new Prenom(s,isMale); 
           }
           else return pre;
        }
    }
}

string InBlackIa::askDefine(Prenom* pre)
{
    //cout<<"dans askDefine"<<endl;
    bool ok=false;
    int k=0;
    while(!ok)
    {
        string s;
        string rep="Qui est "+pre->basicName+"? ("+pre->basicName+" est...)";
        Talk(rep);
        s=waitForUser();
        if(s.compare("")!=0)
            {
            Phrase* ph=new Phrase(s);
            if(ph->size>=3)
            {
                if(ph->exp[0].compare(pre->basicName)==0 && ph->exp[2].compare("est")==0)
                {
                    return s;
                }
            }
            delete ph;
            }

        k++;
    }
}


int InBlackIa::askAge()
{
    bool ok=false;
    while(!ok)
    {
        int n;
        string rep="Quel est son age?";
        Talk(rep);
        string s;
        s=waitForUser();
        sscanf(s.c_str(),"%d",&n);
        sscanf(s.c_str(),"%*[^\n]");
        //getchar();
        if(n>=0 && n<200)
        {
            return n;
        }
    }
}


int InBlackIa::askTaille()
{
   bool ok=false;
    while(!ok)
    {
        int n;
        string rep="Quel est sa taille en cm?";
        Talk(rep);
        string s;
        s=waitForUser();
        sscanf(s.c_str(),"%d",&n);
        sscanf(s.c_str(),"%*[^\n]");
        //getchar();
        if(n>=50 && n<200)
        {
            return n;
        }
    }
}


int InBlackIa::askPoids()
{
    bool ok=false;
    while(!ok)
    {
        int n;
        string rep="Quel est son poids en kilo?";
        Talk(rep);
        string s;
        s=waitForUser();
        sscanf(s.c_str(),"%d",&n);
        sscanf(s.c_str(),"%*[^\n]");
        //getchar();
        if(n>=3 && n<500)
        {
            return n;
        }
    }
}


string InBlackIa::askSkinColor()
{
    bool ok=false;
    while(!ok)
    {
        string s;
        string rep="Quel est sa couleur de peau? (blanc, mat, typé, métis, noir)";
        Talk(rep);
        s=waitForUser();
        if(s.compare("blanc")==0||s.compare("mat")==0||s.compare("typé")==0||s.compare("métis")==0||s.compare("noir")==0)
        {
            return s;
        }
    }
}


string InBlackIa::askHairColor()
{
   bool ok=false;
    while(!ok)
    {
        string s;
        string colors[12];
        colors[0]="blanc";
        colors[1]="blond";
        colors[2]="chatain";
        colors[3]="roux";
        colors[4]="brun";
        colors[5]="rouge";
        colors[6]="vert";
        colors[7]="bleu";
        colors[8]="violet";
        colors[9]="fuscia";
        colors[10]="gris";
        colors[11]="noir";
        string rep="Quel est sa couleur de cheveux? (blanc, blond, chatain, roux, brun, rouge, vert, bleu, violet, fuscia, noir)";
        Talk(rep);
        s=waitForUser();
        int k;
        for(k=0;k<12;k++)
        {
            if(s.compare(colors[k])==0)
            {
                return s;
            }
        }
    }
}


string InBlackIa::askEyeColor()
{
   bool ok=false;
    while(!ok)
    {
        string s;
        string rep="De quelle couleur sont ses yeux? (bleu, marron, vert, noir)\n";
        Talk(rep);
        s=waitForUser();
        if(s.compare("bleu")==0||s.compare("marron")==0||s.compare("vert")==0||s.compare("noir")==0||s.compare("noir")==0)
        {
            return s;
        }
    }
}


InBlackIa::~InBlackIa() {
}

