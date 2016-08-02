#ifndef DATABASEMOT_H
#define DATABASEMOT_H

#include <cstdlib>
#include <string>
#include <map>

#include "../Langage/Mot.h"

class Mot;

class DatabaseMot
{
    public:
        DatabaseMot();
        void insert(Mot*);
        Mot* findMot(std::string);
        std::map<std::string,Mot*> db;
        virtual ~DatabaseMot();
    protected:
    private:
};

#endif // DATABASEMOT_H
