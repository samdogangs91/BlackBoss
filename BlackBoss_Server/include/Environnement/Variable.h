/* 
 * File:   Variable.h
 * Author: mblack
 *
 * Created on 29 octobre 2015, 02:32
 */

#ifndef VARIABLE_H
#define	VARIABLE_H
#include "../IA/comprehension.h"
#include <limits>

template<typename T>
class Variable {
public:
    std::string name;
    T val;
    bool isCste;
    std::string type;
    T domDef[2];
    
    //opérateurs sur les constantes
    Variable<T>* operator+(Variable<T>* var1);
    Variable<T>* operator-(Variable<T>* var1);
    Variable<T>* operator*(Variable<T>* var1);
    Variable<T>* operator/(Variable<T>* var1);
    Variable<T>* operator^(Variable<T>* var1);
    Variable<T>* operator=(Variable<T>* var1);
    
    Variable(std::string _name,T inf, T sup,std::string type="float"); //créer une variable inconnue
    Variable(std::string name,T val);//créer une constante
    //Variable(std::string name,std::string type="float");//Variable dont on ne connais pas le domaine de définition
    Variable(const Variable& orig);
    virtual ~Variable();
private:

};

#endif	/* VARIABLE_H */

