/* 
 * File:   DatabaseUser.h
 * Author: mblack
 *
 * Created on 3 septembre 2015, 05:48
 */

#ifndef DATABASEUSER_H
#define	DATABASEUSER_H
#include <map>
#include "../Environnement/User.h"

class DatabaseUser {
public:
    DatabaseUser();
    std::map<std::string,User*> db;
    void insert(User* user);
    User* findUser(std::string fname);
    DatabaseUser(const DatabaseUser& orig);
    virtual ~DatabaseUser();
private:

};

#endif	/* DATABASEUSER_H */

