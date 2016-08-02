/* 
 * File:   SubExpr.h
 * Author: mblack
 *
 * Created on 4 novembre 2015, 20:32
 */

#ifndef SUBEXPR_H
#define	SUBEXPR_H

#include "Monome.h"

template <typename T>
class SubExpr //Expressions sans parenthèses 
{
public:
    SubExpr(std::string s); //s est une string contenant une expression développée
    std::vector<Monome<T>*> cont;
    SubExpr(const SubExpr& orig);
    virtual ~SubExpr();
private:

};

#endif	/* SUBEXPR_H */

