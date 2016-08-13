#include "attribut.h"
#include <iostream>

using namespace std;

Attribut::Attribut(std::string _name, std::string _type)
{
    name=_name;
    type=_type;
}

void Attribut::print()
{
    cout<<"name: "+name+", "+"type: "+type<<endl;
}
