#ifndef ATTRIBUT_H
#define ATTRIBUT_H

#include <iostream>

class Attribut
{
public:
    Attribut(std::string _name,std::string _type);
    std::string type;
    std::string name;
    void print();
};

#endif // ATTRIBUT_H
