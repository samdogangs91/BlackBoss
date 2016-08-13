#include "dbvar.h"

using namespace std;

DbVar::DbVar()
{

}


void DbVar::insert(Vargen *var)
{
    cont.insert(pair<std::string,Vargen*>(var->name,var));
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
    else
    {
        return new Vargen(name,type);
    }
    return var;
}
