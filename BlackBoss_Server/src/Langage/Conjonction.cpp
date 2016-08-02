#include "../../include/Langage/Conjonction.h"

Conjonction::Conjonction()
{
    //ctor
}

Conjonction::Conjonction(Mot*m)
{
    def=m->def; //d�finition du mot
    isMasc=m->isMasc;//masculin
    isFem=m->isFem;
    isSing=m->isSing;
    isDenm=m->isDenm;//d�nombrable ou pas
    sing=m->sing;
    fem=m->fem;
    plur=m->plur;
    plurFem=m->plurFem;
    catMot=m->catMot; //cat�gorie du mot : adjectif, pr�nom, etc...
    //foncs=m->foncs; //m�thodes propres au mot
}

Conjonction::~Conjonction()
{
    //dtor
}
