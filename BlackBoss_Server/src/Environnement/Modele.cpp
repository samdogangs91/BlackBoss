/* 
 * File:   Modele.cpp
 * Author: mblack
 * 
 * Created on 29 octobre 2015, 01:46
 */

#include <Poco/Activity.h>

#include "../../include/Environnement/Modele.h"

using namespace std;

Modele::Modele(string fname) : _activity(this,&Modele::game){
    name=fname;
    //recherche dans le base de données de la formule et du game
}

void Modele::start()
{
    _activity.start();
}

void Modele::stop()
{
    _activity.stop();
    _activity.wait();
}

void Modele::game()
{
    funGame(in,intern,&out);
}

Modele::~Modele() {
}

