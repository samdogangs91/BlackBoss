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
#include <cstdlib>
#include <cstdio>
#include "event.h"


class Vargen;
class Type;
class DbVar;

class Instruction {
public:
    Instruction(std::string id);
    Instruction(std::string _name,std::string _cont,std::vector<Vargen*> _arg,std::vector<DbVar*> _var);
    Instruction(std::string name,std::string argS, std::string retourS, std::string inst, std::vector<DbVar*> dbVar_, bool tmp=true, unsigned int prior=2, std::string _assoc="droite", bool _isOp=false);

    //Attributs
    std::string name;
    std::string id;
    unsigned int prior;
    std::string brut;
    bool tmp; //instruction temporaire ou non
    int index; //indice de l'appel de la fonction dans cont[0]
    bool isOp;
    bool ok;
    std::string cond; //condition
    std::vector<Instruction*> newInst; //nouvelles instructions créées dans l'instruction courante
    std::vector<Instruction*> instFilles;
    std::string assoc;//Associativité à droite ou à gauche
    std::vector<std::string> cont;
    std::vector<Type*> argT; //types ds variables d'entrée
    std::vector<Type*> retourT;//types des variables de sortie
    std::vector<Vargen*> arg; //variables d'entrée
    std::vector<Vargen*> retour; //variables de sortie
    std::vector<Vargen*> attClass; //Attribut de classe
    std::string type; //Instruction For, If, While, Set ou Program (suite d'instructions)
    std::vector<DbVar*> varDb; //variables des instructions mères et de l'instruction courante
    //DbVar* varTmp; //variables courantes
    //DbVar* varPar; //variables des instructions parentes

    void initType(); //initialisation des types de base

    //Methodes de manipulation des instructions
    void preCompile(std::string inst); //remplis l'attribut cont et determine le type de l'instruction
    bool interCompile(); //gestion des signaux return, error, continue et break
    void compile(); //compilation
    void determineType();
    void setValuesIn(std::vector<Vargen*> in);

    void print();
    void deleteInst();
    Instruction(const Instruction& orig);
    virtual ~Instruction();
private:

};

bool isNotSpe(char c);
std::string addNameSpace(std::string nameSpace,std::string nameInst);
std::string delNameSpace(std::string nameInst);


bool isOpe(char c);
std::string setArgInString(std::string brut, std::vector<Vargen*> arg);
std::string recopieString(std::string s);

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
Vargen* identity(std::string cont, std::vector<Vargen*> arg, std::vector<DbVar*> _varDb);
Vargen* Return(Instruction* inst);
std::vector<Vargen*> makeInstruction(std::string nameInst, std::vector<Vargen*> _arg, std::vector<Vargen*> exArg, std::vector<DbVar*> _varDb, Vargen* var=NULL);
Vargen* NewVar(Instruction* name, Instruction* type, std::vector<Vargen*> funArg, std::vector<DbVar*> _varDb, Instruction* arg=NULL, Instruction* tmp=NULL);
void deleteVar(std::string name, std::vector<DbVar*> _varDb);
Instruction* NewInst(Instruction* name,Instruction* argS, Instruction* retourS, Instruction* inst, std::vector<DbVar*> varDb_, Instruction* tmp=NULL, Instruction* prior=NULL, Instruction* _assoc=NULL, Instruction* _isOp=NULL);
void newType(std::string name, std::string desc, std::string cont, std::string meth);
void addAtt(std::string nameType, std::string nameAtt, std::string typeAtt);
void addMeth(Instruction* nameType, Instruction* nameMeth, std::vector<DbVar*> _varDb, Instruction* argTMeth=NULL);
void modifAtt(std::string nameType, std::string nameAtt, std::string typeAtt);
void deleteAtt(std::string nameType, std::string nameAtt);
void deleteMeth(std::string nameType, std::string nameMeth);
void setTmp(Instruction* instAtt, Instruction* tmpInst);
void Event(std::string src,std::string dest,std::string cont,std::string type);

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
Vargen* getAtt(Vargen* var, std::string att); //operator ->
Instruction* getMeth(Vargen* var, std::string name, std::string argT="",std::string retourT="");
Vargen* Cro(Instruction* inst, Instruction* num);
Vargen* Point(Instruction* instVar, std::string att);
void In(std::string stream, Instruction* inst);
void Out(std::string stream, Instruction* inst);
void NewConst(Instruction* nameType,std::vector<DbVar*> varDb_, Instruction* arg=NULL, Instruction* cont=NULL);


//opérateur pour les nombres
void Incr(Vargen* var); //operator ++
void Decr(Vargen * var); //operator --
Vargen* PlusU(Instruction* inst);
Vargen* MoinsU(Instruction* inst);
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



int getMaxTable(std::string nameType);

int indexInString(std::string name, std::string s, std::vector<int> index, bool isRight, unsigned int nbAtten=1);
std::vector<Instruction*> findInstruction(std::string name);
std::string getStringId(std::string id);
std::string getIdString(std::string cont);
std::string getIdInstruction(std::string name,std::string argT="", std::string retourT="");
void deliverEvent(Event* ev);


#endif	/* INSTRUCTION_H */

