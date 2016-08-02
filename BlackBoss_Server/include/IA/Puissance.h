/* 
 * File:   Puissance.h
 * Author: mblack
 *
 * Created on 3 novembre 2015, 23:26
 */

#ifndef PUISSANCE_H
#define	PUISSANCE_H

#include "../Environnement/Variable.h"
template <typename T>
class Puissance {
public:
    Variable<T>* vari;
    float deg;
    Puissance<T>* operator*(Puissance<T>* p);
    Puissance(Variable<T>* var,float d=1);
    Puissance(const Puissance& orig);
    virtual ~Puissance();
private:

};

#endif	/* PUISSANCE_H */

