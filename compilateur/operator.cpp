#include "Instruction.h"

using namespace std;

extern string intType;
extern string charType;
extern string boolType;
extern string floatType;
extern string stringType;


Vargen* identity(std::string cont, DbVar* varTmp, DbVar* varPar)
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
       varTmp->insert(ret);
       return ret;
   }

   //case char
   char c;
   res=sscanf(contC,"'%c'",&c);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,charType,cont);
      varTmp->insert(ret);
      return ret;
   }

   //case float
   float val;
   res=sscanf(contC,"%f",&val);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,floatType,cont);
      varTmp->insert(ret);
      return ret;
   }

   //case bool
   if(cont.compare("true")==0 || cont.compare("false")==0)
   {
      ret=new Vargen(cont,boolType,cont);
      varTmp->insert(ret);
      return ret;
   }

   //case string
   char* contStr;
   res=sscanf(contC,"\"%s\"",contStr);
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,stringType,cont);
      varTmp->insert(ret);
      return ret;
   }

   //case Vargen
   ret=varPar->find(cont);
   if(ret==NULL)
   {
       ret=varTmp->find(cont);
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
    delete inst;
    return ret;
}


Vargen* NewVar(std::string name, std::string type, DbVar* varTmp, std::string arg, bool tmp)
{
    Vargen* ret=NULL;
    ret=varTmp->find(name);
    if(ret==NULL)
    {
        ret=new Vargen(name,type,arg,tmp);
    }
    else
    {
        cout<<"La variable "<<name<<" n'existe pas!"<<endl;
    }

    return ret;
}


void deleteVar(string name, DbVar *varTmp)
{
    varTmp->erase(name);
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
        if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(boolType)==0)
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
        if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(boolType)==0)
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
    if(var1->type->name.compare(var2->type->name)==0)
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
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res=Equal2(var1,var2);
        string bStr=(res? "true" : "false");
        ret=new Vargen(var1->name+"=="+var2->name,boolType,bStr);
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Diff(Instruction* inst1, Instruction* inst2)//operator !=
{
    Vargen* ret=NULL;
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res=!Equal2(var1,var2);
        string bStr=(res? "true" : "false");
        ret=new Vargen(var1->name+"=="+var2->name,boolType,bStr);
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* SupEqual(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
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
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
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
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
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
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        bool res;
        if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
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
    if(inst->retour.size()==1)
    {
        Vargen* var=inst->retour[0];
        if(var->type->name.compare(boolType)==0)
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
    if(inst->retour.size()==1)
    {
        Vargen* varInst=inst->retour[0];
        var->setVal(varInst);
    }
    delete inst;
}


Vargen* getAtt(Instruction* instVar, std::string att) //operator ->
{
    Vargen* ret=NULL;
    if(instVar->retour.size()==1)
    {
        Vargen* var=instVar->retour[0];
        ret=var->getAtt(att);
    }
    delete instVar;
    return ret;
}


Instruction* getMeth(Vargen* var, std::string name, std::string argT,std::string retourT)
{
    return var->getMeth(name,argT,retourT);
}


Vargen* Cro(Instruction* inst, Instruction* num)
{
    Vargen* ret=NULL;
    if(inst->retour.size()==1 && inst->retour.size()==1)
    {
        Vargen* var1=inst->retour[0];
        Vargen* var2=inst->retour[0];
        if(var2->type->name.compare(intType)==0)
        {
            int k=var2->valInt;
            if(var1->type->name.compare(stringType)==0)
            {

            }
            else if(var1->type->isContainer())
            {
              if(k>=0 && k<var1->arg.size())
              {

              }
            }
        }
    }
    return ret;
}


Vargen* Point(Instruction* inst1, Instruction* inst2)
{
    Vargen* ret=NULL;
    if(instVar->retour.size()==1)
    {
        Vargen* var=instVar->retour[0];
        ret=var->getAtt(att);
    }
    delete instVar;
    return ret;
}


void In(std::string stream, Instruction* inst)
{
    if(stream.compare("cin")==0)
    {
        if(inst->retour.size()==1)
        {
            Vargen* var=inst->retour[0];
            string s;
            getline(cin,s);
            if(var->type->name.compare(intType)==0)
            {
                int val;

            }
        }
    }

}


void Out(std::string stream, Instruction* inst)
{

}


//opérateur pour les nombres
void Incr(Vargen* var) //operator ++
{

}


void Decr(Vargen * var) //operator --
{

}


Vargen* Plus(Instruction* inst1, Instruction* inst2)
{

}


Vargen* Moins(Instruction* inst1, Instruction* inst2)
{

}


Vargen* Mult(Instruction* inst1, Instruction* inst2)
{

}


Vargen* Div(Instruction* inst1, Instruction* inst2)
{

}


Vargen* PlusEqual(Instruction* inst1, Instruction* inst2)
{

}


Vargen* MoinsEqual(Instruction* inst1, Instruction* inst2)
{

}


Vargen* MultEqual(Instruction* inst1, Instruction* inst2)
{

}


Vargen* DivEqual(Instruction* inst1, Instruction* inst2)
{

}
