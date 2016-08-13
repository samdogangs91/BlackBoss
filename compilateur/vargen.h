#ifndef VARGEN_H
#define VARGEN_H

#include "type.h"
#include "Instruction.h"
#include <vector>


class Instruction;
class Type;

void storeString(std::string _arg);

class Vargen
{
public:

    Vargen(std::string _name,std::string _type,std::string val="");
    //attributs de tout type de variable
    std::string name;
    bool ok;
    Type* type;
    std::vector<Vargen*> arg;

    //valeur des types de bases si la variable a un type de base
    std::string valStr;
    int valInt;
    bool valBool;
    char valChar;

    //Methode manipulant les variables
    void setValue(std::string _val);
    void setValueAtt(std::string nameAtt, std::string newVal);
    Vargen* getAtt(std::string nameAtt);
    void deleteVar();
    Instruction * getMeth(std::string id);

    void print();

    ~Vargen();
};

#endif // VARGEN_H
