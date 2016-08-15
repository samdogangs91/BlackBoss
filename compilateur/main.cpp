#include <iostream>
#include "Instruction.h"
#include "Database.h"
using namespace std;

Database* memory;
MYSQL data;

int main(int argc, char *argv[])
{
    memory=new Database();
    //Type* pre=new Type("determinant","","ins_id:int unsigned;name:string;argT:string;retourT:string;cont:string;","");
    //Type* ins= new Type("instruction");
    //ins->addAtt("isOp","bool");

    //Instruction* ins=new Instruction("string::=","string;string;","","If(arg.size()==2 && retour.size()==0; If(arg[0]->type->name==arg[1]->type->name; arg[0]->setValue(arg[1]);) ;);",15,"gauche");
    //char idTmp[4];
    //sprintf(idTmp,"%d",);
    //string id=getIdInstruction("string::=","string;string;","");
    Instruction* ins=new Instruction("--","","","",2,"droite",true);
    //ins->print();
    //ins->compile();
    //ins->deleteAtt("assoc");
    //Vargen* var=new Vargen("Mathieu","string","Mathieu");
    //var->print();
    //Type* tri=new Type("triplet");
    //tri->print();
    /*while(1)
    {
        string s;
        Instruction* inst= new Instruction(s);
        inst->compile();
    }*/
    return 0;
}
