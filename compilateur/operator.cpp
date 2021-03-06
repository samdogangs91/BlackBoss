#include "Instruction.h"
#include <fstream>
#include <sstream>
#include "Database.h"

using namespace std;

extern string intType;
extern string charType;
extern string boolType;
extern string doubleType;
extern string stringType;
extern string signalType;

extern Database* memory;

extern vector<DbVar*> context;

string Continue_="Continue";
string Break_="Break";
string Erreur_="Erreur";
string Retour_="Retour";


Vargen* selectArg(string cont, vector<DbVar*> varDb)
{
    Vargen* ret=NULL;
    unsigned int k;
    int nbFle=0;
    string var="";
    string att="";
    int nbAtt=0;
    for(k=0;k<cont.size();k++)
    {
       if(k<=cont.size()-3)// toto->c
       {
            if(cont[k]=='-' && cont[k+1]=='>')
            {
                nbFle++;
                k++;
                continue;
            }
       }
       if(!isNotSpe(cont[k]))
       {
           continue;
       }
       if(nbFle==0)
       {
           var+=cont[k];
       }
       if(nbFle==1)
       {
           if(var.compare("")!=0)
           {
               //cout<<"var: "<<var<<endl;
               unsigned int i;
               for(i=0;i<varDb.size();i++)
               {
                   ret=varDb[i]->find(var);
                   if(ret!=NULL)
                   {
                       break;
                   }
               }
               var="";
           }
       }
       if(nbFle>0)
       {
           if(nbFle==nbAtt+2)
           {
               if(att.compare("")!=0)
               {
                   if(ret!=NULL)
                   {
                        ret=ret->getAtt(att);
                   }
                   else
                   {
                       break;
                   }
                   att="";
               }
           }
           else if(k==cont.size()-1)
           {
               att+=cont[k];
               if(att.compare("")!=0)
               {
                   //cout<<"att: "<<att<<endl;
                   if(ret!=NULL)
                   {
                        ret=ret->getAtt(att);
                   }
                   else
                   {
                       break;
                   }
                   att="";
               }
           }
           else
           {
               att+=cont[k];
               //cout<<"att tmp: "<<att<<endl;
           }
       }

    }
    return ret;
}


Vargen* identity(std::string cont, vector<Vargen*> arg,vector<DbVar*> varDb)
{
   int res;
   int nb;
   //cout<<"dans identity: cont="<<cont<<endl;
   Vargen* ret=NULL;
   char* contC=(char*)cont.c_str();


   //on vérifie si cont contient un '.' et peut donc être un réel
   bool verifInt=true;
   unsigned int l;
   for(l=0;l<cont.size();l++)
   {
       if(cont[l]=='.' ||(cont[l]>='a' && cont[l]<='z'))
       {
           verifInt=false;
           break;
       }
   }
   //case int
   if(verifInt)
   {
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
   }


   //case double
   double val;
   res=sscanf(contC,"%lf",&val);
   //cout<<"identity: cont="<<cont<<", et val="<<val<<endl;
   sscanf(contC,"%*[^\n]");
   if(res==1)
   {
      ret=new Vargen(cont,doubleType,cont);
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
   if(cont.size()>=2)
   {
       int size=cont.size();
       if(cont[0]=='"' && cont[size-1]=='"')
       {
           int size=cont.size();
           string s=cont.substr(1,size-2);
           ret=new Vargen(s,stringType,s);
           int sizeTmp=context.size();
           if(sizeTmp>1)
           {
               context[sizeTmp-2]->insert(ret);
           }
           else
           {
               context[sizeTmp-1]->insert(ret);
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
           return ret;
       }
   }
   if(ret==NULL)
   {
       for(k=0;k<arg.size();k++)
       {
           if(arg[k]->name.compare(cont)==0)
           {
               ret=arg[k];
               return ret;
           }
       }
   }

   //on  vérifie si cont contient une '->' et peut être une sélection d'attribut var1->attx->sousAtty
   if(ret==NULL)
   {
       string sel=cont;
       unsigned int size=sel.size();
       if(size>0)
       {
           if(sel[size-1]==';')
           {
               sel=sel.substr(0,size-1);
               sel=uselessPar(sel);
               ret=selectArg(sel,varDb);
           }
           else
           {
               sel=uselessPar(sel);
               ret=selectArg(sel,varDb);
           }

       }
   }


   return ret;
}



void setTmp(Instruction *instAtt, Instruction *tmpInst)
{
    Vargen* att=NULL;
    Vargen* tmp=NULL;
    instAtt->compile();
    if(instAtt->retour.size()==1)
    {
        att=instAtt->retour[0];
        /*unsigned int size=_varDb.size();
        if(size>1)
        {
            _varDb[size-2]->insert(att);
        }*/
    }
    else
    {
        Erreur("la variable de gauche dans setTmp est NULL",context);
    }

    tmpInst->compile();
    if(tmpInst->retour.size()==1)
    {
        Vargen* ret=tmpInst->retour[0];
        if(ret->type->name.compare(boolType)==0)
        {
            tmp=ret;
        }
        else
        {
            Erreur("la variable de doite dans setTmp n'est pas de type bool",context);
        }
    }
    else
    {
        Erreur("la variable de droite dans setTmp est NULL",context);
    }

    if(att!=NULL && tmp!=NULL)
    {
        att->setTmp(tmp->valBool);
    }
}



Vargen* Return(Instruction* inst)
{
    Vargen* ret=NULL;
    inst->compile();
    //cout<<"dans return "<<endl;
    //cout<<"fin compile "<<inst->type<<endl;
    if(inst->retour.size()==1)
    {        
        ret=new Vargen(inst->retour[0]);
        //cout<<"return "<<ret->name<<endl;
        ret->update();
    }
    int size=inst->varDb.size();
    if(size>1)
    {
        inst->varDb[size-2]->insert(ret);
    }
    delete inst;
    return ret;
}


Vargen* NewVar(Instruction* name_, Instruction* type_, vector<Vargen*> funArg, vector<DbVar*> _varDb, Instruction* arg_, Instruction* tmp_)
{
    Vargen* ret=NULL;
    string name="";
    string type="";
    string arg="";
    bool tmp=true;
    name_->compile();
    if(name_->retour.size()==1)
    {
        Vargen* var=name_->retour[0];
        if(var->type->name.compare(stringType)==0)
        {
            name=var->valStr;
            unsigned int size=_varDb.size();
            if(size>1)
            {
                _varDb[size-2]->insert(new Vargen(var));
            }
        }
        else
        {
            Erreur("La variable name n'est pas de type string mais de type "+var->type->name,context);
        }
    }
    else
    {
        Erreur("la variable name, pour créer une nouvelle variable, est NULL",context);
    }

    type_->compile();
    if(type_->retour.size()==1)
    {
        Vargen* var=type_->retour[0];
        if(var->type->name.compare(stringType)==0)
        {
            type=var->valStr;
            unsigned int size=_varDb.size();
            if(size>1)
            {
                _varDb[size-2]->insert(new Vargen(var));
            }
        }
        else
        {
            Erreur("La variable type n'est pas de type string mais de type "+var->type->name,context);
        }
    }
    else
    {
        Erreur("la variable type, pour créer une nouvelle variable, est NULL",context);
    }

    if(arg_!=NULL)
    {
        arg_->compile();
        if(arg_->retour.size()==1)
        {
            Vargen* var=arg_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                arg=var->valStr;
                unsigned int size=_varDb.size();
                if(size>1)
                {
                    _varDb[size-2]->insert(new Vargen(var));
                }
            }
            else
            {
                Erreur("La variable arg n'est pas de type string mais de type "+var->type->name,context);
            }
        }
        else
        {
            Erreur("la variable arg, pour créer une nouvelle variable, est NULL",context);
        }
    }

    if(tmp_!=NULL)
    {
        tmp_->compile();
        if(arg_->retour.size()==1)
        {
            Vargen* var=tmp_->retour[0];
            if(var->type->name.compare(boolType)==0)
            {
                tmp=var->valBool;
                unsigned int size=_varDb.size();
                if(size>1)
                {
                    _varDb[size-2]->insert(new Vargen(var));
                }
            }
            else
            {
                Erreur("La variable tmp dans NewVar n'est pas de type bool mais de type "+var->type->name,context);
            }
        }
        else
        {
            Erreur("la variable tmp, pour créer une nouvelle variable, est NULL",context);
        }
    }

    if(name.compare("")!=0 && type.compare("")!=0)
    {
        unsigned int k;
        for(k=0;k<_varDb.size();k++)
        {
            ret=_varDb[k]->find(name);
            if(ret!=NULL)
            {
                if(ret->type->name.compare(stringType)==0)
                {
                    if(ret->valStr.compare(name)==0)
                    {
                        _varDb[k]->remove(ret);
                        ret=NULL;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
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
                //cout<<"Create NewVar: name="<<name<<", tmp="<<boolalpha<<tmp<<endl;
                ret=new Vargen(name,type,arg,tmp);
                int size=_varDb.size();
                if(size>1)
                {
                    _varDb[size-2]->insert(new Vargen(ret));
                }
            }
        }
        else
        {
            Erreur("La variable "+name+" existe déja!",context);
        }
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


Instruction* NewInst(Instruction* name_,Instruction* argS_, Instruction* retourS_, Instruction* inst_, vector<DbVar*> varDb_, Instruction* tmp_, Instruction* prior_, Instruction* assoc_, Instruction* isOp_)
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
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    varDb_[size-2]->insert(new Vargen(var));
                }
                if(var->type->name.compare(stringType)==0)
                {
                    okName=true;
                    name=recopieString(var->valStr);
                }
                else
                {
                    Erreur("l'argument name de NewInst n'est pas de type string",context);
                }
            }
            else
            {

            }
        }
        else
        {
            Erreur("le nom de l'instruction est NULL",context);
        }
        //delete name_;
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
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    varDb_[size-2]->insert(new Vargen(var));
                }
                if(var->type->name.compare(stringType)==0)
                {
                    okArg=true;
                    argS=var->valStr;
                    //cout<<"argS dans NewInst="<<argS<<endl;
                }
                else
                {
                    Erreur("l'argument argS de NewInst n'est pas de type string",context);
                }
            }
            else
            {

            }
        }
        else
        {
            Erreur("argS de l'instruction est NULL",context);
        }
        //delete argS_;
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
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    varDb_[size-2]->insert(new Vargen(var));
                }
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

            }
        }
        else
        {
            Erreur("l'argument retourS de l'instruction est NULL",context);
        }
        //delete retourS_;
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
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    varDb_[size-2]->insert(new Vargen(var));
                }
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

            }
        }
        else
        {
            Erreur("l'argument inst de l'instruction est NULL",context);
        }
        //delete inst_;
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
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    varDb_[size-2]->insert(new Vargen(var));
                }
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
        }
        else
        {
            Erreur("l'argument tmp de l'instruction est NULL",context);
        }
        //delete tmp_;
    }

    if(prior_!=NULL)
    {
        prior_->compile();
        if(prior_->retour.size()==1)
        {
            Vargen* var=prior_->retour[0];
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    varDb_[size-2]->insert(new Vargen(var));
                }
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

            }
        }
        else
        {
            Erreur("l'argument prior de l'instruction est NULL",context);
        }
        //delete prior_;
    }

    if(assoc_!=NULL)
    {
        assoc_->compile();
        if(assoc_->retour.size()==1)
        {
            Vargen* var=assoc_->retour[0];
            unsigned int size=varDb_.size();
            if(size>1)
            {
                varDb_[size-2]->insert(new Vargen(var));
            }
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
        //delete assoc_;
    }

    if(isOp_!=NULL)
    {
        isOp_->compile();
        if(isOp_->retour.size()==1)
        {
            Vargen* var=isOp_->retour[0];
            unsigned int size=varDb_.size();
            if(size>1)
            {
                varDb_[size-2]->insert(new Vargen(var));
            }
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
        //delete isOp_;
    }


    if(okName && okArg && okRetour && okInst && okTmp && okPrior && okAssoc && okIsOp)
    {
        vector<DbVar*> exContext=context;
        ret=new Instruction(name,argS,retourS,inst,varDb_,tmp,prior,assoc,isOp);
        context=exContext;
    }
    else if(okName && okArg && okRetour && okInst && okTmp && okPrior && okAssoc)
    {
        vector<DbVar*> exContext=context;
        ret=new Instruction(name,argS,retourS,inst,varDb_,tmp,prior,assoc);
        context=exContext;
    }
    else if(okName && okArg && okRetour && okInst && okTmp && okPrior)
    {
        vector<DbVar*> exContext=context;
        ret=new Instruction(name,argS,retourS,inst,varDb_,tmp,prior);
        context=exContext;
    }
    else if(okName && okArg && okRetour && okInst && okTmp)
    {
        vector<DbVar*> exContext=context;
        ret=new Instruction(name,argS,retourS,inst,varDb_,tmp);
        context=exContext;
    }
    else if(okName && okArg && okRetour && okInst)
    {
        //cout<<"création de l'instruction "<<name<<endl;
        vector<DbVar*> exContext=context;
        ret=new Instruction(name,argS,retourS,inst,varDb_);
        context=exContext;
        //cout<<"Instruction "<<name<<" créé"<<endl;
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


void addMeth(Instruction* nameType_, Instruction* nameMeth_,vector<DbVar*> varDb,Instruction* argTMeth_)
{
    string nameType="";
    string nameMeth="";
    string argTMeth="";

    nameType_->compile();
    if(nameType_->retour.size()==1)
    {
         Vargen* var=nameType_->retour[0];
         if(var->type->name.compare(stringType)==0)
         {
             nameType=var->valStr;
             unsigned int size=varDb.size();
             if(size>1)
             {
                 varDb[size-2]->insert(new Vargen(var));
             }
         }
         else
         {
             Erreur("La variable nameType dans l'ajout de méthode n'est pas de type string",context);
         }
    }
    else
    {
        Erreur("La variable nameType dans l'ajout de méthode return NULL",context);
    }

    nameMeth_->compile();
    if(nameMeth_->retour.size()==1)
    {
        Vargen* var=nameMeth_->retour[0];
        if(var->type->name.compare(stringType)==0)
        {
            nameMeth=var->valStr;
            unsigned int size=varDb.size();
            if(size>1)
            {
                varDb[size-2]->insert(new Vargen(var));
            }
        }
        else
        {
            Erreur("La variable nameMeth dans l'ajout de méthode n'est pas de type string",context);
        }
    }
    else
    {
        Erreur("La variable nameMeth dans l'ajout de méthode return NULL",context);
    }

    if(argTMeth_!=NULL)
    {
        argTMeth_->compile();
        if(argTMeth_->retour.size()==1)
        {
            Vargen* var=argTMeth_->retour[0];
            if(var->type->name.compare(stringType)==0)
            {
                argTMeth=var->valStr;
                unsigned int size=varDb.size();
                if(size>1)
                {
                    varDb[size-2]->insert(new Vargen(var));
                }
            }
            else
            {
                Erreur("La variable argTMeth dans l'ajout de méthode n'est pas de type string",context);
            }
        }
        else
        {
            Erreur("La variable argTMeth dans l'ajout de méthode return NULL",context);
        }
    }

    if(nameType.compare("")!=0 && nameMeth.compare("")!=0)
    {
        Type* type= new Type(nameType);
        //cout<<"dans addMeth: "<<nameMeth+"("+argTMeth+")"<<endl;
        string id=getIdInstruction(nameMeth,argTMeth);
        stringstream ss;
        ss<<id;
        unsigned int idNum;
        ss>>idNum;
        type->addMeth(idNum);
        delete type;
    }
    else
    {
        Erreur("La méthode n'a pas été ajoutée au type voulu",context);
    }
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
    stringstream ss;
    ss<<nameMeth;
    unsigned int idNum;
    ss>>idNum;
    type->deleteMeth(idNum);
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
        else if(var1->type->name.compare(doubleType)==0)
        {
            ret=(var1->valReal==var2->valReal);
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
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            res=((var1->valReal)>=(var2->valReal));
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
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            res=((var1->valReal)>(var2->valReal));
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
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            res=((var1->valReal)<=(var2->valReal));
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
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            res=((var1->valReal)<(var2->valReal));
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
            Erreur("var1 dans Set est NULL",context);
        }
        else if(varInst==NULL)
        {
            Erreur("var2 dans Set est NULL",context);
        }
        else
        {
            var->setVal(varInst);
        }
    }
    else
    {
        Erreur("l'instruction droite de l'instruction Set renvoie NULL",context);
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


Vargen* getAtt(Vargen* var, std::string att) //operator ->
{
    Vargen* ret=NULL;    
    if(var==NULL)
    {
        Erreur("var est NULL",context);
    }
    else
    {
        if(att.compare("size")==0)
        {
            if(var->type->name.compare(stringType)==0)
            {
                stringstream ss;
                ss<<var->valStr;
                string valS="";
                ss>>valS;
                ret=new Vargen(var->name+"_size","int",valS);
            }
            else if(var->type->isContainer())
            {
                stringstream ss;
                ss<<var->arg.size();
                string valS="";
                ss>>valS;
                ret=new Vargen(var->name+"_size","int",valS);
            }
            else
            {
                Erreur("size n'est pas un attribut de "+var->type->name,context);
            }
        }
        else
        {
            ret=var->getAtt(att);
        }
    }
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
                    stringstream ss;
                    ss<<c;
                    //char* ch;
                    //sprintf(ch,"%c",c);
                    string str="";
                    ss>>str;
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
            Vargen* var1=inst->retour[0];
            unsigned int k;
            Vargen*var=NULL;
            for(k=0;k<inst->varDb.size()-1;k++)
            {
                var=inst->varDb[k]->find(var1->name);
                if(var!=NULL)
                {
                    break;
                }
            }
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
                else if(var->type->name.compare(doubleType)==0)
                {
                    double val;
                    char*sTmp=(char*)s.c_str();
                    int res=sscanf(sTmp,"%f",&val);
                    sscanf(sTmp,"%*[^\n]");
                    if(res==1)
                    {
                       var->valReal=val;
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
                    else if(var->type->name.compare(doubleType)==0)
                    {
                        double val;
                        char*sTmp=(char*)s.c_str();
                        int res=sscanf(sTmp,"%f",&val);
                        sscanf(sTmp,"%*[^\n]");
                        if(res==1)
                        {
                           var->valReal=val;
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
                else if(type.compare(doubleType)==0)
                {
                    cout<<var->valReal;
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
                    else if(type.compare(doubleType)==0)
                    {
                        file<<var->valReal;
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
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(intType)==0)
        {
            double val=var1->valReal+var2->valInt;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var1->valInt+var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var1->valReal+var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
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
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(stringType)==0)
        {
            double val=var1->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string s=valS;
            s+=var2->valStr;
            string name="plus_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,stringType,s);
        }
        else if(var1->type->name.compare(stringType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
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
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,intType,valS);
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(intType)==0)
        {
            double val=var1->valReal-var2->valInt;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var1->valInt-var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var1->valReal-var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="moins_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
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
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,intType,valS);
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(intType)==0)
        {
            double val=var1->valReal*var2->valInt;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var1->valInt*var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            double val=var1->valReal*var2->valReal;
            stringstream ss;
            ss<<val;
            string valS="";
            ss>>valS;
            string name="mult_"+var1->name+"_"+var2->name;
            ret=new Vargen(name,doubleType,valS);
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
                string name="Div_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,intType,valS);
            }
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(intType)==0)
        {
            if(var2->valInt==0)
            {
                Erreur("division par zéro!",context);
            }
            else
            {
                double val=var1->valReal/var2->valInt;
                stringstream ss;
                ss<<val;
                string valS="";
                ss>>valS;
                string name="Div_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,doubleType,valS);
            }
        }
        else if(var1->type->name.compare(intType)==0 && var2->type->name.compare(doubleType)==0)
        {
            if(var2->valReal==0)
            {
                Erreur("division par zéro!",context);
            }
            else
            {
                double val=var1->valInt/var2->valReal;
                stringstream ss;
                ss<<val;
                string valS="";
                ss>>valS;
                string name="Div_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,doubleType,valS);
            }
        }
        else if(var1->type->name.compare(doubleType)==0 && var2->type->name.compare(doubleType)==0)
        {
            if(var2->valReal==0)
            {
                Erreur("division par zéro!",context);
            }
            else
            {
                double val=var1->valReal/var2->valReal;
                stringstream ss;
                ss<<val;
                string valS="";
                ss>>valS;
                string name="Div_"+var1->name+"_"+var2->name;
                ret=new Vargen(name,doubleType,valS);
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



Vargen* PlusU(Instruction *inst)
{
   Vargen* ret=NULL;
   inst->compile();
   if(inst->retour.size()==1)
   {
       Vargen* ret2=new Vargen(inst->retour[0]);
       if(ret2!=NULL)
       {
           if(ret2->type->name.compare(intType)==0 || ret2->type->name.compare(doubleType)==0)
           {
               ret=ret2;
           }
           else
           {
               Erreur("La variable de retour dans Plus Unaire est NULL",context);
           }
       }
       else
       {
           Erreur("la variable de retour dans Plus Unaire est NULL",context);
       }
   }
   delete inst;
   return ret;
}


Vargen* MoinsU(Instruction *inst)
{
   Vargen* ret=NULL;
   inst->compile();
   if(inst->retour.size()==1)
   {
       Vargen* ret2=new Vargen(inst->retour[0]);
       if(ret2!=NULL)
       {
           if(ret2->type->name.compare(intType)==0)
           {
               ret=ret2;
               ret->valInt=-ret->valInt;
           }
           else if(ret2->type->name.compare(doubleType)==0)
           {
               ret=ret2;
               ret->valReal=-ret->valReal;
           }
           else
           {
               Erreur("La variable de retour dans Moins Unaire est NULL",context);
           }
       }
       else
       {
           Erreur("la variable de retour dans Moins Unaire est NULL",context);
       }
   }
   delete inst;
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
    if(ret!=NULL)
    {
        retour->arg.push_back(new Vargen(ret));
        if(_varDb.size()>0)
        {
            _varDb[0]->erase(Retour_);
            _varDb[0]->insert(retour);
        }
    }
    else
    {
        Erreur("La variable de retour est NULL",_varDb);
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
    string id="";
    if(var==NULL)
    {
        id=getIdInstruction(nameInst,argT);
        if(id.compare("")==0)
        {
            //cout<<"id="<<id<<endl;
            nameInst=nameInst+"::"+nameInst;
            id=getIdInstruction(nameInst,argT);
        }
    }
    else
    {
        string name="";
        name=var->type->name+"::"+nameInst;
        id=getIdInstruction(nameInst,argT);
    }
    if(id.compare("")!=0)
    {
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
            inst->attClass=var->arg;
        }


        //Compilation
        inst->compile();
        //cout<<"fin de compilation de "<<inst->name<<endl;
        for(k=0;k<inst->retour.size();k++)
        {
            ret.push_back(new Vargen(inst->retour[k]));
        }
        //delete inst;
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
                    //cout<<"deleteVar: "<<crtVar->name<<endl;
                    crtVar->deleteVar();
                    delete crtVar;
                }
            }
        }
        delete inst;
    }
    else
    {
        Erreur("l'instruction "+nameInst+"("+argT+") n'existe pas!",context);
    }
    return ret;
}


int getMaxTable(string type)
{
    int ret=-1;
    string req="select max("+type+"_id) as "+type+"_id from "+type+";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {

        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            string retS=row[0]?  row[0]:"";
            if(retS.compare("")!=0)
            {
                stringstream ss;
                ss<<retS;
                ss>>ret;
            }
            else
            {
                ret=0;
            }
        }
        else
        {
            ret=0;
        }
    }
    else
    {
        cout<<"erreur requete sql:"<<req<<endl;
    }
    mysql_free_result(res);
    return ret;
}


string transfContMeth(string nameVar,string cont, vector<Attribut*> att)
{
    string ret="";
    unsigned int k;
    string actualMot="";
    for(k=0;k<cont.size();k++)
    {
        bool isOp=isOpe(cont[k]);
        if(isOp)
        {
            if(actualMot.compare("")!=0)
            {
                unsigned int l;
                bool isAtt=false;
                //bool isBorStr=false; //isBasicOrString
                for(l=0;l<att.size();l++)
                {
                    if(att[l]->name.compare(actualMot)==0)
                    {
                        isAtt=true;
                        break;
                    }
                }
                if(isAtt)
                {
                    ret+=nameVar+"->"+actualMot;
                    isAtt=false;
                }
                else
                {
                    ret+=actualMot;
                }
                ret+=cont[k];
                actualMot="";
            }
        }
        else
        {
            actualMot+=cont[k];
        }
        if(k==cont.size()-1 && !isOp)
        {
            if(actualMot.compare("")!=0)
            {
                unsigned int l;
                bool isAtt=false;
                for(l=0;l<att.size();l++)
                {
                    if(att[k]->name.compare(actualMot)==0)
                    {
                        isAtt=true;
                        break;
                    }
                }
                if(isAtt)
                {
                    ret+=nameVar+"->"+actualMot;
                    isAtt=false;
                }
                else
                {
                    ret+=actualMot;
                }
                ret+=cont[k];
                actualMot="";
            }
        }

    }

    for(k=0;k<att.size();k++)
    {
        ret+="SetTmp("+nameVar+"->"+att[k]->name+",false);";
    }
    return ret;
}


void NewConst(Instruction *nameType_, std::vector<DbVar *> varDb_, Instruction *arg_, Instruction *cont_)
{
    string nameType="";
    string arg="";
    string cont="";
    bool tmp=true;

    nameType_->compile();
    if(nameType_->retour.size()==1)
    {
        Vargen* var=nameType_->retour[0];
        if(var!=NULL)
        {
            if(var->type->name.compare(stringType)==0)
            {
                nameType=var->valStr;
                Type* type=new Type(nameType);
                tmp=type->tmp;
                delete type;
            }
            else
            {
                Erreur("Le champs nameType de NewConst n'est pas de type string",context);
            }
            unsigned int size=varDb_.size();
            if(size>1)
            {
                var->setTmp(tmp);
                varDb_[size-2]->insert(new Vargen(var));
            }

        }
        else
        {
            Erreur("Le champs nameType de NewConst est NULL",context);
        }
    }
    else
    {
        Erreur("Le champs nameType de NewConst return NULL",context);
    }

    if(arg_!=NULL)
    {
        arg_->compile();
        if(arg_->retour.size()==1)
        {
            Vargen* var=arg_->retour[0];
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    var->setTmp(tmp);
                    varDb_[size-2]->insert(new Vargen(var));
                }
                if(var->type->name.compare(stringType)==0)
                {
                    arg=var->valStr;
                }
                else
                {
                    Erreur("Le champs arg de NewConst n'est pas de type string",context);
                }
            }
            else
            {
                Erreur("Le champs arg de NewConst est NULL",context);
            }
        }
        else
        {
            Erreur("Le champs arg de NewConst return NULL",context);
        }
    }

    if(cont_!=NULL)
    {
        cont_->compile();
        if(cont_->retour.size()==1)
        {
            Vargen* var=cont_->retour[0];
            if(var!=NULL)
            {
                unsigned int size=varDb_.size();
                if(size>1)
                {
                    var->setTmp(tmp);
                    varDb_[size-2]->insert(new Vargen(var));
                }
                if(var->type->name.compare(stringType)==0)
                {
                    cont=var->valStr;
                }
                else
                {
                    Erreur("Le champs contenu de NewConst n'est pas de type string",context);
                }
            }
            else
            {
                Erreur("Le champs contenu de NewConst est NULL",context);
            }
        }
        else
        {
            Erreur("Le champs contenu de NewConst return NULL",context);
        }
    }


    if(nameType.compare("")!=0)
    {
        string argT="";
        if(arg.compare("")!=0 && cont.compare("")==0)
        {
            vector<Vargen*> argVar=makeArgVar(arg);
            unsigned int k;
            for(k=0;k<argVar.size();k++)
            {                
                if(k!=argVar.size()-1)
                {
                    argT+=argVar[k]->type->name+",";
                }
                else
                {
                    argT+=argVar[k]->type->name;
                }
                delete argVar[k];
            }
            argVar.clear();
        }
        string nameInst=nameType+"::"+nameType;
        Type* type=new Type(nameType);
        int num=getMaxTable(nameType);
        if(num>=0)
        {
            num++;
            stringstream ss;
            string idNum="";
            ss<<num;
            ss>>idNum;
            string exCont=cont;
            string nameVar=nameType+"_"+idNum;
            exCont=transfContMeth(nameVar,cont,type->cont);
            cont="NewVar(\""+nameVar+"\",\""+nameType+"\",\"\",false);"+exCont+"Return "+nameVar+";";
            Instruction* inst=new Instruction(nameInst,arg,nameType+";",cont,varDb_,type->tmp);
            //cout<<"id newInst="<<inst->id<<endl;
            inst->print();
            if(inst->id.compare("")!=0)
            {
                stringstream ss1;
                unsigned int idInt;
                ss1<<inst->id;
                ss1>>idInt;
                type->addMeth(idInt);
            }
            else
            {
                Erreur("L'instruction "+nameInst+"("+argT+") n'a pas été ajoutée à la base de données",context);
            }
            delete inst;
        }
        else
        {
            cout<<"getMaxTable="<<num<<endl;
            Erreur("L'instruction "+nameInst+"("+argT+") n'a pas été ajoutée à la base de données",context);
        }
        delete type;        
    }

}





