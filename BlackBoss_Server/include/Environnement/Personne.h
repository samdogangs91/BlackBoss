#ifndef PERSONNE_H
#define PERSONNE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "../Environnement/Objet.h"
#include "../Langage/Mot.h"
#include "../Langage/Prenom.h"
#include "../Langage/NomPropre.h"

class NomPropre;

class Personne : public Objet
{
    public:
        Prenom* prenom;
        bool knowPrenom;
        NomPropre* nom;
        bool knowNom;
        Prenom* pseudo;
        bool knowPseudo;
        int age;
        bool knowAge;
        int taille;
        bool knowTaille;
        int date[3];//date de naissance, remplacer par une classe date plus tard
        bool knowDate;
        int poids;
        bool knowPoids;
        std::string ville;
        bool knowVille;
        bool sexe; // true->masculin false->feminin
        bool knowSexe;
        std::string hairColor;
        bool knowHair;
        std::string skinColor;
        bool knowSkin;
        std::string eyeColor;
        bool knowEye;
        std::string pays;
        bool knowPays;
        bool knowDefine;

        Personne* pere;
        Personne* mere;
        std::vector<Personne*> freres;
        bool hasBro;
        std::vector<Personne*> soeurs;
        bool hasSis;
        std::vector<Personne*> fils;
        bool hasSon;
        std::vector<Personne*> fille;
        bool hasDaughter;
        std::string identPre;

        
        Personne();
        Personne(std::string);
        Personne(Prenom* _pre, NomPropre* _nom, Prenom* _pse, bool _sexe, std::string _def,int age, int taille, int poids, std::string _skin, std::string _hair, std::string _eye,std::fstream *file);
        virtual ~Personne();
    protected:
    private:
};

#endif // PERSONNE_H
