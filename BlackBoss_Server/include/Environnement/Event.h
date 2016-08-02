/* 
 * File:   Event.h
 * Author: mblack
 *
 * Created on 22 avril 2016, 22:19
 */

#ifndef EVENT_H
#define	EVENT_H

#include "../IA/comprehension.h"


class Event {
public:
    Event(std::string _dest, std::string _src, std::string _cont,std::string _type,unsigned int _sigNum);
    std::string dest;
    std::string src;
    std::string cont;
    std::string type;
    unsigned int sigNum;
    Event(const Event& orig);
    virtual ~Event();
private:

};

#endif	/* EVENT_H */

