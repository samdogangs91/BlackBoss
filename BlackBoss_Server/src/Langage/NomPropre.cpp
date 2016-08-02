#include "../../include/Langage/NomPropre.h"
//#include "../../include/IA/BlackIa.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/DatabaseFun.h"
//extern InBlackIa* InBoss; 
using namespace std;
extern Database* memory;
extern string NomP;

NomPropre::NomPropre(string fname)
{
    basicName=fname;
    ident=fname;
    cat=NomP;
    isDenm=false;
    isMasc=true;
    isFem=true;
    isSing=true;
    string query="insert into mot(basicName,type) values(\""+basicName+"\",\""+catMot+"\");";
    //cout<<"requete sql: "<<query<< endl;
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2);
    //InBoss->mots->insert(this);
}

NomPropre::NomPropre(std::string fname, fstream* dataF)
{
  basicName=fname;
  ident=fname;
    catMot=NomP;
    isDenm=false;
    isMasc=true;
    isFem=true;
    isSing=true;
    string query="insert into mot(basicName,type) values(\""+basicName+"\",\""+catMot+"\");";
    //cout<<"requete sql: "<<query<< endl;
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2,dataF);
    //BlackBoss->mots->insert(this);
    //mot=fname;
}

NomPropre::NomPropre(Mot*m)
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


NomPropre::~NomPropre()
{
    //dtor
}
