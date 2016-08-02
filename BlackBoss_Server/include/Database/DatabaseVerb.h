/* 
 * File:   DatabaseVerb.h
 * Author: mblack
 *
 * Created on 3 septembre 2015, 08:14
 */

#ifndef DATABASEVERB_H
#define	DATABASEVERB_H
#include <map>
#include "../Langage/Verbe.h"
class Verbe;

class DatabaseVerb {
public:
    DatabaseVerb();
    void insert(Verbe*);
    Verbe* findVerb(std::string fname);
    std::map<std::string,Verbe*> db;
    DatabaseVerb(const DatabaseVerb& orig);
    virtual ~DatabaseVerb();
private:

};

#endif	/* DATABASEVERB_H */

