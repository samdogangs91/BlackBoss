#include "Instruction.h"
#include <fstream>

using namespace std;

extern string intType;
extern string charType;
extern string boolType;
extern string floatType;
extern string stringType;
extern string signalType;

extern vector<DbVar*> context;

string Continue_="Continue";
string Break_="Break";
string Erreur_="Erreur";
string Retour_="Retour";


Vargen* identity(std::string cont, vector<Vargen*> arg, vector<DbVar*> varDb)
{
   int res;
   int nb;
   //cout<<"dans identity: cont="<<cont<<endl;
   Vargen* ret=NULL;
   char* contC=(char*)cont.c_str();

   //case int
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
   if(cont.size()>3)
   {
       int size=cont.size();
       if(cont[0]=='"' && cont[size-1]=='"')
       {
           int size=cont.size();
           string s=cont.substr(1,size-2);
           ret=new Vargen(s,stringType,s);
           int sizeTmp=varDb.size();
           if(sizeTmp>1)
           {
               varDb[sizeTmp-2]->insert(ret);
           }
           else
           {
               varDb[sizeTmp-1]->insert(ret);
           }
           return ret;
       }
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
   if(ret==NULL)
   {
       for(k=0;k<arg.size();k++)
       {
           if(arg[k]->name.compare(cont)==0)
           {
               ret=arg[k];
               break;
           }
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


Vargen* NewVar(std::string name, std::string type, vector<Vargen*> funArg, vector<DbVar*> _varDb, std::string arg, bool tmp)
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
        bool okCreate=true;
        for(k=0;k<funArg.size();k++)
        {
            okCreate=!(name.compare(funArg[k]->name)==0);
            if(!okCreate)
            {
                Erreur("La variable est déja présente dans les arguments",context);
                break;
            }
        }
        if(okCreate)
        {
            //cout<<"tmp="<<boolalpha<<tmp<<endl;
            ret=new Vargen(name,type,arg,tmp);
            int size=_varDb.size();
            if(size>1)
            {
                _varDb[size-2]->insert(ret);
            }
        }
    }
    else
    {
        Erreur("La variable "+name+" existe déja!",context);
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


Instruction* NewInst(Instruction* name_,Instruction* argS_, Instruction* retourS_, Instruction* inst_,Instruction* tmp_, Instruction* prior_, Instruction* assoc_, Instruction* isOp_)
{
    bool okName=false;
    bool okArg=false;
    bool okInst=false;
    bool okRetour=false;
    bool okTmp=false;
    bool okPrior=false;
    bool okAssoc=false;
    bool okIsOp=false;
    Instruction* ret=NULL;
    string name="";
    string argS="";
    string retourS="";
    string inst="";
    bool tmp;
    unsigned int prior;
    string assoc="droite";
    bool isOp;
    //cout<<"compile name dans NewInst"<<endl;
    if(name_!=NULL)
    {
        name_->compile();
        if(name_->retour.size()==1)
        {
            Vargen* var=name_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                okName=true;
                name=var->valStr;
            }
            else
            {
                Erreur("l'argument name de NewInst n'est pas de type string",context);
            }
        }
        else
        {
            Erreur("le nom de l'instruction est NULL",context);
        }
    }
    else
    {
        Erreur("Impossible d'obtenir le nom de l'instruction",context);
    }

    //cout<<"compile argS dans NewInst"<<endl;
    if(argS_!=NULL)
    {
        argS_->compile();
        if(argS_->retour.size()==1)
        {
            Vargen* var=argS_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                okArg=true;
                argS=var->valStr;
            }
            else
            {
                Erreur("l'argument argS de NewInst n'est pas de type string",context);
            }
        }
        else
        {
            Erreur("argS de l'instruction est NULL",context);
        }
    }
    else
    {
        Erreur("Impossible d'obtenir argS de l'instruction",context);
    }

    //cout<<"compile retourS dans NewInst"<<endl;
    if(retourS_!=NULL)
    {
        retourS_->compile();
        if(retourS_->retour.size()==1)
        {
            Vargen* var=retourS_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                okRetour=true;
                retourS=var->valStr;
            }
            else
            {
                Erreur("l'argument argS de NewInst n'est pas de type string",context);
            }
        }
        else
        {
            Erreur("l'argument retourS de l'instruction est NULL",context);
        }
    }
    else
    {
        Erreur("Impossible d'obtenir le type de retour de l'instruction",context);
    }

    if(inst_!=NULL)
    {
        inst_->compile();
        if(inst_->retour.size()==1)
        {
            Vargen* var=inst_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                okInst=true;
                inst=var->valStr;
            }
            else
            {
                Erreur("l'argument inst de NewInst n'est pas de type string",context);
            }
        }
        else
        {
            Erreur("l'argument inst de l'instruction est NULL",context);
        }
    }
    else
    {
        Erreur("Impossible d'obtenir le contenu de l'instruction",context);
    }

    if(tmp_!=NULL)
    {
        tmp_->compile();
        if(tmp_->retour.size()==1)
        {
            Vargen* var=tmp_->retour[0];
            if(var->type->name.compare(boolType)==0)
            {
                okTmp=true;
                tmp=var->valBool;
            }
            else
            {
                Erreur("l'argument tmp de NewInst n'est pas de type bool",context);
            }
        }
        else
        {
            Erreur("l'argument tmp de l'instruction est NULL",context);
        }
    }

    if(prior_!=NULL)
    {
        prior_->compile();
        if(prior_->retour.size()==1)
        {
            Vargen* var=prior_->retour[0];
            if(var->type->name.compare(intType)==0)
            {
                prior=var->valInt;
                if(prior>0 && prior<16)
                {
                   okPrior=true;
                }
                else
                {
                   Erreur("l'argument prior de l'instruction doit etre compris entre 0 et 15",context);
                }
            }
            else
            {
                Erreur("l'argument prior de NewInst n'est pas de type int",context);
            }
        }
        else
        {
            Erreur("l'argument prior de l'instruction est NULL",context);
        }
    }

    if(assoc_!=NULL)
    {
        assoc_->compile();
        if(assoc_->retour.size()==1)
        {
            Vargen* var=assoc_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                assoc=var->valStr;
                okAssoc=(assoc.compare("gauche")==0)||(assoc.compare("droite")==0);
                if(!okAssoc)
                {
                   Erreur("l'argument assoc vaut soit 'gauche' soit 'droite'",context);
                }
            }
            else
            {
                Erreur("l'argument assoc de NewInst n'est pas de type string",context);
            }
        }
        else
        {
            Erreur("l'argument assoc de l'instruction est NULL",context);
        }
    }

    if(isOp_!=NULL)
    {
        isOp_->compile();
        if(isOp_->retour.size()==1)
        {
            Vargen* var=isOp_->retour[0];
            if(var->type->name.compare(boolType)==0)
            {
                okIsOp=true;
                isOp=var->valBool;
            }
            else
            {
                Erreur("l'argument isOp de NewInst n'est pas de type bool",context);
            }
        }
        else
        {
            Erreur("l'argument isOp de l'instruction est NULL",context);
        }
    }


    if(okName && okArg && okRetour && okInst && okTmp && okPrior && okAssoc && okIsOp)
    {
        ret=new Instruction(name,argS,retourS,inst,tmp,prior,assoc,isOp);
    }
    else if(okName && okArg && okRetour && okInst && okTmp && okPrior && okAssoc)
    {
        ret=new Instruction(name,argS,retourS,inst,tmp,prior,assoc);
    }
    else if(okName && okArg && okRetour && okInst && okTmp && okPrior)
    {
        ret=new Instruction(name,argS,retourS,inst,tmp,prior);
    }
    else if(okName && okArg && okRetour && okInst && okTmp)
    {
        ret=new Instruction(name,argS,retourS,inst,tmp);
    }
    else if(okName && okArg && okRetour && okInst)
    {
        cout<<"création de l'instruction "<<name<<endl;
        ret=new Instruction(name,argS,retourS,inst);
        cout<<"Instruction "<<name<<" créé"<<endl;
    }
    return ret;
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
    vector<DbVar*> varDb=inst1->varDb;
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
            Erreur("var1 est NULL dans and",varDb);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL dans and",varDb);
        }
        else if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(boolType)==0)
        {
            bool b=(var1->valBool)&&(var2->valBool);
            string bStr=(b? "true" : "false");
            ret=new Vargen(var1->name+"&&"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(boolType)!=0)
        {
            Erreur("Operande gauche de && n'est pas de type bool",varDb);
        }
        else
        {
            Erreur("Operande droite de && n'est pas de type bool",varDb);
        }
    }
    else
    {
        Erreur("Erreur dans l'operateur &&",varDb);
    }
    delete inst1;
    delete inst2;
    return ret;
}


Vargen* Or(Instruction* inst1, Instruction* inst2)// operator ||
{
    inst1->compile();
    inst2->compile();
    vector<DbVar*> varDb=inst1->varDb;
    Vargen* ret=NULL;
    if(inst1->retour.size()==1 && inst2->retour.size()==1)
    {
        Vargen* var1=inst1->retour[0];
        Vargen* var2=inst2->retour[0];
        if(var1==NULL)
        {
            Erreur("var1 est NULL",varDb);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL",varDb);
        }
        else if(var1->type->name.compare(boolType)==0 && var2->type->name.compare(boolType)==0)
        {
            bool b=(var1->valBool)||(var2->valBool);
            string bStr=(b? "true" : "false");
            ret=new Vargen(var1->name+"||"+var2->name,boolType,bStr);
        }
        else if(var1->type->name.compare(boolType)!=0)
        {
            Erreur("Operande gauche de || n'est pas de type bool",varDb);
        }
        else
        {
            Erreur("Operande droite de || n'est pas de type bool",varDb);
        }
    }
    else
    {
        Erreur("Erreur dans l'operateur ||",varDb);
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
        Erreur("var1 est NULL",context);
    }
    else if(var2==NULL)
    {
        Erreur("var2 est NULL",context);
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
        else if(var1->type->name.compare(intType)==0)
        {
            ret=(var1->valInt==var2->valInt);
        }
        else if(var1->type->name.compare(floatType)==0)
        {
            ret=(var1->valFloat==var2->valFloat);
        }
        else if(var1->type->name.compare(charType)==0)
        {
            ret=(var1->valChar==var2->valChar);
        }
        else if(var1->type->name.compare(boolType)==0)
        {
            ret=(var1->valBool==var2->valBool);
        }
        else if(var1->type->name.compare(stringType)==0)
        {
            ret=(var1->valStr.compare(var2->valStr)==0);
        }

    }
    else
    {
        //cout<<"le type des deux variables est différent"<<endl;
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
            Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL",context);
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
            Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL",context);
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
           Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
           Erreur("var2 est NULL",context);
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
            Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL",context);
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
            Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL",context);
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
            Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
            Erreur("var1 est NULL",context);
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
            Erreur("var est NULL",context);
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
            Erreur("var1 est NULL",context);
        }
        else if(varInst==NULL)
        {
            Erreur("va2 est NULL",context);
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
       Erreur("var1 est NULL",context);
    }
    else
    {
        Erreur("var2 est NULL",context);
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
            Erreur("var est NULL",context);
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
        Erreur("var1 est NULL",context);
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
            Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
            Erreur("var2 est NULL",context);
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
                    Erreur("depassement de taille de la string='"+var1->valStr+"'",context);
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
                    Erreur("depassement de taille du container",context);
                }
            }
        }
        else
        {
            Erreur("Pas d'opérateur []("+var1->type->name+","+var2->type->name+")",context);
        }
    }
    else
    {
        Erreur("Les instructions autour de l'opérateur crochet n'ont pas été corretement compilées",context);
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
            Erreur("var1 est NULL",context);
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
                Erreur("var1 est NULL",context);
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
                    Erreur("var1 est NULL",context);
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
            Erreur("Erreur lors de l'ouverture du fichier "+stream,context);
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
            Erreur("var2 est NULL",context);
        }
        else
        {
            //cout<<"dans Out"<<endl;
            //var->print();
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
                    else
                    {
                        cout<<var->valStr;
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
                        file<<var->valStr;
                        if(var->valStr.compare("endl")==0)
                        {
                            file<<endl;
                        }
                    }
                }
                else
                {
                    Erreur("Erreur lors de l'ouverture du fichier "+stream,context);
                }
            }
        }
    }
    else
    {
        Erreur("lors du parsing de l'instruction à émettre sur "+stream,context);
    }
}


//opérateur pour les nombres
void Incr(Vargen* var) //operator ++
{
    if(var==NULL)
    {
        Erreur("var est NULL",context);
    }
    else if(var->type->name.compare(intType)==0)
    {
        var->valInt+=1;
    }
    else
    {
        Erreur("pas d'opérateur ++ pour les variables de type "+var->type->name,context);
    }
}


void Decr(Vargen * var) //operator --
{
    if(var==NULL)
    {
        Erreur("var est NULL",context);
    }
    else if(var->type->name.compare(intType)==0)
    {
        var->valInt+=1;
    }
    else
    {
        Erreur("pas d'opérateur -- pour les variables de type "+var->type->name,context);
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
           Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
           Erreur("var2 est NULL",context);
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
            Erreur("aucun opérateur +("+var1->type->name+","+var2->type->name+")",context);
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
           Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
           Erreur("var2 est NULL",context);
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
            Erreur("aucun opérateur -("+var1->type->name+","+var2->type->name+")",context);
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
           Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
           Erreur("var2 est NULL",context);
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
            Erreur("aucun opérateur *("+var1->type->name+","+var2->type->name+")",context);
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
           Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
           Erreur("var2 est NULL",context);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            if(var2->valInt==0)
            {
                Erreur("division par zéro!",context);
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
                Erreur("division par zéro!",context);
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
                Erreur("division par zéro!",context);
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
                Erreur("division par zéro!",context);
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
            Erreur("aucun opérateur ∕("+var1->type->name+","+var2->type->name+")",context);
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
           Erreur("var1 est NULL",context);
        }
        else if(var2==NULL)
        {
           Erreur("var1 est NULL",context);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(intType)==0)
        {
            if(var2->valInt==0)
            {
                Erreur("division par zéro!",context);
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


/*
 * compile l'appel d'une instruction présente dans la bdd
 * si var est Non NULL alors il s'agit d'un appel de methode liée à var
 */
vector<Vargen*> makeInstruction(string nameInst, std::vector<Vargen *> _arg, std::vector<Vargen*> exArg, std::vector<DbVar *> _varDb, Vargen* var)
{
    vector<Vargen*> ret;

    //Recherche de l'instruction à compiler
    string argT="";
    unsigned int k;
    vector<DbVar*> exContext=context; //on garde en mémoire l'ancien contexte
    for(k=0;k<_arg.size();k++)
    {
        argT+=_arg[k]->type->name+";";
    }
    string id=getIdInstruction(nameInst,argT);
    Instruction* inst=new Instruction(id);


    //Préparation à la compilation
    //Mise en place des arguments
    for(k=0;k<_arg.size();k++)
    {
        inst->arg.push_back(_arg[k]);
    }
    //Mise en place des attributs de classe si l'instruction est une méthode
    if(var!=NULL)
    {
        if(inst->varDb.size()>0)
        {
            DbVar* dbInst=inst->varDb[0];
            for(k=0;k<var->arg.size();k++)
            {
                dbInst->insert(new Vargen(var->arg[k]));
            }
        }
    }


    //Compilation
    inst->compile();
    for(k=0;k<inst->retour.size();k++)
    {
        ret.push_back(inst->retour[k]);
    }
    delete inst;
    context=exContext; //mise en place des anciennes variables


    //Destruction des variables non réutilisées
    for(k=0;k<_arg.size();k++)
    {
        Vargen* crtVar=_arg[k];
        bool okDelete=true;
        Vargen* search=NULL;
        unsigned int j;
        for(j=0;j<_varDb.size();j++)
        {
            search=_varDb[j]->find(crtVar->name);
            if(search!=NULL)
            {
                okDelete=false;
                break;
            }
        }
        if(okDelete)
        {
            for(j=0;j<exArg.size();j++)
            {
                okDelete=!(crtVar->name.compare(exArg[j]->name)==0);
                if(!okDelete)
                {
                    break;
                }
            }
            if(okDelete)
            {
                crtVar->deleteVar();
                delete crtVar;
            }
        }
    }


    return ret;
}

