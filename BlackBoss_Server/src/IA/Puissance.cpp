/* 
 * File:   Puissance.cpp
 * Author: mblack
 * 
 * Created on 3 novembre 2015, 23:26
 */

#include <string>

#include "../../include/IA/Puissance.h"

using namespace std;
extern string stringType;
extern string intType;
extern string floatType;

template<typename T>
Puissance<T>::Puissance(Variable<T>* var, float d ) {
    vari=var;
    deg=d;
}

template<typename T>
Puissance<T>::Puissance(const Puissance& orig) {
}

template<typename T>
Puissance<T>* Puissance<T>::operator *(Puissance<T>* p)
{
    if((p->vari->type.compare(intType)==0 || p->vari->type.compare(intType)==0)(vari->type.compare(intType)==0 || vari->type.compare(floatType)==0))
    {
        if(p->vari->name.compare(vari->name)==0)
        {
            deg=deg+p->deg;
        }
    }
    return NULL;
}

template<typename T>
Puissance<T>::~Puissance() {
}

