/* 
 * File:   User.cpp
 * Author: mblack
 * 
 * Created on 3 septembre 2015, 04:41
 */

#include "../../include/Environnement/User.h"
#include "../../include/Environnement/Event.h"
#include "../../include/IA/other.h"
//#include "../../include/IA/BlackIa.h"

//extern BlackIa* BlackBoss;
//extern InBlackIa* InBoss;


using namespace std;
extern string cons;
extern string Incons;
extern string userStr;
extern string msgTalk;
extern string servStr;

extern unsigned int sigUser;



User::User(string log, Personne* pers):_activity(this, &User::Behaviour)
{
   login=log;
   knowAge=pers->knowAge;
   if(knowAge) age=pers->age;
   basicName=pers->basicName;
   def=pers->def;
   knowEye=pers->knowEye;
   if(knowEye) eyeColor=pers->eyeColor;
   knowHair=pers->knowHair;
   if(knowHair) hairColor=pers->hairColor;
   ident=pers->ident;
   nom=pers->nom;
   prenom=pers->prenom;
   knowNom=true;
   knowPrenom=true;
   knowPseudo=pers->knowPseudo;
   if(knowPseudo) pseudo=pers->pseudo;
   
   knowPoids=pers->knowPoids;
   if(knowPoids) poids=pers->poids;
   
   knowSexe=pers->knowSexe;
   if(knowSexe) sexe=pers->sexe;
   
   knowSkin=pers->knowSkin;
   if(knowSkin) skinColor=pers->skinColor;
   
   knowTaille=pers->knowTaille;
   if(knowTaille) taille=pers->taille;
}

void User::hasEvents()
{
    while(events.size()>0)
    {
      Event* ev=events.front();
      //cout<<"in User"<<endl;
      //cout<<ev->cont<<endl;
      //Envoi vers le client
      if(ev->src.compare(cons)==0)
      {
          event(servStr,userStr,ev->cont,msgTalk,sigUser);
      }
      else if(ev->src.compare(servStr)==0)
      {
          event(cons,userStr,ev->cont,msgTalk,sigUser);
      }
      events.pop();
    }
    hasEvent=false;
}

string User::Talk()
{
    //string rep;
    cout<<login<<":/>";
    //cin>>rep;

    char rep[10000];
    fgets(rep,sizeof rep,stdin);
    int k=strlen(rep);
    rep[k-1]='\0';
    string trueRep;
    trueRep.append(rep);
    hasTalk=true;
    userTalk=trueRep;
    event(cons,userStr,trueRep,msgTalk,sigUser);
    //BlackBoss->hasRep=true;
    return trueRep;
}

User::User(MYSQL_ROW row,fstream* dataF, bool fadmin,bool fsuperAdmin): _activity(this, &User::Behaviour)
{
    hasTalk=false;
    
    admin=fadmin;
    superAdmin=fsuperAdmin;
    knowSexe=true;
    knowPrenom=true;
    knowNom=true;
    string s_sexe=row[0]? row[0]:"";
    sexe=(s_sexe.compare("M")==0);
    string s_pre=row[1]? row[1]: "";
    //cout<<"prenom: "<<s_pre<<endl;
    prenom=new Prenom(s_pre);
    basicName=prenom->basicName;
    string s_nom=row[2]? row[2]: "";
    //cout<<"nom: "<<s_nom<<endl;
    nom= new NomPropre(s_nom);
    string s_pse=row[3]? row[3]: "";
    pseudo= new Prenom(s_pse);
    def=row[4]? row[4]:"";
    string s_age=row[5]? row[5]: "";
    sscanf(s_age.c_str(),"%d",&age);
    string s_taille=row[6]? row[6]: "";
    sscanf(s_taille.c_str(),"%d",&taille);
    string s_poids=row[7]? row[7]: "";
    sscanf(s_poids.c_str(),"%d",&poids);
    hairColor=row[8]? row[8]:"";
    skinColor=row[9]? row[9]:"";
    eyeColor=row[10]? row[10]:"";
    ident=row[11]? row[11]:""; 
    login=row[12]? row[12]:""; 
    string ident_tmp;
    hasEvent=false;
    start();
    //cout<<"login ok"<<endl;
    /*identPre=prenom->basicName;
    identPre+=" ";
    //cout<<"ident ok"<<endl;
    int k=0;
    while(1)
    {
        char buff[4];
        sprintf(buff,"%d",k);
        ident_tmp=identPre;
        ident_tmp+=buff;
        cout<<"ident "<<ident_tmp<<endl;
        if(!InBoss->isOk)
        {
            cout<<"inBoss not ok"<<endl;
        }
        if(InBoss->persPre->db.count(ident_tmp)==0)
            {
                cout<<"end count"<<endl;
                break;
            }
        
        k++;
    }
    identPre=ident_tmp;
    cout<<"User ok"<<endl;
    InBoss->pers->insert(this);
    InBoss->persPre->db.insert(pair<string,Personne*>(identPre,this));*/
    
}

void User::start()
{
    _activity.start();
}

void User::stop()
{
    _activity.stop();
    _activity.wait();
}

void User::Behaviour()
{
    while(!_activity.isStopped())
    {
        while(hasEvent)
        {
            hasEvents();
        }
        Poco::Thread::sleep(20);
    }
}



User::~User() {
}

