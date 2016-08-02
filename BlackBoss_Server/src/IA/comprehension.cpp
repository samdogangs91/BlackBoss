/*
 * File:   comprehension.cpp
 * Author: enrop
 *
 * Created on 10 juillet 2015, 09:16
 */

#include <cstdlib>

#include "../../include/IA/phrase.h"
#include "../../include/IA/comprehension.h"

using namespace std;

/*
 *
 */

//extern BlackIa* BlackBoss;

bool isSpace(char c)
{return c==' ';
}

 bool isApost(char c)
 {
     return c=='\'';
 }

 bool isPorV(char c)
 {
     return (c==',')||(c=='.')||(c=='!')||(c=='?')||(c==';');
 }

 bool isSeparateur(char c)
 {
     return isSpace(c)||isApost(c)||isPorV(c);
 }

 

 string rien(string s)
 {
     string rep="";
     return rep;
 }

