#ifndef DATABASEPERS_H
#define DATABASEPERS_H

#include <cstdlib>
#include <string>
#include <map>
#include "../Environnement/Personne.h"

class Personne;

class DatabasePers
{
    public:
        DatabasePers();
        void insert(Personne*);
        Personne* findPersonne(std::string fname);
        std::map<std::string,Personne*> db;
        virtual ~DatabasePers();
    protected:
    private:
};

#endif // DATABASEPERS_H
