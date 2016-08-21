#ifndef TYPE_H
#define TYPE_H


#ifdef WIN32
#include <window.h>
#include <winsock2.h>
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <mysql/mysql.h>

#include "attribut.h"
#include "Instruction.h"
#include <vector>

class Instruction;

std::string newString(std::string);

std::vector<Attribut*> makeAtt(std::string contS);

bool isBasic(std::string);


class Type
{
public:
    Type(std::string _name,bool estBasic=false);
    Type(std::string _name, std::string _desc,std::string _cont, std::string _meth);
    Type(std::string container, std::string content);

    //Attributs
    std::string name;
    std::vector<Attribut*> cont;
    std::vector<Instruction*> meth;
    std::string desc;

    //Methode de modification d'un type
    void addAtt(std::string _name,std::string _type);
    void deleteAtt(std::string _name);
    void modifAtt(std::string _name, std::string _type);
    void addMeth(std::string _name);
    void deleteMeth(std::string _name);
    void deleteType();

    bool isContainer();

    //GetMethode
    Instruction* getMeth(std::string id);

    void print();

    ~Type();
};

#endif // TYPE_H
