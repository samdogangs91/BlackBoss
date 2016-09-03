#include <iostream>
#include "Instruction.h"
#include "Database.h"
using namespace std;

Database* memory;
MYSQL data;

int main(int argc, char *argv[])
{
    //cout<<"lol"<<endl;
    memory=new Database();
    //Type* pre=new Type("determinant","","ins_id:int unsigned;name:string;argT:string;retourT:string;cont:string;","");
    //Type* ins= new Type("instruction");
    //ins->addAtt("isOp","bool");

    //Instruction* ins=new Instruction("string::=","string;string;","","If(arg.size()==2 && retour.size()==0; If(arg[0]->type->name==arg[1]->type->name; arg[0]->setValue(arg[1]);) ;);",15,"gauche");
    //char idTmp[4];
    //sprintf(idTmp,"%d",);
    //string id=getIdInstruction("==","","");
    //Instruction* ins=new Instruction("<","","","",8,"droite",true);
    //string s="a+b+c+d;";
    //string ret=parenth(s);
    //cout<<ret<<endl;

    vector<DbVar*> db;
    string s="If(true;(NewVar(x,int,2);Return x+1+1;););";
    Instruction* inst=new Instruction("lol",s,db);
    //inst->print();
    inst->compile();
    unsigned int k;
    for(k=0;k<inst->retour.size();k++)
    {
        inst->retour[k]->print();
    }
    //inst->print();
    //cout<<"lol"<<endl;
    //string list="list";
    //string intS="int";

    //Type* typ=new Type(list,intS);
    //string s="s=\"a+b\"+b";
    //string s1="((a+b))";
    //string ret;
    //ret=parenth(s);
    //ret=uselessPar(s1);
    //cout<<"après parenth"<<endl;
    //cout<<"retour: "<<ret<<endl;
    //cout<<"isWellParent= "<<isWellPar(ret)<<endl;
    //Instruction* ins2=new Instruction("%","","","",5,"droite",true);
    //Instruction* ins3=new Instruction(">","","","",8,"droite",true);
    //Instruction* ins=new Instruction(id);
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
