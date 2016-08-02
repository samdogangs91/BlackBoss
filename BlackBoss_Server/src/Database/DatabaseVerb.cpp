/* 
 * File:   DatabaseVerb.cpp
 * Author: mblack
 * 
 * Created on 3 septembre 2015, 08:14
 */

#include "../../include/Database/DatabaseVerb.h"

using namespace std;

DatabaseVerb::DatabaseVerb() {
}

Verbe* DatabaseVerb::findVerb(std::string fname)
{
    map<string,Verbe*>::iterator it;
    it=db.find(fname);
    if(it!=db.end())
    {
        return it->second;
    }
    else 
    {   
        Verbe* infini=new Verbe(fname);
        map<string,Verbe*>::iterator it;
        it=db.find(fname);
        if(it!=db.end())
        {
            return it->second;
        }
        else return infini;
    }
}

void DatabaseVerb::insert(Verbe* verb)
{
    db.insert(pair<string,Verbe*>(verb->ident,verb));
}

DatabaseVerb::DatabaseVerb(const DatabaseVerb& orig) {
}

DatabaseVerb::~DatabaseVerb() {
}

