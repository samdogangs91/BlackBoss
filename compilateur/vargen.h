#ifndef VARGEN_H
#define VARGEN_H

#include "type.h"
#include "Instruction.h"
#include <vector>


class Instruction;
class Type;

void storeString(std::string _arg);
std::string insertIntoList(std::string _arg);

class Vargen
{
public:

    Vargen(std::string _name,std::string _type,std::string val="", bool tmp=true);
    Vargen(Vargen* var);

    //attributs de tout type de variable
    std::string name;
    bool ok;
    bool tmp;
    Type* type;
    std::vector<Vargen*> arg;

    //valeur des types de bases si la variable a un type de base
    std::string valStr;
    int valInt;
    bool valBool;
    char valChar;
    double valReal;

    //Methode manipulant les variables
    void setVal(Vargen* val);
    void setValue(std::string _val);
    void setValueAtt(std::string nameAtt, std::string newVal);
    Vargen* getAtt(std::string nameAtt);
    void deleteVar();

    Instruction * getMeth(std::string id);
    Instruction* getMeth(std::string name,std::string argT="", std::string retourT="");

    void print();

    ~Vargen();
};

std::vector<Vargen*> makeArgVar(std::string _arg);

#endif // VARGEN_H
