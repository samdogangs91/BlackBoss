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
    string s6="NewInst(\"Reste\",\"x:int;y:int;\",\"int;\",\"Return x%y;\");Return Reste(16,4);";
    string s7="NewVar(x,int);NewVar(y,int);x=10;y=2;Return x/y;";
    string s8="NewVar(k,int);For(k=0;k<5;k++;(cout<<\"lol\"<<endl;););";
    string s9="NewInst(\"Sum\",\"x:int;y:int;z:int;\",\"int;\",\"Return x+y+z;\");Return Sum(1,2,3);";
    string s10="NewVar(\"x\",\"list_int\",\"1;2;3;4;5;\");Return x[2]+x[3];";

    vector<Vargen*> arg;
    cout<<s10<<endl;
    /*string cont="gouts:list_gout;";
    string desc="";
    string name="personality";
    Type* type=new Type(name,desc,cont,";",false);
    type->print();
    delete type;*/
    while(1)
    {
        cout<<"Donnez une instruction à compiler"<<endl;
        string s;
        getline(cin,s);
        Instruction* inst=new Instruction("Inst",s,arg,db);
        inst->compile();
        unsigned int k;
        for(k=0;k<inst->retour.size();k++)
        {
            inst->retour[k]->print();
        }
        delete inst;
        cout<<"_____________________________________________"<<endl;
    }
    return 0;
}
