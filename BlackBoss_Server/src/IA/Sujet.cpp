/* 
 * File:   Sujet.cpp
 * Author: mblack
 * 
 * Created on 11 octobre 2015, 20:07
 */

#include "../../include/IA/Sujet.h"
//#include "../../include/IA/BlackIa.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/DatabaseVerb.h"
#include "../../include/IA/phrase.h"


using namespace std;

//extern BlackIa* BlackBoss;
//extern InBlackIa* InBoss;
extern User* Vous;
extern Database* memory;

std::string adj="adjectif";
std::string adv="adverbe";
std::string conj="conjonction";
std::string det="determinant";
std::string nomC="nom";
std::string nomP="nomPropre";
std::string pre="prenom";
std::string pro="pronom";
std::string proP="pronomPersonnel";
std::string verb="verbe";

std::string pers="pers";
std::string obj="obj";
std::string ia="ia";

extern DatabaseMot* Dmots;
extern DatabaseVerb* Dverbs;
extern DatabasePers* Dpers;

extern string nameIa;
extern string defIa;

Sujet::Sujet() {
    knowSexe=false;
    knowNom=false;
    knowNom2=false;
    hasAction=false;
    hasEtat=false;
}

void Sujet::makeSujet(Phrase* ph)
{
    int k=0;
    bool knowAction=false;
    bool knowEtat=false;
    bool exKnowAction=hasAction;
    bool exKnowEtat=hasEtat;
    bool knowQui=false;
    bool ok=false;
    bool interro=false;
    bool qui=false;
    bool je=false;
    bool tu=false;
    bool il=false;
    bool elle=false;
    bool on=false;
    bool nous=false;
    bool vous=false;
    bool ils=false;
    bool elles=false;
    //cout<<"yo!"<<endl;
    for(k=0; k<ph->size; k++) //recherche du verbe
    {
       string actual=ph->exp[k];
       string query="select type from mot where basicName=\""+actual+"\";";
       MYSQL_RES* result=NULL;
       result=memory->request(query);
       MYSQL_ROW row;
       
       if(result!=NULL)
       {
           if(row=mysql_fetch_row(result))
           {
               string type=row[0]? row[0]:"";
               if(type.compare(verb)!=0)
               {
                    
                    Mot* m=Dmots->findMot(actual);
                    //cout<<"mot actuel: "<<m->basicName<<endl;
                    if(k==0 && m->basicName.compare("qui")==0)//si la phrase est une question commençant par qui
                    {
                        qui=true;

                    }
                    else if(!interro)
                    {
                        if(m->catMot.compare(proP)==0)
                        {
                           if(m->basicName.compare("je")==0)
                           {
                               je=true;
                               knowQui=true;
                           }
                           else if(m->basicName.compare("tu")==0)
                           {
                               tu=true;
                               knowQui=true;
                           }
                           else if(m->basicName.compare("il")==0)
                           {
                               il=true;
                           }
                        }
                    }
               }
               else
               {
                   Verbe* m=Dverbs->findVerb(actual);
                    if((!knowAction) && !knowEtat) //recherche du verbe
                    {
                        if(m->basicName.compare("")!=0)
                        {
                            Verbe* ver=m;
                            if(!ver->isEtat)
                            {
                              Verbe* ver2=Dverbs->findVerb(ver->inf);
                              if(ver2->basicName.compare("faire")==0)
                              {
                                  knowAction=true;
                                  knowEtat=exKnowEtat;
                                  hasAction=exKnowAction;
                                  hasEtat=exKnowEtat;
                              }
                              else
                              {
                                  action=ver2;
                                  //cout<<"Verbe reconnu: "<<action->basicName<<endl;
                                  knowAction=true;
                                  hasAction=true;
                              }
                            }
                            else
                            {
                                Verbe*ver2=Dverbs->findVerb(ver->inf);
                                etat=ver2;
                                knowEtat=true;
                                hasEtat=true;
                            }
                            switch(ver->pers)
                            {
                              case 1:
                                  if(je)
                                  {
                                      nom=Vous->basicName;
                                      def=Vous->def;
                                      type=pers;
                                      numPers=ver->pers;
                                      ok=true;
                                  }
                                  break;
                              case 2:
                                  if(tu)
                                  {
                                  nom=nameIa;
                                  def=defIa;
                                  type=ia;
                                  numPers=ver->pers;
                                  ok=true;
                                  }
                                  break;
                              case 3:
                                  numPers=ver->pers;
                                  break;
                              case 4:
                                  numPers=ver->pers;
                                  type=pers;
                                  break;
                              case 5:
                                  numPers=ver->pers;
                                  type=pers;
                                  break;
                              case 6:
                                  numPers=ver->pers;
                                  break;
                            }
                        }
                    }
               }
         
               
           }
       }
       mysql_free_result(result);
    }
    if(je)//recherche du sujet pour les 1ere et 2e personne
    {
       nom=Vous->basicName;
       def=Vous->def;
       type=pers;
       numPers=1;
       ok=true;
    }
    if(tu)
    {
        nom=nameIa;
        def=defIa;
        type=ia;
        numPers=2;
        ok=true;
    }
    if(!ok &&(knowAction ||knowEtat))
    {
        for(k=0;k<ph->size;k++)//recherche du sujet pour le case 3 ou 6
        {
            if(numPers!=6)
            {
                Verbe* ver;
                if(knowEtat)
                {
                    ver=etat;
                }
                else ver=action;
                string actual=ph->exp[k];
                if(!isSeparateur(actual[0]))
                {
                    //cout<<"mot actuel: "<<actual<<endl;
                    string query="select basicName from verbe where basicName=\""+actual+"\";";
                    MYSQL_RES* result;
                    result=memory->request(query);
                    MYSQL_ROW row;

                    if(result!=NULL)
                    {
                        if((row=mysql_fetch_row(result)))
                        {
                        }
                        else //si le mot n'est pas un verbe
                        {
                            Mot* m=Dmots->findMot(actual);
                            //cout<<"mot actuel: "<<m->basicName<<", type: "<<m->catMot<<endl;
                            if(m->basicName.compare("")!=0)
                            {
                                if(m->catMot.compare(nomC)==0)
                                 {
                                    nom=m->basicName;
                                    knowNom=true;
                                    isMale=m->isMasc;
                                    type=obj; 
                                    break;
                                 }
                                else if(m->catMot.compare(pre)==0) //a changer
                                 {
                                    nom=m->basicName;
                                    type=pers;
                                    //cout<<"type: "<<type<<endl;
                                    Personne* pers1=Dpers->findPersonne(m->basicName);
                                    if(pers1->knowPrenom && pers1->knowSexe)
                                    {
                                        if(pers1->prenom->basicName.compare("")!=0)
                                        {
                                            knowNom=true;
                                           isMale=pers1->sexe;
                                           knowSexe=true;
                                        }
                                    }
                                    break;
                                 }
                                 else if(m->catMot.compare(proP)==0)
                                 {
                                     if(m->basicName.compare("il")==0)
                                     {
                                         if(knowSexe)
                                         {
                                             if(!isMale)
                                             {
                                                 
                                                 //BlackBoss->Talk("De qui parles-tu? "+nom+" est une femme lol");
                                             }
                                         }
                                     }
                                     else if(m->basicName.compare("elle")==0)
                                     {
                                         if(knowSexe)
                                         {
                                             if(isMale)
                                             {
                                                 //BlackBoss->Talk("De qui parles-tu? "+nom+" est un homme lol");
                                             }
                                         }
                                     }
                                 }
                            }
                        }
                    }
                    mysql_free_result(result);
                }
                
            }
            else
            {
                
            }
                
        }
    }
}

Sujet::Sujet(const Sujet& orig) {
}

Sujet::~Sujet() {
}

