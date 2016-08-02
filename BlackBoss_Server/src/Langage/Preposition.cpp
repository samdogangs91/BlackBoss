#include "../../include/Langage/Preposition.h"

Preposition::Preposition()
{
    //ctor
}

Preposition::Preposition(Mot*m)
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
    //idFoncs=m->idFoncs;
    //foncs=m->foncs; //m�thodes propres au mot
}

Preposition::~Preposition()
{
    //dtor
}
