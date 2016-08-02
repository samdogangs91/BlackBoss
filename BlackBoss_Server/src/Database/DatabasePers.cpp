#include "../../include/Database/DatabasePers.h"

using namespace std;

DatabasePers::DatabasePers()
{
    //ctor
}

void DatabasePers::insert(Personne* pers)
{
db.insert(pair<string,Personne*>(pers->ident,pers));

}

Personne* DatabasePers::findPersonne(std::string fname)
{
    string ident=fname+" 0";
    map<string,Personne*>::iterator it;
    it=db.find(ident);
    if(it!=db.end())
    {
        return it->second;
    }
    else
    {
        return new Personne(fname);
    }
}

DatabasePers::~DatabasePers()
{
    //dtor
}
