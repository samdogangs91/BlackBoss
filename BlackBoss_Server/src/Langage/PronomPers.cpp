#include "../../include/Langage/PronomPers.h"
//#include "../../include/IA/BlackIa.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/DatabaseFun.h"

using namespace std;

extern string ProP;
//extern InBlackIa* InBoss;
extern MYSQL data;
extern Database* memory;
extern DatabaseMot* Dmots;

PronomPers::PronomPers()
{
    foncs.push_back(rien);
}

PronomPers::PronomPers(Mot*m)
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

PronomPers::PronomPers(std::string nom, Pf fun)
{
  basicName=nom;
  catMot=ProP;
  foncs.push_back(fun);
  ident=nom;
  Dmots->insert(this);
    
}

PronomPers::PronomPers(std::string nom, std::fstream* dataF, std::string fun)
{
  basicName=nom;
  catMot=ProP;
  //foncs.push_back(rien);
  ident=nom;
  //BlackBoss->mots->insert(this);
  string query="insert into mot(basicName,type,fun) values(\""+basicName+"\",\""+catMot+"\",\""+fun+"\");";
  //cout<<"requete sql: "<<query<< endl;
  string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
  //cout<<"requete sql: "<<query2<< endl;
  memory->insert(query,query2,dataF);
}

PronomPers::~PronomPers()
{
    //dtor
}
