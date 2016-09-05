#include <iostream>
#include "Instruction.h"
#include "Database.h"
using namespace std;

Database* memory;
MYSQL data;

int main(int argc, char *argv[])
{
    memory=new Database();
    vector<DbVar*> db;
    string s="NewVar(x,bool,false);NewVar(y,int,5);y*=5;If(!x&&(y==1)||(y!=1);(Return y;););";
    string s2="NewVar(z,string,cool);z=z+\" raoul\";Return z;";
    Instruction* inst=new Instruction("Inst",s2,db);
    inst->compile();
    unsigned int k;
    for(k=0;k<inst->retour.size();k++)
    {
        inst->retour[k]->print();
    }
    delete inst;    
    return 0;
}
