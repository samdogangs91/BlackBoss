#include "../../include/Langage/Pronom.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseMot.h"
//#include "../../include/IA/BlackIa.h"

extern std::string Pro;
//extern InBlackIa* InBoss;

extern MYSQL data;
extern Database* memory;
extern DatabaseMot* Dmots;

using namespace std;

Pronom::Pronom()
{
    foncs.push_back(rien);
}

Pronom::Pronom(Mot*m)
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
    //idFoncs=m->idFoncs;
    //foncs=m->foncs; //m�thodes propres au mot
}

Pronom::Pronom(std::string nom, Pf fun)
{
  basicName=nom;
  catMot=Pro;
  //idFoncs.push_back(0);
  foncs.push_back(fun);
  ident=nom;
  Dmots->insert(this);  
}

Pronom::Pronom(std::string nom, std::fstream* dataF, std::string fun)
{
  basicName=nom;
  catMot=Pro;
  ident=nom;
  //BlackBoss->mots->insert(this);
  string query="insert into mot(basicName,type,fun) values(\""+basicName+"\",\""+catMot+"\",\""+fun+"\");";
  string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
  memory->insert(query,query2,dataF);
}

Pronom::~Pronom()
{
    //dtor
}
