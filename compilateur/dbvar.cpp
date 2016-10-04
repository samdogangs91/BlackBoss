#include "dbvar.h"

using namespace std;

DbVar::DbVar()
{

}


DbVar::DbVar(DbVar* orig, DbVar* orig2)
{
    map<string,Vargen*>::iterator it;
    for(it=orig->cont.begin();it!=orig->cont.end();it++)
    {
        insert(it->second);
    }
    if(orig2!=NULL)
    {
        for(it=orig2->cont.begin();it!=orig2->cont.end();it++)
        {
            insert(it->second);
        }
    }
}

/*
 * efface de la base de variable sans la supprimer de la bdd
 */
void DbVar::remove(Vargen *_var)
{
    map<string,Vargen*>::iterator it=cont.find(_var->name);
    if(it!=cont.end())
    {
        Vargen* var=it->second;
        delete var;
        cont.erase(it);
    }
}


void DbVar::insert(Vargen *var)
{
    if(var!=NULL)
    {
        cont.insert(pair<std::string,Vargen*>(var->name,var));
    }
}

Vargen* DbVar::find(std::string name, std::string type)
{
    Vargen* var=NULL;
    map<string,Vargen*>::iterator it;
    it=cont.find(name);
    if(it!=cont.end())
    {
        return it->second;
    }
    else if(type.compare("")!=0)
    {
        var=new Vargen(name,type);
        insert(var);
    }
    return var;
}

/*
 * efface de la base de variable en la supprimant de la bdd
 */
void DbVar::erase(string name)
{
    map<string,Vargen*>::iterator it=cont.find(name);
    if(it!=cont.end())
    {
        Vargen* var=it->second;
        var->deleteVar();
        delete var;
        cont.erase(it);
    }
}

DbVar::~DbVar()
{
    map<string,Vargen*>::iterator it;
    for(it=cont.begin();it!=cont.end();it++)
    {
        Vargen* var=it->second;
        //cout<<"delete de "<<var->name<<endl;
        if(var->tmp)
        {
            var->deleteVar();
        }
        cont.erase(it);
    }
}
