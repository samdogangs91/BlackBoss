#include "../../include/IA/comprehension.h"
//#include "../include/IA/InBlackIa.h"
#include "../../include/IA/Sujet.h"
#include "../../include/Database/DatabasePers.h"
#include "../../include/Database/DatabaseVerb.h"

using namespace std;

//extern InBlackIa* InBoss;
extern Sujet* sjt;

extern string pers;
extern string ia;
extern DatabaseVerb* Dverbs;
extern DatabasePers* Dpers;

string qui_Est(string s)
 {
     string rep="";
     Phrase* ph=new Phrase(s);
     Verbe * est=Dverbs->findVerb("être");
     switch(sjt->numPers)
     {
         case 1:
             est=est->present->je;
             break;
         case 2:
             est=est->present->tu;
             break;
         case 3:
             est=est->present->il;
             break;
         case 4:
             est=est->present->nous;
             break;
         case 5:
             est=est->present->vous;
             break;
         case 6:
             est=est->present->ils;
             break;    
     }
     //cout<<"Verbe reconnu: "<<est->basicName<<endl;
     if(ph->size>4)
     {
         if((ph->exp[0].compare("qui")==0) && (ph->exp[1].compare(" ")==0)&& (ph->exp[2].compare(est->basicName)==0) && (ph->exp[3].compare(" ")==0))
         {
             if(sjt->type.compare(pers)==0)
             {
                string s=sjt->nom;
                if(ph->size>6)
                {  
                   s+=" ";
                   s+=ph->exp[6];
                   Personne* pers1=Dpers->findPersonne(s);
                   if(pers1->knowPrenom)
                   {
                       rep=pers1->def;
                   }
                   else
                   {
                       rep="je ne connais pas "+sjt->nom;
                   }
                }
               else
               {
                    
                   Personne* pers1=Dpers->findPersonne(s); 
                   if(pers1->knowPrenom)
                   {
                       rep=pers1->def;
                   }
                   else
                   {
                       rep="je ne connais pas "+sjt->nom;
                   }

               }
            }
             else if(sjt->type.compare(ia)==0)
             {
                 rep=sjt->def;
             }
             else
             {
                 rep="Il ne s'agit pas d'une personne! Mdr";
             }
        }
     }
     delete ph;
     return rep;
 }

string est(string s)
{
    string rep="";
    return rep;
}

string que_fait(string s)
{
    string rep="";
     Phrase* ph=new Phrase(s);
     map<string,Verbe*>::iterator itVerb;
     itVerb=Dverbs->db.find("faire");
     Verbe * infFaire=itVerb->second;
     Verbe* faire=infFaire;
     switch(sjt->numPers)
     {
         case 1:
             faire=faire->present->je;
             break;
         case 2:
             faire=faire->present->tu;
             break;
         case 3:
             faire=faire->present->il;
             break;
         case 4:
             faire=faire->present->nous;
             break;
         case 5:
             faire=faire->present->vous;
             break;
         case 6:
             faire=faire->present->ils;
             break;    
     }
     //cout<<"Verbe reconnu: "<<faire->basicName<<endl;
     
     if(ph->size >4)
     {
         if(ph->exp[0].compare("que")==0&& (ph->exp[1].compare(" ")==0)&& (ph->exp[2].compare(faire->basicName)==0) && (ph->exp[3].compare(" ")==0) )
         {
             if(sjt->hasAction)
             {
                switch(sjt->numPers)
                    {
                   case 1:
                       rep="Tu "+sjt->action->present->tu->basicName;
                       break;
                   case 2:
                       rep="Je "+sjt->action->present->je->basicName;
                       break;
                   case 3:
                       if(sjt->isMale)
                       {
                           rep="Il "+sjt->action->present->il->basicName;
                       }
                       else
                       {
                           rep="Elle "+sjt->action->present->il->basicName;
                       }
                       break;
                   case 4:
                       rep="Nous "+sjt->action->present->nous->basicName;
                       break;
                   case 5:
                       
                       break;
                   case 6:
                       
                       break;
                   }
             }
                else
                {
                    switch(sjt->numPers)
                    {
                    case 1:
                       rep="Je ne sais pas ce que tu fais.";
                       break;
                    case 2:
                       rep="Je ne fais rien";
                       break;
                    case 3:
                       if(sjt->isMale)
                       {
                           rep="Je ne sais pas ce qu'il fait.";
                       }
                       else
                       {
                           rep="Je ne sais pas ce qu'elle fait.";
                       }
                       break;
                   case 4:
                       //rep="Nous "+sjt->action->present->tu->basicName;
                       break;
                   case 5:
                       
                       break;
                   case 6:
                       
                       break;
                    }
                
                }
             }
         }
     
     delete ph;
    return rep;
}
