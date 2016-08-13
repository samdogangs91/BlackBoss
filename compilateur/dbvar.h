#ifndef DBVAR_H
#define DBVAR_H

#include <map>
#include "vargen.h"

class Vargen;

class DbVar
{
public:
    DbVar();
    void insert(Vargen* _var);
    Vargen* find(std::string name, std::string type="");
    std::map<std::string,Vargen*> cont;
};

#endif // DBVAR_H
