#include "../../include/Langage/Verbe.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/DatabaseFun.h"
#include "../../include/Database/DatabaseVerb.h"

using namespace std;
extern string Verb;
//extern InBlackIa* InBoss;

string part="participe";
string passe="past";
string imp="imparf";
string condi="cond";
string subjo="subj";
string pres="pres";
string fut="futur";
string imperat="imperat";
string partpre="ppres";

extern Database* memory;
extern DatabaseMot* Dmots;
extern DatabaseFun* Dfun;
extern DatabaseVerb* Dverbs;

Verbe::Verbe(std::string name, std::string infini, std::string tmps, int numPers, int fgroupe, std::string fun , bool fIsEtat, bool fIsAux)
{
    catMot=Verb;
    basicName=name;
    inf=infini;
    temps=tmps;
    pers=numPers;
    ident=basicName;
    isEtat=fIsEtat;
    isAux=fIsAux;
    groupe=fgroupe;
    string ffun=fun;
    Phrase* ph=new Phrase(ffun);
    int k;
    for(k=0;k<ph->size;k++)
    {
        Pf actualFun=Dfun->find(ph->exp[k]);
        foncs.push_back(actualFun);
    }
    delete ph;
    Dmots->insert(this);
    Dverbs->insert(this);
}


Verbe::Verbe(std::string name, std::string infini, std::string tmps, int numPers, fstream* dataF, int fgroupe, std::string fun, bool fIsEtat, bool fIsAux)
{
    catMot=Verb;
    basicName=name;
    inf=infini;
    temps=tmps;
    pers=numPers;
    ident=basicName;
    isEtat=fIsEtat;
    isAux=fIsAux;
    groupe=fgroupe;
    string query="insert into mot(basicName,type,fun) values(\""+basicName+"\",\""+catMot+"\",\""+fun+"\");";
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2,dataF);
    string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    char motGroupe[4];
    sprintf(motGroupe,"%d",groupe);
    string s_groupe=motGroupe;
    char motPers[4];
    sprintf(motPers,"%d",pers);
    string s_pers=motPers;
    string s_Etat=(fIsEtat)? ("true"):("false");
    string s_Aux=(fIsEtat)? ("true"):("false");
    string query3="insert into verbe(verbe_id,basicName,inf,numPers,temps,isAux,isEtat,numGr) values("+s_id+",\""+basicName+"\",\""+inf+"\","+s_pers+",\""+temps+"\","+s_Aux+","+s_Etat+","+s_groupe+");";
    memory->insert(query3,dataF);
    
    //cout<<"conjug: "<<basicName<<endl;
    //BlackBoss->verbs->insert(this);
    //BlackBoss->mots->insert(this);
}

Verbe::Verbe(Mot*m)
{
    def=m->def; //d�finition du mot
    isMasc=m->isMasc;//masculin
    isFem=m->isFem;
    isSing=m->isSing;
    isDenm=m->isDenm;//d�nombrable ou pas
    sing=m->sing;
    fem=m->fem;
    plur=m->plur;
    plurFem=m->plurFem;
    catMot=m->catMot; //cat�gorie du mot : adjectif, pr�nom, etc...
    //foncs=m->foncs; //m�thodes propres au mot
}

Verbe::Verbe(std::string infini, Conjugaison* pastConj, Conjugaison* impConj, Conjugaison* condConj, Conjugaison* subjConj, Conjugaison* presConj, Conjugaison* imperaConj, Conjugaison* futConj, std::string partic, std::string pPres, fstream* dataF, std::string fun, bool fIsEtat, bool fIsAux, int fgroupe)
{
    catMot=Verb;
    basicName=infini;
    ident=basicName;
    inf=infini;
    past=pastConj;
    imparf=impConj;
    cond=condConj;
    subj=subjConj;
    present=presConj;
    imper=imperaConj;
    futur=futConj;
    isEtat=fIsEtat;
    isAux=fIsAux;
    groupe=fgroupe;
    string query="insert into mot(basicName,type,fun) values(\""+basicName+"\",\""+catMot+"\",\""+fun+"\");";
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2,dataF);
    string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    char motGroupe[4];
    sprintf(motGroupe,"%d",groupe);
    string s_groupe=motGroupe;
    string s_Etat=(fIsEtat)? ("true"):("false");
    string s_Aux=(fIsEtat)? ("true"):("false");
    string query3="insert into verbe(verbe_id,basicName,inf,partPast,partPre,isAux,isEtat,numGr) values("+s_id+",\""+basicName+"\",\""+inf+"\",\""+partic+"\",\""+pPres+"\","+s_Aux+","+s_Etat+","+s_groupe+");";
    memory->insert(query3,dataF);
    
    participe=new Verbe(partic,infini,part,0,dataF,groupe,fun);
    partPresent=new Verbe(pPres,infini,partpre,0,dataF,groupe,fun);
    //BlackBoss->verbs->insert(this);
    //BlackBoss->mots->insert(this);
}

Verbe::Verbe(std::string infini,std::fstream *dataF, string fun, bool fIsEtat, bool fIsAux)
{
    catMot=Verb;
    basicName=infini;
    inf=infini;
    //foncs.push_back(fun);
    isEtat=fIsEtat;
    isAux=fIsAux;
    int size=inf.size();
    if(size>1)
    {   
        if(inf[size-1]=='r')
        {
            groupe=0;
            if(inf[size-2]=='e')
            {
                groupe=1;
            }
            else if(inf[size-2]=='i')
            {
                groupe=2;
            }

            string pp;
            int k;
            for(k=0;k<size-2;k++)
            {
                pp+=inf[k];   
            }
            string racine=pp.c_str();
            if(groupe==1) pp+="é";
            else if(groupe==2) pp+="i";
            participe=new Verbe(pp,inf,part,0,dataF,groupe,fun);
            string pPresent;
            if(groupe==1)
            {
                pPresent=racine+"ant";
            }
            else if(groupe==2)
            {
                pPresent=racine+"issant";
            }
            string query="insert into mot(basicName,type,fun) values(\""+basicName+"\",\""+catMot+"\",\""+fun+"\");";
            string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
            memory->insert(query,query2,dataF);
            string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
            int mot_id=memory->getIntAt(query4);
            char charMotId[4];
            sprintf(charMotId,"%d",mot_id);
            string s_id=charMotId;
            char motGroupe[4];
            sprintf(motGroupe,"%d",groupe);
            string s_groupe=motGroupe;

            string s_Etat=(fIsEtat)? ("true"):("false");
            string s_Aux=(fIsEtat)? ("true"):("false");
            string query3="insert into verbe(verbe_id,basicName,inf,partPast,partPre,isAux,isEtat,numGr) values("+s_id+",\""+basicName+"\",\""+inf+"\",\""+pp+"\",\""+pPresent+"\","+s_Aux+","+s_Etat+","+s_groupe+");";
            memory->insert(query3,dataF);
            
            past=new Conjugaison(inf,dataF,groupe,passe, fun);
            imparf=new Conjugaison(inf,dataF,groupe,imp,fun);
            cond=new Conjugaison(inf,dataF,groupe,condi,fun);
            subj=new Conjugaison(inf,dataF,groupe,subjo,fun);
            present=new Conjugaison(inf,dataF,groupe,pres,fun);
            futur=new Conjugaison(inf,dataF,groupe,fut,fun);
            imper=new Conjugaison(inf,dataF,groupe,imperat,fun);
            
            partPresent=new Verbe(pPresent,inf,partpre,0,dataF,groupe,fun);
            ident=basicName;
            //printf("new verb: '%s' du groupe %d\n", inf.c_str(),groupe);
            //BlackBoss->verbs->insert(this);
            //BlackBoss->mots->insert(this);
        }
    }
}

Verbe::Verbe(std::string fname)
{
    string query="select inf,isEtat,isAux,numGr,partPre,partPast,fun from verbe natural join mot where basicName=\""+fname+"\";";
    MYSQL_RES* result=NULL;
    MYSQL_ROW row;
    basicName=fname;
    inf=fname;
    catMot=Verb;
    result=memory->request(query);
    string ffun;
    if(result!=NULL)
    {
        if(row=mysql_fetch_row(result))
        {
            inf=row[0]? row[0]: "";
            ident=inf;
            basicName=inf;
            int nEtat;
            sscanf(row[1],"%d",&nEtat);
            sscanf(row[1],"%*[^\n]");
            isEtat=(nEtat==1);
            int nAux;
            sscanf(row[2],"%d",&nAux);
            sscanf(row[2],"%*[^\n]");
            isAux=(nAux==1);
            int nGr;
            sscanf(row[3],"%d",&nGr);
            sscanf(row[3],"%*[^\n]");
            groupe=nGr;
            string pPres=row[4]? row[4]:"";
            string partPast=row[5]? row[5]:"";
            ffun=row[6]? row[6]:"";
            partPresent=new Verbe(pPres,inf,partpre,0,groupe,ffun,isEtat,isAux);
            participe=new Verbe(partPast,inf,part,0,groupe,ffun,isEtat,isAux);
            Phrase* ph=new Phrase(ffun);
            int k;
            for(k=0;k<ph->size;k++)
            {
                Pf actualFun=Dfun->find(ph->exp[k]);
                foncs.push_back(actualFun);
            }
            delete ph;
        }
    }
    mysql_free_result(result);
    query="select je,tu,il,nous,vous,ils,temps from conjugaison where inf=\""+inf+"\";";
    result=memory->request(query);
    if(result!=NULL)
    {
        while(row=mysql_fetch_row(result))
        {
            string cje=row[0]? row[0]: "";
            string ctu=row[1]? row[1]: "";
            string cil=row[2]? row[2]: "";
            string cnous=row[3]? row[3]: "";
            string cvous=row[4]? row[4]: "";
            string cils=row[5]? row[5]: "";
            string ctemps=row[6]? row[6]: "";
            Conjugaison* conj= new Conjugaison(cje,ctu,cil,cnous,cvous,cils,inf,ctemps,ffun,isEtat,isAux,groupe);
            if(ctemps.compare(passe)==0)
            {
                past=conj;
            }
            if(ctemps.compare(imp)==0)
            {
                imparf=conj;
            }
            if(ctemps.compare(condi)==0)
            {
                cond=conj;
            }
            if(ctemps.compare(subjo)==0)
            {
                subj=conj;
            }
            if(ctemps.compare(imperat)==0)
            {
                imper=conj;
            }
            if(ctemps.compare(pres)==0)
            {
                present=conj;
            }
            if(ctemps.compare(fut)==0)
            {
                futur=conj;
            }
            
        }
    }
    mysql_free_result(result);
    Dmots->insert(this);
    Dverbs->insert(this);
}

Verbe::~Verbe()
{
    //dtor
}
