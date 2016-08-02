/* 
 * File:   Instruction.h
 * Author: mblack
 *
 * Created on 23 février 2016, 18:37
 */

#ifndef INSTRUCTION_H
#define	INSTRUCTION_H
#include "phrase.h"
#include "../Environnement/Variable.h"

class Instruction {
public:
    Instruction(std::string inst);
    std::vector<std::string> cont;
    void compile();
    std::string type; //Instruction For, If, While, Set ou Program (suite d'instructions)
    std::string var; //Variable principale de l'instruction
    bool makeBool(); //convertit en booléen une instruction 
    Instruction(const Instruction& orig);
    virtual ~Instruction();
private:

};

#endif	/* INSTRUCTION_H */

