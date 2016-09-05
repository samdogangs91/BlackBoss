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
    Instruction(std::string _name,std::string _cont,std::vector<DbVar*> _var);
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
    std::vector<DbVar*> varDb; //variables des instructions mères et de l'instruction courante
    //DbVar* varTmp; //variables courantes
    //DbVar* varPar; //variables des instructions parentes

    //Methodes de manipulation des instructions
    void preCompile(std::string inst); //remplis l'attribut cont et determine le type de l'instruction
    bool interCompile(); //gestion des signaux return, error, continue et break
    void compile(); //compilation
    void setValuesIn(std::vector<Vargen*> in);

    void print();
    Instruction(const Instruction& orig);
    virtual ~Instruction();
private:

};

std::vector<std::string> getArg(std::string s, char sep, unsigned int numPar_=1);
bool isWellPar(std::string s, unsigned int _nbFun=0);
std::string parenth(std::string s); //paranthèsage de la string
std::string uselessPar(std::string s);
void Erreur(std::string err, std::vector<DbVar*> _varDb);

//Instructions minimales:
void For(Instruction* deb, Instruction* stop, Instruction* incr,Instruction* boucle);
void While(Instruction* cond, Instruction* boucle);
void If(Instruction* cond, Instruction* boucle);
void Else(Instruction* cond, Instruction* boucle);
Vargen* identity(std::string cont, std::vector<DbVar*> _varDb);
Vargen* Return(Instruction* inst);
void makeInstruction(std::string nameInst, std::vector<Instruction*> _arg, std::vector<DbVar*> _varDb);
Vargen* NewVar(std::string name, std::string type, std::vector<DbVar*> _varDb, std::string arg="", bool tmp=true);
void deleteVar(std::string name, std::vector<DbVar*> _varDb);
void NewInst(std::string name,std::string argS, std::string retourS, std::string inst, unsigned int prior=2, std::string _assoc="droite", bool _isOp=false, bool tmp=false);
void newType(std::string name, std::string desc, std::string cont, std::string meth);
void addAtt(std::string nameType, std::string nameAtt, std::string typeAtt);
void addMeth(std::string nameType, std::string nameMeth);
void modifAtt(std::string nameType, std::string nameAtt, std::string typeAtt);
void deleteAtt(std::string nameType, std::string nameAtt);
void deleteMeth(std::string nameType, std::string nameMeth);

//operator booleen
Vargen* And(Instruction* inst1, Instruction* inst2);// operator &&
Vargen* Or(Instruction* inst1, Instruction* inst2);// operator ||
Vargen* Equal(Instruction* inst1, Instruction* inst2);//operator ==
Vargen* Diff(Instruction* inst1, Instruction* inst2);//operator !=
Vargen* SupEqual(Instruction* inst1, Instruction* inst2);
Vargen* Sup(Instruction* inst1, Instruction* inst2);
Vargen* InfEqual(Instruction* inst1, Instruction* inst2);
Vargen* Inf(Instruction* inst1, Instruction* inst2);
Vargen* Neg(Instruction* inst);

//operateur généraux
void Set(Vargen* var, Instruction* inst);//operator =
Vargen* getAtt(Instruction* var, std::string att); //operator ->
Instruction* getMeth(Vargen* var, std::string name, std::string argT="",std::string retourT="");
Vargen* Cro(Instruction* inst, Instruction* num);
Vargen* Point(Instruction* instVar, std::string att);
void In(std::string stream, Instruction* inst);
void Out(std::string stream, Instruction* inst);


//opérateur pour les nombres
void Incr(Vargen* var); //operator ++
void Decr(Vargen * var); //operator --
Vargen* Plus(Instruction* inst1, Instruction* inst2);
Vargen* Moins(Instruction* inst1, Instruction* inst2);
Vargen* Mult(Instruction* inst1, Instruction* inst2);
Vargen* Div(Instruction* inst1, Instruction* inst2);
Vargen* Reste(Instruction* inst1, Instruction* inst2);
void PlusEqual(Instruction* inst1, Instruction* inst2);
void MoinsEqual(Instruction* inst1, Instruction* inst2);
void MultEqual(Instruction* inst1, Instruction* inst2);
void DivEqual(Instruction* inst1, Instruction* inst2);

//signaux liés à la compilation
void Continue(std::vector<DbVar *> _varDb);
void Break(std::vector<DbVar *> _varDb);
void Erreur(std::string err, std::vector<DbVar *> _varDb);
void Retour(Vargen* ret, std::vector<DbVar *> _varDb);




int indexInString(std::string name, std::string s, std::vector<int> index, unsigned int nbAtten=1);
std::vector<Instruction*> findInstruction(std::string name);
std::string getStringId(std::string id);
std::string getIdString(std::string cont);
std::string getIdInstruction(std::string name,std::string argT="", std::string retourT="");


#endif	/* INSTRUCTION_H */

