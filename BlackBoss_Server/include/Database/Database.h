#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <map>

#include "../IA/comprehension.h"



class Database
{
    public:
        bool ok;
        std::fstream* dataF;
        Database();
        unsigned int num_rows(std::string query);
        void insert(std::string query);
        void insert(std::string query,std::string query2);
        void insert(std::string query,std::string query2, std::fstream* dataF); //query: requete principale, query2: requete de teste
        void insert(std::string query, std::fstream* dataF); //ajoute à la base de données sans requête de test de présence
        int getIntAt(std::string query);//renvoie les atributs entiers
        MYSQL_RES* request(std::string query); //réalise la requête query dans la BDD et renvoie un pointeur vers le tableau de résultat
        virtual ~Database();
    protected:
    private:

};

#endif // DATABASE_H
