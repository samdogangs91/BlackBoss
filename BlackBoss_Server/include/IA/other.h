#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "../Environnement/Event.h"

void BlackBossTalk(std::string);

std::string VousTalk();

bool modeApprentissage(std::string);

//void BlackBoss();

void deliverEvent(Event* ev);

Event* event(std::string _dest, std::string _src, std::string _cont, std::string _type, unsigned int _sigNum);

Event* eventS(std::string _dest, std::string _src, std::string _cont, std::string _type, unsigned int _sigNum);
