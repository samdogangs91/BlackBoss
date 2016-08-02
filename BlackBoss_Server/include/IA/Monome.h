/* 
 * File:   Monome.h
 * Author: mblack
 *
 * Created on 4 novembre 2015, 00:50
 */

#ifndef MONOME_H
#define	MONOME_H

#include "Puissance.h"
template<typename T>
class Monome //représente les groupes de la forme a(X^n)(Y^p)...
{
public:
    Monome(float _coeff,std::vector<Puissance<T>*>);
    Monome(std::string s); //s est une string représentant un monome
    std::vector<Puissance<T>*> ordre(std::vector<Puissance<T>*>); //ordonne les variables du monome
    float coeff;
    std::vector<Puissance<T>*> cont;
    Monome<T>* operator+(Monome<T>* m);
    Monome<T>* operator-(Monome<T>* m);
    Monome<T>* operator*(Monome<T>* m);
    Monome(const Monome& orig);
    virtual ~Monome();
private:

};

#endif	/* MONOME_H */

