#ifndef OBJET_H
#define OBJET_H

#include <iostream>
#include <string>


class Objet
{
    public:
        Objet();
        int idData; //identifiant base de données
        std::string basicName;
        Objet(std::string);
        std::string def;
        std::string ident;
        std::string cat;

        virtual void write(std::fstream *file){};
        virtual int read(std::fstream *file, int *pos){};
        virtual ~Objet();
    protected:
    private:
};

#endif // OBJET_H
