/* 
 * File:   DatabaseUser.cpp
 * Author: mblack
 * 
 * Created on 3 septembre 2015, 05:48
 */

#include "../../include/Database/DatabaseUser.h"
using namespace std;

DatabaseUser::DatabaseUser() {
}

User* DatabaseUser::findUser(std::string fname)
{
    
}

void DatabaseUser::insert(User* user)
{
db.insert(pair<string,User*>(user->login,user));

}

DatabaseUser::DatabaseUser(const DatabaseUser& orig) {
}

DatabaseUser::~DatabaseUser() {
}

