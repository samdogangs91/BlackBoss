#ifndef DBVAR_H
#define DBVAR_H

#include <map>
#include "vargen.h"

class Vargen;

class DbVar
{
public:
    DbVar();
    DbVar(DbVar* orig, DbVar* orig2=NULL);
    void insert(Vargen* _var);
    void erase(std::string name);
    Vargen* find(std::string name, std::string type="");
    std::map<std::string,Vargen*> cont;
    ~DbVar();
};

#endif // DBVAR_H
