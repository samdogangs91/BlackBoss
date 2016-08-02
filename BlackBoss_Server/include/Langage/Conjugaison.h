/* 
 * File:   Conjugaison.h
 * Author: mblack
 *
 * Created on 3 septembre 2015, 04:16
 */

#ifndef CONJUGAISON_H
#define	CONJUGAISON_H

#include <string>
#include "Verbe.h"

class Verbe;

class Conjugaison {
public:
    Verbe* je;
    Verbe* tu;
    Verbe* il;
    Verbe* nous;
    Verbe* vous;
    Verbe* ils;

    Conjugaison();
    Conjugaison(std::string inf, std::fstream* dataF, int groupe, std::string temps, std::string fun="rien", bool fIsEtat =false, bool fIsAux= false );
    Conjugaison(std::string cje,std::string ctu,std::string cil,std::string cnous,std::string cvous,std::string cils, std::string inf, std::string temps, std::fstream* dataF,std::string fun="rien", bool fIsEtat=false, bool fIsAux=false, int groupe=3);
    Conjugaison(std::string cje,std::string ctu,std::string cil,std::string cnous,std::string cvous,std::string cils, std::string inf, std::string temps, std::string fun="rien", bool fIsEtat=false, bool fIsAux=false, int groupe=3);    
    Conjugaison(const Conjugaison& orig);
    virtual ~Conjugaison();
private:

};

#endif	/* CONJUGAISON_H */

