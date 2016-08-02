/* 
 * File:   DatabaseFun.h
 * Author: mblack
 *
 * Created on 25 octobre 2015, 13:43
 */

#ifndef DATABASEFUN_H
#define	DATABASEFUN_H

#include "../IA/comprehension.h"
#include <map>

typedef std::string (*Pf)(std::string);

class DatabaseFun {
public:
    DatabaseFun();
    std::map<std::string,Pf> db;
    void insert(std::string,Pf fun);
    Pf find(std::string fun);
    DatabaseFun(const DatabaseFun& orig);
    virtual ~DatabaseFun();
private:

};

#endif	/* DATABASEFUN_H */

