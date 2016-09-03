#include "Instruction.h"
#include <fstream>

using namespace std;

extern string intType;
extern string charType;
extern string boolType;
extern string floatType;
extern string stringType;
extern string signalType;

string Continue_="Continue";
string Break_="Break";
string Erreur_="Erreur";
string Retour_="Retour";


Vargen* identity(std::string cont, vector<DbVar*> varDb)
{
   //case int
   int res;
   int nb;
   Vargen* ret=NULL;
   char* contC=(char*)cont.c_str();
   res=sscanf(contC,"%d",&nb);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
       ret=new Vargen(cont,intType,cont);
       int size=varDb.size();
       if(size>1)
       {
           varDb[size-2]->insert(ret);
       }
       return ret;
   }

   //case char
   char c;
   res=sscanf(contC,"'%c'",&c);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,charType,cont);
      int size=varDb.size();
      if(size>1)
      {
          varDb[size-2]->insert(ret);
      }
      return ret;
   }

   //case float
   float val;
   res=sscanf(contC,"%f",&val);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,floatType,cont);
      int size=varDb.size();
      if(size>1)
      {
          varDb[size-2]->insert(ret);
      }
      return ret;
   }

   //case bool
   if(cont.compare("true")==0 || cont.compare("false")==0)
   {
      ret=new Vargen(cont,boolType,cont);
      int size=varDb.size();
      if(size>1)
      {
          varDb[size-2]->insert(ret);
      }
      return ret;
   }

   //case string
   char* contStr;
   res=sscanf(contC,"\"%s\"",contStr);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,stringType,cont);
      int size=varDb.size();
      if(size>1)
      {
          varDb[size-2]->insert(ret);
      }
      return ret;
   }

   //case Vargen
   unsigned int k;
   for(k=0;k<varDb.size();k++)
   {
       ret=varDb[k]->find(cont);
       if(ret!=NULL)
       {
           break;
       }
   }

   return ret;
}


Vargen* Return(Instruction* inst)
{
    Vargen* ret=NULL;
    inst->compile();
    if(inst->retour.size()==1)
    {
        ret=new Vargen(inst->retour[0]);
    }
    int size=inst->varDb.size();
    if(size>1)
    {
        inst->varDb[size-2]->insert(ret);
    }
    delete inst;
    return ret;
}


Vargen* NewVar(std::string name, std::string type, vector<DbVar*> _varDb, std::string arg, bool tmp)
{
    Vargen* ret=NULL;
    unsigned int k;
    for(k=0;k<_varDb.size();k++)
    {
        ret=_varDb[k]->find(name);
        if(ret!=NULL)
        {
            break;
        }
    }
    if(ret==NULL)
    {
        ret=new Vargen(name,type,arg,tmp);
        int size=_varDb.size();
        if(size>1)
        {
            _varDb[size-2]->insert(ret);
        }
    }
    else
    {
        cout<<"Attention: La variable "<<name<<" existe déja!"<<endl;
    }

    return ret;
}


void deleteVar(string name, vector<DbVar*> varDb)
{
    unsigned int k;
    for(k=0;k<varDb.size();k++)
    {
        varDb[k]->erase(name);
    }
}


void NewInst(std::string name,std::string argS, std::string retourS, std::string _inst, unsigned int prior, std::string _assoc, bool _isOp, bool tmp)
{
    Instruction* inst=new Instruction(name,argS,retourS,_inst,prior,_assoc,_isOp,tmp);
    delete inst;
}


void newType(std::string name, std::string desc, std::string cont, std::string meth)
{
    Type* type=new Type(name,desc,cont,meth);
    delete type;
}


void addAtt(std::string nameType, std::string nameAtt, std::string typeAtt)
{
    Type* type=new Type(nameType);
    type->addAtt(nameAtt,typeAtt);
    delete type;
}


void addMeth(std::string nameType, std::string nameMeth)
{
    Type* type= new Type(nameType);
    type->addMeth(nameMeth);
    delete type;
}


void modifAtt(std::string nameType, std::string nameAtt, std::string typeAtt)
{
    Type* type= new Type(nameType);
    type->modifAtt(nameAtt,typeAtt);
    delete type;
}


void deleteAtt(std::string nameType, std::string nameAtt)
{
    Type* type= new Type(nameType);
    type->deleteAtt(nameAtt);
    delete type;
}


void deleteMeth(std::string nameType, std::string nameMeth)
{
    Type* type= new Type(nameType);
    type->deleteMeth(nameMeth);
    delete type;
}


//operator booleen
Vargen* And(Instruction* inst1, Instruction* inst2)// operator &&
{
    inst1->compile();
    inst2->compile();
    Vargen* ret=NULL;
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(boolType)==0)
        {
            bool b=(var1->valBool)&&(var2->valBool);
            string bStr=(b? "true" : "false");
            ret=new Vargen(var1->name+"&&"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(boolType)!=0)
        {
            cout<<"Operande gauche de && n'est pas de type bool"<<endl;
        }
        else
        {
            cout<<"Operande droite de && n'est pas de type bool"<<endl;
        }
    }
    else
    {
        cout<<"Erreur dans l'operateur &&"<<endl;
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Or(Instruction* inst1, Instruction* inst2)// operator ||
{
    inst1->compile();
    inst2->compile();
    Vargen* ret=NULL;
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(boolType)==0)
        {
            bool b=(var1->valBool)||(var2->valBool);
            string bStr=(b? "true" : "false");
            ret=new Vargen(var1->name+"||"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(boolType)!=0)
        {
            cout<<"Operande gauche de || n'est pas de type bool"<<endl;
        }
        else
        {
            cout<<"Operande droite de || n'est pas de type bool"<<endl;
        }
    }
    else
    {
        cout<<"Erreur dans l'operateur ||"<<endl;
    }
    delete inst1;
    delete inst2;
    return ret;
}


bool Equal2(Vargen* var1, Vargen* var2)
{
    bool ret=true;
    if(var1==NULL)
    {
        cout<<"Erreur: var1 est NULL"<<endl;
    }
    else if(var2==NULL)
    {
        cout<<"Erreur: var2 est NULL"<<endl;
    }
    else if(var1->type->name.compare(var2->type->name)==0)
    {
        if(var1->arg.size()>0 && var2->arg.size()>0)
        {
            if(var1->arg.size()==var2->arg.size())
            {
                unsigned int k;
                for(k=0;k<var1->arg.size();k++)
                {
                    ret=ret&&(Equal2(var1->arg[k],var2->arg[k]));
                    if(!ret)
                    {
                        break;
                    }
                }
            }
            else
            {
                ret=false;
            }
        }
        else if(var1->arg.size()==0 && var2->arg.size()==0)
        {
            if(var1->type->name.compare(intType))
            {
                ret=(var1->valInt==var2->valInt);
            }
            else if(var1->type->name.compare(floatType))
            {
                ret=(var1->valFloat==var2->valFloat);
            }
            else if(var1->type->name.compare(charType))
            {
                ret=(var1->valChar==var2->valChar);
            }
            else if(var1->type->name.compare(boolType))
            {
                ret=(var1->valBool==var2->valBool);
            }
            else if(var1->type->name.compare(stringType))
            {
                ret=(var1->valStr.compare(var2->valStr)==0);
            }
        }
        else
        {
            ret=false;
        }
    }
    else
    {
        ret=false;
    }
    return ret;
}


Vargen* Equal(Instruction* inst1, Instruction* inst2)//operator ==
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else
        {
            bool res=Equal2(var1,var2);
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"=="+var2->name,boolType,bStr);
        }
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Diff(Instruction* inst1, Instruction* inst2)//operator !=
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else
        {
            bool res=!Equal2(var1,var2);
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"!="+var2->name,boolType,bStr);
        }
    }
    delete inst1;
    delete inst2;
    return ret;
}



Vargen* SupEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1==NULL)
        {
           cout<<"Erreur: var est NULL"<<endl;
        }
        else if(var2==NULL)
        {
           cout<<"Erreur: var est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            res=((var1->valInt)>=(var2->valInt));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">="+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            res=((var1->valFloat)>=(var2->valFloat));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">="+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(charType)==0 && var2->type->name.compare(charType)==0)
        {
            res=((var1->valChar)>=(var2->valChar));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">="+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(stringType)==0)
        {
            res=(var1->valStr.compare(var2->valStr)>=0);
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">="+var2->name,boolType,bStr);
        }

    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Sup(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            res=((var1->valInt)>(var2->valInt));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            res=((var1->valFloat)>(var2->valFloat));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(charType)==0 && var2->type->name.compare(charType)==0)
        {
            res=((var1->valChar)>(var2->valChar));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(stringType)==0)
        {
            res=(var1->valStr.compare(var2->valStr)>0);
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+">"+var2->name,boolType,bStr);
        }

    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* InfEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            res=((var1->valInt)<=(var2->valInt));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<="+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            res=((var1->valFloat)<=(var2->valFloat));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<="+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(charType)==0 && var2->type->name.compare(charType)==0)
        {
            res=((var1->valChar)<=(var2->valChar));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<="+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(stringType)==0)
        {
            res=(var1->valStr.compare(var2->valStr)<=0);
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<="+var2->name,boolType,bStr);
        }

    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Inf(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            res=((var1->valInt)<(var2->valInt));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            res=((var1->valFloat)<(var2->valFloat));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(charType)==0 && var2->type->name.compare(charType)==0)
        {
            res=((var1->valChar)<(var2->valChar));
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(stringType)==0)
        {
            res=(var1->valStr.compare(var2->valStr)<0);
            string bStr=(res? "true" : "false");
            ret=new Vargen(var1->name+"<"+var2->name,boolType,bStr);
        }

    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Neg(Instruction* inst)
{
    Vargen* ret=NULL;
    inst->compile();
    if(inst->retour.size()==1)
    {
        Vargen* var=inst->retour[0];
        if(var==NULL)
        {
            cout<<"Erreur: var est NULL"<<endl;
        }
        else if(var->type->name.compare(boolType)==0)
        {
           bool res=!var->valBool;
           string bStr=(res? "true" : "false");
           ret=new Vargen("!"+var->name,boolType,bStr);
        }
    }
    delete inst;
    return ret;
}


//operateur généraux
void Set(Vargen* var, Instruction* inst)//operator =
{
    inst->compile();
    if(inst->retour.size()==1)
    {
        Vargen* varInst=inst->retour[0];
        if(var==NULL)
        {
            cout<<"Erreur: var est NULL"<<endl;
        }
        else if(varInst==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else
        {
            var->setVal(varInst);
        }
    }
    delete inst;
}

void Set2(Vargen* var1, Vargen* var2)
{
    if(var1!=NULL && var2!=NULL)
    {
        var1->setVal(var2);
    }
    else if(var1==NULL)
    {
       cout<<"Erreur: var1 est NULL"<<endl;
    }
    else
    {
       cout<<"Erreur: var2 est NULL"<<endl;
    }
}


Vargen* getAtt(Instruction* instVar, std::string att) //operator ->
{
    Vargen* ret=NULL;
    if(instVar->retour.size()==1)
    {
        Vargen* var=instVar->retour[0];
        if(var==NULL)
        {
            cout<<"Erreur: var est NULL"<<endl;
        }
        else
        {
            ret=var->getAtt(att);
        }
    }
    delete instVar;
    return ret;
}


Instruction* getMeth(Vargen* var, std::string name, std::string argT,std::string retourT)
{
    if(var==NULL)
    {
        cout<<"Erreur: var est NULL"<<endl;
    }
    else
    {
        return var->getMeth(name,argT,retourT);
    }
    return NULL;
}


Vargen* Cro(Instruction* inst, Instruction* num)
{
    Vargen* ret=NULL;
    inst->compile();
    num->compile();
    if(inst->retour.size()==1 && num->retour.size()==1)
    {
        Vargen* var1=inst->retour[0];
        Vargen* var2=num->retour[0];
        if(var1==NULL)
        {
            cout<<"Erreur: var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
            cout<<"Erreur: var2 est NULL"<<endl;
        }
        else if(var2->type->name.compare(intType)==0)
        {
            int k=var2->valInt;
            char kStr[4];
            string kS;
            sprintf(kStr,"%d",k);
            kS=kStr;
            string name="Cro_"+kS+"_"+var1->name;
            if(var1->type->name.compare(stringType)==0)
            {                
                if(k<var1->valStr.size())
                {
                    char c=var1->valStr[k];
                    char* ch;
                    sprintf(ch,"%c",c);
                    string str=ch;
                    ret=new Vargen(name,charType,str);
                }
                else
                {
                    cout<<"Erreur depassement de taille de la string (k="<<k<<", string='"<<var1->valStr<<"'"<<endl;
                }
            }
            else if(var1->type->isContainer())
            {
                if(k>=0 && k<var1->arg.size())
                {
                    ret=var1->arg[k];
                }
                else
                {
                    cout<<"Erreur depassement de taille du container"<<endl;
                }
            }
        }
    }
    return ret;
}


Vargen* Point(Instruction* instVar, string att)
{
    Vargen* ret=NULL;
    instVar->compile();
    if(instVar->retour.size()==1)
    {
        Vargen* var=instVar->retour[0];
        if(var==NULL)
        {
            cout<<"Erreur: var est NULL"<<endl;
        }
        else
        {
            ret=var->getAtt(att);
        }
    }
    delete instVar;
    return ret;
}


void In(std::string stream, Instruction* inst)
{
    inst->compile();
    if(stream.compare("cin")==0)
    {
        if(inst->retour.size()==1)
        {
            Vargen* var=inst->retour[0];
            if(var==NULL)
            {
                cout<<"Erreur: var est NULL"<<endl;
            }
            else
            {
                string s;
                getline(cin,s);
                if(var->type->name.compare(intType)==0)
                {
                    int val;
                    char*sTmp=(char*)s.c_str();
                    int res=sscanf(sTmp,"%d",&val);
                    sscanf(sTmp,"%*[^\n]");
                    if(res==1)
                    {
                       var->valInt=val;
                    }
                }
                else if(var->type->name.compare(charType)==0)
                {
                    char val;
                    char*sTmp=(char*)s.c_str();
                    int res=sscanf(sTmp,"%c",&val);
                    sscanf(sTmp,"%*[^\n]");
                    if(res==1)
                    {
                       var->valChar=val;
                    }
                }
                else if(var->type->name.compare(floatType)==0)
                {
                    float val;
                    char*sTmp=(char*)s.c_str();
                    int res=sscanf(sTmp,"%f",&val);
                    sscanf(sTmp,"%*[^\n]");
                    if(res==1)
                    {
                       var->valFloat=val;
                    }
                }
                else if(var->type->name.compare(stringType)==0)
                {
                    var->valStr=s;
                }
                else if(var->type->name.compare(boolType)==0)
                {
                    if(s.compare("true")==0)
                    {
                        var->valBool=true;
                    }
                    else if(s.compare("false")==0)
                    {
                        var->valBool=false;
                    }
                }
            }
        }
    }
    else
    {
        ifstream file(stream.c_str(),ios::in);
        if(file)
        {
            if(inst->retour.size()==1)
            {
                Vargen* var=inst->retour[0];
                if(var==NULL)
                {
                    cout<<"Erreur: var est NULL"<<endl;
                }
                else
                {
                    string s;
                    getline(file,s);
                    if(var->type->name.compare(intType)==0)
                    {
                        int val;
                        char*sTmp=(char*)s.c_str();
                        int res=sscanf(sTmp,"%d",&val);
                        sscanf(sTmp,"%*[^\n]");
                        if(res==1)
                        {
                           var->valInt=val;
                        }
                    }
                    else if(var->type->name.compare(charType)==0)
                    {
                        char val;
                        char*sTmp=(char*)s.c_str();
                        int res=sscanf(sTmp,"%c",&val);
                        sscanf(sTmp,"%*[^\n]");
                        if(res==1)
                        {
                           var->valChar=val;
                        }
                    }
                    else if(var->type->name.compare(floatType)==0)
                    {
                        float val;
                        char*sTmp=(char*)s.c_str();
                        int res=sscanf(sTmp,"%f",&val);
                        sscanf(sTmp,"%*[^\n]");
                        if(res==1)
                        {
                           var->valFloat=val;
                        }
                    }
                    else if(var->type->name.compare(stringType)==0)
                    {
                        var->valStr=s;
                    }
                    else if(var->type->name.compare(boolType)==0)
                    {
                        if(s.compare("true")==0)
                        {
                            var->valBool=true;
                        }
                        else if(s.compare("false")==0)
                        {
                            var->valBool=false;
                        }
                    }

                }
            }
        }
        else
        {
            cout<<"Erreur lors de l'ouverture du fichier "<<stream<<endl;
        }
    }
    delete inst;
}


void Out(std::string stream, Instruction* inst)
{
    inst->compile();
    if(inst->retour.size()==1)
    {
        Vargen* var=inst->retour[0];
        if(var==NULL)
        {
            cout<<"Erreur: var est NULL"<<endl;
        }
        else
        {
            string type=var->type->name;
            if(stream.compare("cout")==0)
            {
                if(type.compare(intType)==0)
                {
                    cout<<var->valInt;
                }
                else if(type.compare(charType)==0)
                {
                    cout<<var->valChar;
                }
                else if(type.compare(floatType)==0)
                {
                    cout<<var->valFloat;
                }
                else if(type.compare(boolType)==0)
                {
                    cout<<boolalpha<<var->valBool;
                }
                else if(type.compare(stringType)==0)
                {
                    if(var->valStr.compare("endl")==0)
                    {
                        cout<<endl;
                    }
                }
            }
            else
            {
                ofstream file(stream.c_str(),ios::ate);
                if(file)
                {
                    if(type.compare(intType)==0)
                    {
                        file<<var->valInt;
                    }
                    else if(type.compare(charType)==0)
                    {
                        file<<var->valChar;
                    }
                    else if(type.compare(floatType)==0)
                    {
                        file<<var->valFloat;
                    }
                    else if(type.compare(boolType)==0)
                    {
                        file<<boolalpha<<var->valBool;
                    }
                    else if(type.compare(stringType)==0)
                    {
                        if(var->valStr.compare("endl")==0)
                        {
                            file<<endl;
                        }
                    }
                }
                else
                {
                    cout<<"Erreur lors de l'ouverture du fichier "<<stream<<endl;
                }
            }
        }
    }
}


//opérateur pour les nombres
void Incr(Vargen* var) //operator ++
{
    if(var==NULL)
    {
        cout<<"Erreur: var est NULL"<<endl;
    }
    else if(var->type->name.compare(intType)==0)
    {
        var->valInt+=1;
    }
    else
    {
        cout<<"Erreur pas d'opérateur ++ pour les variables de type "<<var->type->name<<endl;
    }
}


void Decr(Vargen * var) //operator --
{
    if(var==NULL)
    {
        cout<<"Erreur: var est NULL"<<endl;
    }
    else if(var->type->name.compare(intType)==0)
    {
        var->valInt+=1;
    }
    else
    {
        cout<<"Erreur pas d'opérateur ++ pour les variables de type "<<var->type->name<<endl;
    }
}


Vargen* Plus(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
           cout<<"Erreur var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
           cout<<"Erreur var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            int val=var1->valInt+var2->valInt;
            char valStr[4];
            sprintf(valStr,"%d",val);
            string valS=valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,intType,valS);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(intType)==0)
        {
            float val=var1->valFloat+var2->valInt;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var1->valInt+var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var1->valFloat+var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }

        else if(var1->type->name.compare(charType)==0 && var2->type->name.compare(stringType)==0)
        {
            char c=var1->valChar;
            char* ch;
            sprintf(ch,"%c",c);
            string s="";
            s+=ch;
            s+=var2->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(charType)==0)
        {
            string s=var2->valStr;
            s+=var2->valChar;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(stringType)==0)
        {
            int val=var1->valInt;
            char valStr[4];
            sprintf(valStr,"%d",val);
            string s=valStr;
            s+=var2->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(intType)==0)
        {
            int val=var2->valInt;
            char valStr[4];
            sprintf(valStr,"%d",val);
            string valS=valStr;
            string s=var1->valStr;
            s+=valS;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(stringType)==0)
        {
            float val=var1->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string s=valS;
            s+=var2->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string s=var1->valStr;
            s+=valS;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(stringType)==0)
        {
            bool val=var1->valBool;
            string valS=(val? "true" : "false");
            string s=valS;
            s+=var2->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(boolType)==0)
        {
            bool val=var2->valBool;
            string valS=(val? "true" : "false");
            string s=valS;
            s+=var1->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(stringType)==0)
        {
            string s=var1->valStr;
            s+=var2->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else
        {
            cout<<"Erreur, aucun opérateur +("<<var1->type->name<<","<<var2->type->name<<")"<<endl;
        }
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Moins(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
           cout<<"Erreur var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
           cout<<"Erreur var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            int val=var1->valInt-var2->valInt;
            char valStr[4];
            sprintf(valStr,"%d",val);
            string valS=valStr;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,intType,valS);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(intType)==0)
        {
            float val=var1->valFloat-var2->valInt;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var1->valInt-var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var1->valFloat-var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else
        {
            cout<<"Erreur, aucun opérateur -("<<var1->type->name<<","<<var2->type->name<<")"<<endl;
        }
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Mult(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
           cout<<"Erreur var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
           cout<<"Erreur var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            int val=var1->valInt*var2->valInt;
            char valStr[4];
            sprintf(valStr,"%d",val);
            string valS=valStr;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,intType,valS);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(intType)==0)
        {
            float val=var1->valFloat*var2->valInt;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var1->valInt*var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            float val=var1->valFloat*var2->valFloat;
            char valStr[8];
            sprintf(valStr,"%f",val);
            string valS=valStr;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,floatType,valS);
        }
        else
        {
            cout<<"Erreur, aucun opérateur *("<<var1->type->name<<","<<var2->type->name<<")"<<endl;
        }
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Div(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
           cout<<"Erreur var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
           cout<<"Erreur var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            if(var2->valInt==0)
            {
                cout<<"Erreur: division par zéro!"<<endl;
            }
            else
            {
                int val=(var1->valInt)/(var2->valInt);
                char valStr[4];
                sprintf(valStr,"%d",val);
                string valS=valStr;
                string name="moins_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,intType,valS);
            }
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(intType)==0)
        {
            if(var2->valInt==0)
            {
                cout<<"Erreur: division par zéro!"<<endl;
            }
            else
            {
                float val=var1->valFloat/var2->valInt;
                char valStr[8];
                sprintf(valStr,"%f",val);
                string valS=valStr;
                string name="moins_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,floatType,valS);
            }
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(floatType)==0)
        {
            if(var2->valFloat==0)
            {
                cout<<"Erreur: division par zéro!"<<endl;
            }
            else
            {
                float val=var1->valInt/var2->valFloat;
                char valStr[8];
                sprintf(valStr,"%f",val);
                string valS=valStr;
                string name="moins_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,floatType,valS);
            }
        }
        else if(var1->type->name.compare(floatType)==0 && var2->type->name.compare(floatType)==0)
        {
            if(var2->valFloat==0)
            {
                cout<<"Erreur: division par zéro!"<<endl;
            }
            else
            {
                float val=var1->valFloat/var2->valFloat;
                char valStr[8];
                sprintf(valStr,"%f",val);
                string valS=valStr;
                string name="moins_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,floatType,valS);
            }
        }
        else
        {
            cout<<"Erreur, aucun opérateur ∕("<<var1->type->name<<","<<var2->type->name<<")"<<endl;
        }
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Reste(Instruction *inst1, Instruction *inst2)
{
    Vargen* ret=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
           cout<<"Erreur var1 est NULL"<<endl;
        }
        else if(var2==NULL)
        {
           cout<<"Erreur var2 est NULL"<<endl;
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            if(var2->valInt==0)
            {
                cout<<"Erreur: division par zéro!"<<endl;
            }
            else
            {
                int val=(var1->valInt)%(var2->valInt);
                char valStr[4];
                sprintf(valStr,"%d",val);
                string valS=valStr;
                string name="reste_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,intType,valS);
            }
        }
    }
    return ret;
}


void PlusEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* var=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1)
    {
        var=inst1->retour[0];
    }
    Set2(var,Plus(inst1,inst2));
}


void MoinsEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* var=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1)
    {
        var=inst1->retour[0];
    }
    Set2(var,Moins(inst1,inst2));
}


void MultEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* var=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1)
    {
        var=inst1->retour[0];
    }
    Set2(var,Mult(inst1,inst2));
}


void DivEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* var=NULL;
    inst1->compile();
    inst2->compile();
    if(inst1->retour.size()==1)
    {
        Vargen* var=inst1->retour[0];
    }
    Set2(var,Div(inst1,inst2));
}


void Continue(std::vector<DbVar *> _varDb)
{
    Vargen* cont=new Vargen(Continue_,signalType);
    if(_varDb.size()>0)
    {
        int size=_varDb.size();
        if(size>1)
        {
            _varDb[size-2]->erase(Continue_);
            _varDb[size-2]->insert(cont);
        }
        else
        {
            Erreur("Continue dans instruction primaire impossible",_varDb);
        }
    }
}


void Break(std::vector<DbVar *> _varDb)
{
    Vargen* stop=new Vargen(Break_,signalType);
    if(_varDb.size()>0)
    {
        int size=_varDb.size();
        if(size>1)
        {
            _varDb[size-2]->erase(Break_);
            _varDb[size-2]->insert(stop);
        }
        else
        {
            Erreur("Break dans instruction primaire impossible",_varDb);
        }
    }
}


void Erreur(std::string err, std::vector<DbVar *> _varDb)
{
   cout<<"Erreur: "<<err<<endl;
   Vargen* error=new Vargen(Erreur_,signalType);
   //error->arg.push_back();
   if(_varDb.size()>0)
   {
       _varDb[0]->erase(Erreur_);
       _varDb[0]->insert(error);
   }
}


void Retour(Vargen* ret, std::vector<DbVar *> _varDb)
{
    Vargen* retour=new Vargen(Retour_,signalType);
    retour->arg.push_back(ret);
    if(_varDb.size()>0)
    {
        _varDb[0]->erase(Retour_);
        _varDb[0]->insert(retour);
    }
}

