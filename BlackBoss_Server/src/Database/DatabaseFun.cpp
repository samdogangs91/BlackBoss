/* 
 * File:   DatabaseFun.cpp
 * Author: mblack
 * 
 * Created on 25 octobre 2015, 13:43
 */

#include "../../include/Database/DatabaseFun.h"

using namespace std;

DatabaseFun::DatabaseFun() {
}

Pf DatabaseFun::find(std::string fun)
{
    Pf res=rien;
    map<string,Pf>::iterator it=db.find(fun);
    if(it!=db.end())
    {
        res=it->second;
    }
    return res;
}

void DatabaseFun::insert(string s_fun,Pf fun)
{
    db.insert(pair<string,Pf>(s_fun,fun));
}

DatabaseFun::DatabaseFun(const DatabaseFun& orig) {
}

DatabaseFun::~DatabaseFun() {
}

