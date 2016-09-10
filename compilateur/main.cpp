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
    string s3="NewVar(z,string,cool);NewVar(k,int,1);k++;Return z[k+1]";
    string s4="cout<<\"Mathieu\"<<\" Black!\"<<endl";
    string s5="NewVar(x,int);cout<<\"Quel age as tu?\"<<endl;cin>>x;cout<<\"Vous avez \"<<x<<\" ans\"<<endl";
    string s6="NewInst(\"Plus\",\"x:int;y:int;\",\"int;\",\"Return x+y;\");Return Plus(2,2);";
    /*
     * faire une fonction qui étant donné une string remplace les arguments présents dans la string par ?[k] où k est le numéro d'argument dans l'instruction
     */
    vector<Vargen*> arg;
    Instruction* inst=new Instruction("Inst",s6,arg,db);
    inst->compile();
    unsigned int k;
    for(k=0;k<inst->retour.size();k++)
    {
        inst->retour[k]->print();
    }
    delete inst;    
    return 0;
}
