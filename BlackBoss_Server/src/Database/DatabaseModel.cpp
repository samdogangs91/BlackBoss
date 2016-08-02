/* 
 * File:   DatabaseModel.cpp
 * Author: mblack
 * 
 * Created on 29 octobre 2015, 02:29
 */

#include "../../include/Database/DatabaseModel.h"

using namespace std;

DatabaseModel::DatabaseModel() {
}

void DatabaseModel::insert(Modele* mod)
{
    db.insert(pair<string,Modele*>(mod->name,mod));
}

Modele* DatabaseModel::find(std::string fname)
{
    map<string,Modele*>::iterator it;
    it=db.find(fname);
    if(it==db.end())
    {
        return new Modele(fname);
    }
    else
    {
        return it->second;
    }
}

DatabaseModel::DatabaseModel(const DatabaseModel& orig) {
}

DatabaseModel::~DatabaseModel() {
}

