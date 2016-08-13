/* 
 * File:   Instruction.h
 * Author: mblack
 *
 * Created on 23 février 2016, 18:37
 */

#ifndef INSTRUCTION_H
#define	INSTRUCTION_H
#include "vargen.h"
#include "type.h"
#include <vector>
#include "dbvar.h"

class Vargen;
class Type;
class DbVar;

class Instruction {
public:
    Instruction(std::string id);
    Instruction(std::string _name,std::string _cont);
    Instruction(std::string name,std::string argS, std::string retourS, std::string inst, unsigned int prior=2, std::string _assoc="droite", bool _isOp=false, bool tmp=false);

    //Attributs
    std::string name;
    unsigned int prior;
    std::string brut;
    bool isOp;
    bool ok;
    std::string assoc;//Associativité à droite ou à gauche
    std::vector<std::string> cont;
    std::vector<Type*> argT; //types ds variables d'entrée
    std::vector<Type*> retourT;//types des variables de sortie
    std::vector<Vargen*> arg; //variables d'entrée
    std::vector<Vargen*> retour; //variables de sortie
    std::string type; //Instruction For, If, While, Set ou Program (suite d'instructions)
    DbVar* varTmp;

    //Methodes de manipulation des instructions
    void preCompile(std::string inst); //remplis l'attribut cont et determine le type de l'instruction
    void compile();

    void print();
    Instruction(const Instruction& orig);
    virtual ~Instruction();
private:

};

std::string parenth(std::string s); //paranthèsage de la string

//Instructions minimales:

void For(Instruction* deb, Instruction* stop, Instruction* incr,Instruction* boucle);
void While(Instruction* cond, Instruction* boucle);
void If(Instruction* cond, Instruction* boucle);
void Else(Instruction* cond, Instruction* boucle);
bool And(Instruction* inst1, Instruction* inst2);// operator &&
bool Or(Instruction* inst1, Instruction* inst2);// operator ||
bool Equal(Instruction* inst1, Instruction* inst2);//operator ==
void Set(Vargen* var, Instruction* inst);//operator =
Vargen* getAtt(Vargen* var, std::string att); //operator ->
Instruction* getMeth(Vargen* var, std::string idInst);
void Incr(Vargen* var); //operator ++
void Decr(Vargen * var); //operator --


std::vector<Instruction*> findInstruction(std::string name);
std::string getStringId(std::string id);
std::string getIdString(std::string cont);
std::string getIdInstruction(std::string name,std::string argT, std::string retourT="");


#endif	/* INSTRUCTION_H */

