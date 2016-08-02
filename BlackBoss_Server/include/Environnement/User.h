/* 
 * File:   User.h
 * Author: mblack
 *
 * Created on 3 septembre 2015, 04:41
 */

#ifndef USER_H
#define	USER_H

#include <string>
#include <string.h>
#include "Personne.h"
#include "Event.h"
#include "Poco/Activity.h"
#include "Poco/Thread.h"


//class Personne;

class User: public Personne {
public:
    std::string userTalk;
    bool hasTalk;
    User();
    std::string login;
    std::queue<Event*> events;
    void hasEvents();
    char* mdp;
    bool admin;
    bool superAdmin;
    std::string Talk();
    void Behaviour();
    void start();
    void stop();
    bool hasEvent;
    User(std::string log, Personne* pers);
    User(MYSQL_ROW row, std::fstream* dataF,bool fAdmin=false, bool fSupAdmin=false);
    User(const User& orig);
    virtual ~User();
private:
    Poco::Activity<User> _activity;

};

#endif	/* USER_H */

