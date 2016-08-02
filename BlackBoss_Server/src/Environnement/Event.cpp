/* 
 * File:   Event.cpp
 * Author: mblack
 * 
 * Created on 22 avril 2016, 22:19
 */

#include "../../include/Environnement/Event.h"
#include "../../include/IA/other.h"

using namespace std;


string msgTalk="talk";
string msgThink="think";

Event::Event(string _dest, string _src, string _cont, string _type, unsigned int _sigNum) {
    dest=_dest;
    src=_src;
    cont=_cont;
    type=_type;
    sigNum=_sigNum;
    //cout<<"Event from "<<src<<endl;
    //deliverEvent(this);    
}

Event::Event(const Event& orig) {
}

Event::~Event() {
}

