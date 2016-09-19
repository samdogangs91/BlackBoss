/* 
 * File:   Instruction.cpp
 * Author: mblack
 * 
 * Created on 23 février 2016, 18:37
 */

#include "Instruction.h"
#include "Database.h"
#include <sstream>

using namespace std;

string Arg_="?";
string If_="If";
string Else_="Else";
string Set_="=";
string For_="For";
string While_="While";
string NewVar_="NewVar";
//string Bool_="Bool";
string Prog="Program";
string Return_="Return";
string Identity_="Identity";
string DeleteVar_="DeleteVar";
string NewInst_="NewInst";
string NewType_="NewType";
string AddAtt_="AddAtt";
string AddMeth_="AddMeth";
string ModifAtt_="ModifAtt";
string DeleteAtt_="DeleteAtt";
string DeleteMeth_="DeleteMeth";


//operateur pour les nombres
string SupEqual_=">=";
string Sup_=">";
string InfEqual_="<=";
string Inf_="<";
string Equal_="==";
string Diff_="!=";
string Incr_="++";
string Decr_="--";
string PlusEqual_="+=";
string Plus_="+";
string MoinsEqual_="-=";
string Moins_="-";
string Mult_="*";
string MultEqual_="*=";
string Div_="/";
string DivEqual_="/=";
string Reste_="%";

//operateur d'accès
string Cro_="[";
string Fleche_="->";
string Point_=".";
string In_=">>";
string Out_="<<";

//opérateur pour les booléens
string And_="&&";
string Or_="||";
string Neg_="!";

extern string stringType;
extern string intType;
extern string charType;
extern string boolType;
extern string floatType;
extern Database* memory;

extern string Continue_;
extern string Break_;
extern string Erreur_;
extern string Retour_;
extern string signalType;

vector<DbVar*> context;


bool isKeyWord(string name)
{
    return (Arg_.compare(name)==0)||(If_.compare(name)==0)||(Else_.compare(name)==0)||(Set_.compare(name)==0)||(For_.compare(name)==0)||(While_.compare(name)==0)||(Prog.compare(name)==0)||(NewVar_.compare(name)==0)||(Return_.compare(name)==0)||(Identity_.compare(name)==0)||(DeleteVar_.compare(name)==0)||(NewInst_.compare(name)==0)||(NewType_.compare(name)==0)||(AddAtt_.compare(name)==0)||(AddMeth_.compare(name)==0)||(ModifAtt_.compare(name)==0)||(DeleteAtt_.compare(name)==0)||(DeleteMeth_.compare(name)==0)||(Incr_.compare(name)==0)||(Decr_.compare(name)==0)||(SupEqual_.compare(name)==0)||(Sup_.compare(name)==0)||(InfEqual_.compare(name)==0)||(Inf_.compare(name)==0)||(Equal_.compare(name)==0)||(Diff_.compare(name)==0)||(PlusEqual_.compare(name)==0)||(Plus_.compare(name)==0)||(Moins_.compare(name)==0)||(MoinsEqual_.compare(name)==0)||(Mult_.compare(name)==0)||(MultEqual_.compare(name)==0)||(Div_.compare(name)==0)||(DivEqual_.compare(name)==0)||(Reste_.compare(name)==0)||(Cro_.compare(name)==0)||(Point_.compare(name)==0)||(Fleche_.compare(name)==0)||(In_.compare(name)==0)||(Out_.compare(name)==0)||(And_.compare(name)==0)||(Or_.compare(name)==0)||(Neg_.compare(name)==0);
}



/*
 * renvoie true si le caractère c peut être dans le nom d'une variable ou d'une methode non opérateur
 */
bool isNotSpe(char c)
{
    return (c>='A' && c<='Z')||(c>='a' && c<='z')||(c=='_')||(c==':');
}

/*
 * retourne la string correspondant à l'id dans la base de données
 */
string getStringId(string id)
{
    string ret="";
    string req="select cont from string inner join list_char on string.string_id=list_char.list_char_id where string_id="+id+";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            ret=(row[0]? row[0]: "");
        }
        else
        {
            Erreur("le string_id:"+id+" n'existe pas",context);
        }
    }
    else
    {
        cout<<"Erreur requete sql:"<<req<<endl;
    }
    return ret;
}

string getIdString(string cont)
{
    string id="";
    string req="select string_id from string inner join list_char on string.string_id=list_char.list_char_id where cont=\""+cont+"\";";
    //cout<<"req="<<req<<endl;
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            id=(row[0]? row[0]: "");
        }
        else
        {
            //Erreur("la string: \""+cont+"\" n'existe pas",context);
        }
    }
    else
    {
        cout<<"Erreur requete sql:"<<req<<endl;
    }
    return id;
}

/*
 *string de la forme "type1;type2;...;"
 */
vector<Type*> makeArg(string cont)
{
    vector<Type*> ret;
    unsigned int k=0;
    string nameT="";//nom du type
    for(k=0;k<cont.size();k++)
    {
        if(cont[k]!=';') nameT+=cont[k];
        else
        {
            if(isBasic(nameT) || nameT.compare("string")==0)
            {
               ret.push_back(new Type(nameT,true));
            }
            else
            {
                ret.push_back(new Type(nameT));
            }
            nameT="";
        }
    }
    return ret;
}

/*
 * ordonne les instructions par priorité
 */
vector<Instruction*> orderInst(vector<Instruction*> _inst)
{
    vector<Instruction*> ret;
    while(_inst.size()>0)
    {
        unsigned int k;
        unsigned int pos=0;
        unsigned int prior=20;
        for(k=0;k<_inst.size();k++)
        {
            if(_inst[k]->prior<prior)
            {
                pos=k;
                prior=_inst[k]->prior;
            }
        }
        ret.push_back(_inst[pos]);
        _inst.erase(_inst.begin()+pos);
    }
    return ret;
}

/*
 * return true si c peut etre un operateur
 */
bool isOpe(char c)
{
    return (c=='+')||(c=='-')||(c=='*')||(c=='∕')||(c=='=')||(c=='!')||(c=='>')||(c=='<')||(c=='%')||(c=='|')||(c=='&')||(c==';')||(c=='.');
}



/*
 * return true si i est dans le tableau
 */
bool isIntab(int i,vector<int> tab)
{
    unsigned int k;
    for(k=0;k<tab.size();k++)
    {
        if(tab[k]==i) return true;
    }
    return false;
}

/*
 * return l'indice du name dans la string
 */
int indexInString(string name, string s, vector<int> index, bool isRight, unsigned int nbAtten)
{
    unsigned int i=0;
    //cout<<"tab size: "<<index.size()<<endl;
    unsigned int nbVu=0;
    bool isInBra_=false;
    bool isInQuo_=false;
    int numPar=0;
    if(name.size()>1)
    {
        for(i=0;i<s.size()-1;i++)
        {
           unsigned int nbCom=0;
           unsigned int l;
           if(s[i]=='"' && !isInQuo_)
           {
               if(i>0)
               {
                   if(s[i-1]!='\\')
                   {
                      isInBra_=!isInBra_;
                   }
               }
               else
               {
                   isInBra_=!isInBra_;
               }
           }
           if(s[i]=='\'' && !isInBra_)
           {
               if(i>0)
               {
                   if(s[i-1]!='\\')
                   {
                      isInQuo_=!isInQuo_;
                   }
               }
               else
               {
                   isInQuo_=!isInQuo_;
               }
           }
           if(isInQuo_||isInBra_)
           {
               continue;
           }
           if(s[i]=='(')
           {
               numPar++;
           }
           if(s[i]==')')
           {
               numPar--;
           }
           if(isRight && (numPar>0))
           {
               continue;
           }
           for(l=0;l<name.size();l++)
           {               
               if(isIntab(i,index))
               {
                   break;
               }
               if(i+l<s.size())
               {
                   if(s[i+l]==name[l])
                   {
                       nbCom++;
                   }
                   else break;
               }
           }
           if(nbCom==name.size())
           {
               nbVu++;
           }
           if(nbVu==nbAtten)
           {
               break;
           }

        }
    }
    else if(name.size()==1)
    {
        char op=name[0];
        for(i=0;i<s.size();i++)
        {
            if(s[i]=='"' && !isInQuo_)
            {
                if(i>0)
                {
                    if(s[i-1]!='\\')
                    {
                       isInBra_=!isInBra_;
                    }
                }
                else
                {
                    isInBra_=!isInBra_;
                }
            }
            if(s[i]=='\'' && !isInBra_)
            {
                if(i>0)
                {
                    if(s[i-1]!='\\')
                    {
                       isInQuo_=!isInQuo_;
                    }
                }
                else
                {
                    isInQuo_=!isInQuo_;
                }
            }
            if(isInQuo_||isInBra_)
            {
                continue;
            }
            if(s[i]=='(')
            {
                numPar++;
            }
            if(s[i]==')')
            {
                numPar--;
            }
            if(isRight && (numPar>0))
            {
                continue;
            }
            if(isIntab(i,index))
            {
                continue;
            }
            if(s[i]==op)
            {
                if(i>0 && i<s.size()-1)
                {
                    if(!isOpe(s[i-1]) && !isOpe(s[i+1]))
                    {
                        nbVu++;
                    }
                }
                else if(i==0 && op=='!' && s.size()>0)
                {
                    if(!isOpe(s[1]))
                    {
                        nbVu++;
                    }
                }
            }
            if(nbVu==nbAtten)
            {
                break;
            }
        }
    }
    //index->push_back(i+1);
    //int size=index.size();
    //cout<<"in tab: "<<index[size-1]<<endl;
    return i;
}

string parenth(string s)
{

    string ret=s;
    unsigned int k=0;
    string actualMot="";
    int numPar=0;
    string actualFun="";
    vector<Instruction*> inst;
    vector<int> tab;
    bool isInQuo_=false;
    bool isInBra_=false;
    for(k=0;k<s.size();k++)
    {
        if(s[k]=='"' && !isInQuo_)
        {
            if(k>0)
            {
                if(s[k-1]!='\\')
                {
                    isInBra_=!isInBra_;
                }
            }
            else
            {
                isInBra_=!isInBra_;
            }
        }
        if(s[k]=='\'' && !isInBra_)
        {
            if(k>0)
            {
                if(s[k-1]!='\\')
                {
                    isInQuo_=!isInQuo_;
                }
            }
            else
            {
                isInQuo_=!isInQuo_;
            }
        }
        if(isInQuo_||isInBra_)
        {
            continue;
        }
        if(isNotSpe(s[k]))
        {
            actualMot+=s[k];
            //cout<<"isNotSpe: "<<s[k]<<endl;
        }
        else if(s[k]=='(')
        {
            numPar++;
            if(actualMot.compare("")!=0)
            {
                actualFun=actualMot;
                actualMot="";
            }
        }
        else if(s[k]==')')
        {
            actualMot="";
            numPar--;
        }
        else
        {
            if(k<s.size()-1)
            {
                if(s[k]=='+' && s[k+1]=='+')
                {
                    actualFun="++";
                    k++;
                }
                else if(s[k]=='-' && s[k+1]=='-')
                {
                    actualFun="--";
                    k++;
                }
                else if(s[k]=='=' && s[k+1]=='=')
                {
                    actualFun="==";
                    k++;
                }
                else if(s[k]=='!' && s[k+1]=='=')
                {
                    actualFun="!=";
                    k++;
                }
                else if(s[k]=='+' && s[k+1]=='=')
                {
                    actualFun="++";
                    k++;
                }
                else if(s[k]=='-' && s[k+1]=='=')
                {
                    actualFun="-=";
                    k++;
                }
                else if(s[k]=='*' && s[k+1]=='=')
                {
                    actualFun="*=";
                    k++;
                }
                else if(s[k]=='/' && s[k+1]=='=')
                {
                    actualFun="/=";
                    k++;
                }
                else if(s[k]=='>' && s[k+1]=='=')
                {
                    actualFun=">=";
                    k++;
                }
                else if(s[k]=='<' && s[k+1]=='=')
                {
                    actualFun="<=";
                    k++;
                }
                else if(s[k]=='-' && s[k+1]=='>')
                {
                    actualFun="->";
                    k++;
                }
                else if(s[k]=='[')
                {
                    actualFun="[]";
                }
                else if(s[k]=='&' && s[k+1]=='&')
                {
                    actualFun="&&";
                    k++;
                }
                else if(s[k]=='|' && s[k+1]=='|')
                {
                    actualFun="||";
                    k++;
                }
                else if(s[k]=='<' && s[k+1]=='<')
                {
                    actualFun="<<";
                    k++;
                }
                else if(s[k]=='>' && s[k+1]=='>')
                {
                    actualFun=">>";
                    k++;
                }
            }
            if(k<s.size() && actualFun.compare("")==0)
            {
                if(s[k]=='.')
                {
                    actualFun=".";
                }
                else if(s[k]=='=')
                {
                    actualFun="=";
                }
                else if(s[k]=='!')
                {
                    actualFun="!";
                }
                else if(s[k]=='+')
                {
                    actualFun="+";
                }
                else if(s[k]=='-')
                {
                    actualFun="-";
                }
                else if(s[k]=='*')
                {
                    actualFun="*";
                }
                else if(s[k]=='/')
                {
                    actualFun="/";
                }
                else if(s[k]=='%')
                {
                    actualFun="%";
                }
                else if(s[k]=='>')
                {
                    actualFun=">";
                }
                else if(s[k]=='<')
                {
                    actualFun="<";
                }
            }
        }
        if(actualFun.compare("")!=0)
        {
            //cout<<"instruction: "<<actualFun<<endl;
            string idFun=getIdInstruction(actualFun);
            //cout<<"idFun:"<<idFun<<endl;
            inst.push_back(new Instruction(idFun));
            actualFun="";
            actualMot="";
        }
    }

    vector<Instruction*> tmp=orderInst(inst);
    inst=tmp;
    if(inst.size()>1)
    {
        unsigned k=0;

        while(inst.size()>1)
        {
            s=ret;
            if(k<inst.size()-1)
            {
                ret="";
                string name;
                bool isInst1=false;
                bool isInst2=false;
                bool isEqual=false;
                bool isLeft=false;
                bool isRight=true;
                bool isOp=false;
                bool sameName=false;
                bool isInQuo=false;
                bool isInBra=false;
                if(inst[k]->prior> inst[k+1]->prior)
                {
                    isInst1=true;
                    name=inst[k+1]->name;
                    isOp=inst[k+1]->isOp;
                    inst.erase(inst.begin()+1);
                    //cout<<"plus fort à droite: name= "<<name<<endl;
                }
                else if(inst[k]->prior==inst[k+1]->prior)
                {
                    isEqual=true;
                    if(inst[k]->assoc.compare("gauche")==0)
                    {                        
                        name=inst[k+1]->name;
                        isOp=inst[k+1]->isOp;
                        inst.erase(inst.begin()+1);
                        sameName=(inst[k]->name.compare(inst[k+1]->name)==0);
                        isLeft=true;
                        //cout<<"isEqual, assoc gauche et name="<<name<<endl;
                    }
                    else
                    {
                        name=inst[k]->name;
                        isOp=inst[k]->isOp;
                        inst.erase(inst.begin());
                        //cout<<"isEqual, assoc droite et name="<<name<<endl;
                        isRight=true;
                    }
                }
                else if(inst[k]->prior < inst[k+1]->prior)
                {
                    isInst2=true;
                    name=inst[k]->name;
                    isOp=inst[k]->isOp;
                    inst.erase(inst.begin());
                    //cout<<"plus fort à gauche: name= "<<name<<endl;
                }
                if(isOp)
                {
                   if(name.compare("++")==0 || name.compare("--")==0)
                   {
                       unsigned int i;
                       if(sameName)
                       {
                           i=indexInString(name,s,tab,isRight,2);
                       }
                       else
                       {
                           i=indexInString(name,s,tab,isRight);
                       }
                       tab.push_back(i+1);
                       int j;
                       int numPar2=0;
                       if(i>0)
                       {
                           for(j=i-1;j>=0;j--)
                           {
                               if(s[j]=='"' && !isInQuo)
                               {
                                   if(j>0)
                                   {
                                       if(s[j-1]!='\\')
                                       {
                                           isInBra=!isInBra;
                                       }
                                   }
                                   else
                                   {
                                       isInBra=!isInBra;
                                   }
                               }
                               if(s[j]=='\'' && !isInBra)
                               {
                                   if(j>0)
                                   {
                                       if(s[j-1]!='\\')
                                       {
                                           isInQuo=!isInQuo;
                                       }
                                   }
                                   else
                                   {
                                       isInQuo=!isInQuo;
                                   }
                               }
                               if(isInQuo||isInBra)
                               {
                                   continue;
                               }
                               if(s[j]==')')
                               {
                                   numPar2--;
                               }
                               if(s[j]=='(' && numPar2==-1)
                               {
                                   numPar2++;
                                   continue;
                               }
                               if(s[j]=='(')
                               {
                                   numPar2++;
                               }
                               if(/*!isNotSpe(s[j])*/ isOpe(s[j]) && numPar2==0) //si le caractère ne peut pas être dans une variable
                               {
                                   break;
                               }
                           }
                           unsigned int l;
                           for(l=0;l<s.size();l++)
                           {
                               if(l==j+1)
                               {
                                   ret+='(';
                               }
                               else if(l==i+2)
                               {
                                   ret+=')';
                               }
                               ret+=s[l];
                           }
                           if((i+2)==s.size())
                           {
                               ret+=')';
                           }

                       }
                   }
                   else if(name.compare("!")==0)
                   {
                       unsigned int i;
                       if(sameName)
                       {
                           i=indexInString(name,s,tab,isRight,2);
                       }
                       else
                       {
                           i=indexInString(name,s,tab,isRight);
                       }
                       tab.push_back(i+1);
                       unsigned int l;
                       int numPar2=0;
                       bool ok=false;
                       if(i>0)
                       {
                           for(l=0;l<s.size();l++)
                           {
                               if(l==i-1)
                               {
                                   ret+='(';
                               }
                               if(l>i && !ok)
                               {
                                   if(s[l]=='"' && !isInQuo)
                                   {
                                       if(l>0)
                                       {
                                           if(s[l-1]!='\\')
                                           {
                                               isInBra=!isInBra;
                                           }
                                       }
                                       else
                                       {
                                           isInBra=!isInBra;
                                       }
                                   }
                                   if(s[l]=='\'' && !isInBra)
                                   {
                                       if(l>0)
                                       {
                                           if(s[l-1]!='\\')
                                           {
                                               isInQuo=!isInQuo;
                                           }
                                       }
                                       else
                                       {
                                           isInQuo=!isInQuo;
                                       }
                                   }
                                   if(isInQuo||isInBra)
                                   {
                                       ret+=s[l];
                                       continue;
                                   }
                                   if(s[l]=='(')
                                   {
                                       numPar2++;
                                   }
                                   if(s[l]==')' && numPar2==1)
                                   {
                                       numPar2--;
                                       ret+=')';
                                       continue;
                                   }
                                   if(s[l]==')')
                                   {
                                       numPar2++;
                                   }
                                   if(/*!isNotSpe(s[l])*/ isOpe(s[l]) && numPar2==0)
                                   {
                                       ret+=')';
                                       ok=true;
                                   }
                               }
                               ret+=s[l];
                           }
                           if(!ok)
                           {
                               ret+=')';
                           }
                       }
                       else
                       {
                           if(isInst1 || (isEqual && !isLeft))
                           {
                               ret+='(';
                               for(l=0;l<s.size();l++)
                               {
                                   if(l>i && !ok)
                                   {
                                       if(s[l]=='"' && !isInQuo)
                                       {
                                           if(l>0)
                                           {
                                               if(s[l-1]!='\\')
                                               {
                                                   isInBra=!isInBra;
                                               }
                                           }
                                           else
                                           {
                                               isInBra=!isInBra;
                                           }
                                       }
                                       if(s[l]=='\'' && !isInBra)
                                       {
                                           if(l>0)
                                           {
                                               if(s[l-1]!='\\')
                                               {
                                                   isInQuo=!isInQuo;
                                               }
                                           }
                                           else
                                           {
                                               isInQuo=!isInQuo;
                                           }
                                       }
                                       if(isInQuo||isInBra)
                                       {
                                           ret+=s[l];
                                           continue;
                                       }
                                       if(isInQuo||isInBra)
                                       {
                                           ret+=s[l];
                                           continue;
                                       }
                                       if(s[l]=='(')
                                       {
                                           numPar2++;
                                       }
                                       if(s[l]==')' && numPar2==1)
                                       {
                                           numPar2--;
                                           ret+=')';
                                           continue;
                                       }
                                       if(s[l]==')')
                                       {
                                           numPar2++;
                                       }
                                       if(/*!isNotSpe(s[l])*/ isOpe(s[l]) && numPar2==0)
                                       {
                                           ret+=')';
                                           ok=true;
                                       }
                                   }
                                   ret+=s[l];
                               }
                               if(!ok)
                               {
                                   ret+=')';
                               }
                           }
                       }
                   }
                   else if(name.compare("[]")==0)
                   {
                       unsigned int i;
                       if(sameName)
                       {
                           i=indexInString("[",s,tab,isRight,2);
                       }
                       else
                       {
                           i=indexInString("[",s,tab,isRight);
                       }
                       //cout<<"indexInString: "<<i<<endl;
                       tab.push_back(i+1);
                       int j;
                       int nbCro=0;
                       if(i>0)
                       {
                           int numPar2=0;
                           for(j=i-1;j>=0;j--) //j aura pour valeur l'indice juste avant le début du premier argument
                           {
                               if(s[j]=='"' && !isInQuo)
                               {
                                   if(j>0)
                                   {
                                       if(s[j-1]!='\\')
                                       {
                                           isInBra=!isInBra;
                                       }
                                   }
                                   else
                                   {
                                       isInBra=!isInBra;
                                   }
                               }
                               if(s[j]=='\'' && !isInBra)
                               {
                                   if(j>0)
                                   {
                                       if(s[j-1]!='\\')
                                       {
                                           isInQuo=!isInQuo;
                                       }
                                   }
                                   else
                                   {
                                       isInQuo=!isInQuo;
                                   }
                               }
                               if(isInQuo||isInBra)
                               {
                                   continue;
                               }
                               if(s[j]==')')
                               {
                                   numPar2--;
                               }
                               if(s[j]=='(' && numPar2==-1)
                               {
                                   numPar2++;
                                   continue;
                               }
                               if(s[j]=='(')
                               {
                                   numPar2++;
                               }
                               if(s[j]==']')
                               {
                                   nbCro--;
                               }
                               if(s[j]=='[' && nbCro==-1)
                               {
                                   nbCro++;
                                   continue;
                               }
                               if(s[j]=='[')
                               {
                                   nbCro++;
                               }
                               if(isOpe(s[j]) && numPar2==0 && nbCro==0) //si le caractère ne peut pas être dans une variable
                               {
                                   break;
                               }
                           }
                           unsigned int l;
                           int nbCro=0;
                           bool ok=false;
                           int nbOk=0;
                           ret=s.substr(0,j+1);
                           for(l=j+1;l<s.size();l++)
                           {
                               if(ok && nbOk==0)
                               {
                                   ret+=')';
                                   nbOk++;
                               }
                               if(l==j+1)
                               {
                                   ret+='(';
                               }
                               if(s[l]=='[')
                               {
                                   nbCro++;
                               }
                               if(s[l]==']' && nbCro==1)
                               {
                                   ok=true;
                               }
                               else if(s[l]==']')
                               {
                                   nbCro--;
                               }
                               ret+=s[l];
                           }
                           if(nbOk==0)
                           {
                               ret+=')';
                           }
                       }

                   }
                   else
                   {
                       unsigned int i;
                       if(sameName)
                       {
                           i=indexInString(name,s,tab,isRight,2);
                       }
                       else
                       {
                           i=indexInString(name,s,tab,isRight);
                       }
                        tab.push_back(i+1);
                        //int size=tab.size();
                        //tab.pop_back();
                        //cout<<"tab size: "<<tab.size()<<endl;
                        //int val=i+1;
                        //tab.push_back(val);
                        //cout<<"indexInString: "<<i<<endl;
                        //tab[size-1]=i+1;
                        if(i>0)
                        {
                            int j=0;
                            int numPar2=0;
                            int nbCro=0;
                            bool isInQuo=false;
                            bool isInBra=false;
                            for(j=i-1;j>=0;j--) //j est l'indice juste avant le debut de l' argument 1
                            {
                                if(s[j]=='"' && !isInQuo)
                                {
                                    if(j>0)
                                    {
                                        if(s[j-1]!='\\')
                                        {
                                            isInBra=!isInBra;
                                        }
                                    }
                                    else
                                    {
                                        isInBra=!isInBra;
                                    }
                                }
                                if(s[j]=='\'' && !isInBra)
                                {
                                    if(j>0)
                                    {
                                        if(s[j-1]!='\\')
                                        {
                                            isInQuo=!isInQuo;
                                        }
                                    }
                                    else
                                    {
                                        isInQuo=!isInQuo;
                                    }
                                }
                                if(isInQuo||isInBra)
                                {
                                    continue;
                                }
                                if(s[j]==')'&& !isInQuo && !isInBra)
                                {
                                    numPar2--;
                                }
                                if(s[j]=='(' && numPar2==-1 && !isInQuo && !isInBra)
                                {
                                    numPar2++;
                                    continue;
                                }
                                if(s[j]=='(' && !isInQuo && !isInBra)
                                {
                                    numPar2++;
                                }
                                if(s[j]==']')
                                {
                                    nbCro--;
                                }
                                if(s[j]=='[' && nbCro==-1 && !isInQuo && !isInBra)
                                {
                                    nbCro++;
                                    continue;
                                }
                                if(s[j]=='['&& !isInQuo && !isInBra)
                                {
                                    nbCro++;
                                }                                
                                if(isOpe(s[j]) && numPar2==0 && nbCro==0 && j!=(i-1) && !isInQuo && !isInBra) //si le caractère ne peut pas être dans une variable
                                {
                                    break;
                                }
                            }
                            //cout<<"j="<<j<<endl;
                            unsigned int m;
                            numPar2=0;
                            nbCro=0;
                            isInQuo=false;
                            isInBra=false;
                            //cout<<"s="<<s<<endl;
                            for(m=i+name.size();m<s.size();m++) //m aura pour valeur l'indice juste après la fin du deuxième argument
                            {
                                if(s[m]=='"' && !isInQuo)
                                {
                                    //cout<<"isInBra="<<boolalpha<<isInBra<<endl;
                                    if(m>0)
                                    {
                                        if(s[m]!='\\')
                                        {
                                            isInBra=!isInBra;
                                        }
                                    }
                                    else
                                    {
                                        isInBra=!isInBra;
                                    }
                                    if(!isInBra)
                                    {
                                        continue;
                                    }
                                }
                                if(s[m]=='\'' && !isInBra)
                                {
                                    if(m>0)
                                    {
                                        if(s[m]!='\\')
                                        {
                                            isInQuo=!isInQuo;
                                        }
                                    }
                                    else
                                    {
                                        isInQuo=!isInQuo;
                                    }
                                   if(!isInQuo) //si on est pas dans une quote
                                   {
                                       continue;
                                   }
                                }
                                if(s[m]=='('&& !isInQuo && !isInBra)
                                {
                                    numPar2++;
                                }
                                if(s[m]==')' && numPar2==1 && !isInQuo && !isInBra)
                                {
                                    numPar2--;
                                    continue;
                                }
                                if(s[m]==')' && !isInQuo && !isInBra)
                                {
                                    numPar2--;
                                }
                                if(s[m]=='[')
                                {
                                    nbCro++;
                                }
                                if(s[m]==']' && nbCro==1 && !isInQuo && !isInBra)
                                {
                                    nbCro--;
                                    continue;
                                }
                                if(s[m]==']' && !isInQuo && !isInBra)
                                {
                                    nbCro--;
                                }
                                if(isOpe(s[m]) && numPar2==0 && nbCro==0 && m!=i+1 && !isInQuo && !isInBra) //si le caractère ne peut pas être dans une variable
                                {
                                    break;
                                }
                            }
                            unsigned int l;
                            //cout<<"avant for"<<endl;
                            for(l=0;l<s.size();l++)
                            {
                                if(l==j+1) //indice début d'argument 1
                                {
                                   ret+='(';
                                }
                                if(l==m) //indice après la fin de l'argument 2
                                {
                                    ret+=')';
                                }
                                ret+=s[l];
                            }
                            if(m==s.size())
                            {
                                ret+=')';
                            }
                            //cout<<"ret="<<ret<<endl;
                            //cout<<"retour: "<<ret<<endl;

                        }
                   }
                }
                else //cas des instructions générales
                {
                    unsigned int i;
                    if(sameName)
                    {
                        i=indexInString(name,s,tab,isRight,2);
                    }
                    else
                    {
                        i=indexInString(name,s,tab,isRight);
                    }
                    unsigned int j=0;
                    if(i>0)
                    {
                        for(j=0;j<i;j++)
                        {
                            ret+=s[j];
                        }
                        ret+='(';
                        ret+=name;
                        ret+='(';
                        int numPar2=0;
                        for(j=i+name.size();j<s.size();j++)
                        {
                            ret+=s[j];
                            if(s[j]=='"' && !isInQuo)
                            {
                                if(j>0)
                                {
                                    if(s[j-1]!='\\')
                                    {
                                        isInBra=!isInBra;
                                    }
                                }
                                else
                                {
                                    isInBra=!isInBra;
                                }
                            }
                            if(s[j]=='\'' && !isInBra)
                            {
                                if(j>0)
                                {
                                    if(s[j-1]!='\\')
                                    {
                                        isInQuo=!isInQuo;
                                    }
                                }
                                else
                                {
                                    isInQuo=!isInQuo;
                                }
                            }
                            if(isInQuo||isInBra)
                            {
                                continue;
                            }
                            if(s[j]==')' && numPar2==0)
                            {
                                ret+=')';
                                continue;
                            }
                            else if(s[j]=='(')
                            {
                                numPar2++;
                            }
                            else if(s[j]==')')
                            {
                                numPar2--;
                            }
                        }
                    }
                    else
                    {
                        if(isInst1 || (isEqual && !isLeft))
                        {
                            ret+='(';
                            ret+=name;
                            ret+='(';
                            int numPar2=0;
                            for(j=i+name.size();j<s.size();j++)
                            {
                                ret+=s[j];
                                if(s[j]=='"' && !isInQuo)
                                {
                                    if(j>0)
                                    {
                                        if(s[j-1]!='\\')
                                        {
                                            isInBra=!isInBra;
                                        }
                                    }
                                    else
                                    {
                                        isInBra=!isInBra;
                                    }
                                }
                                if(s[j]=='\'' && !isInBra)
                                {
                                    if(j>0)
                                    {
                                        if(s[j-1]!='\\')
                                        {
                                            isInQuo=!isInQuo;
                                        }
                                    }
                                    else
                                    {
                                        isInQuo=!isInQuo;
                                    }
                                }
                                if(isInQuo||isInBra)
                                {
                                    continue;
                                }
                                if(s[j]==')' && numPar2==0)
                                {
                                    ret+=')';
                                    continue;
                                }
                                else if(s[j]=='(')
                                {
                                    numPar2++;
                                }
                                else if(s[j]==')')
                                {
                                    numPar2--;
                                }
                            }
                        }
                    }

                }

                //cout<<"parenth: "<<ret<<endl;
            }

        }
    }
    //cout<<"retour: "<<ret<<endl;
    inst.clear();
    return ret;
}



bool isWellPar(string s, unsigned int _nbFun)
{
    //cout<<"isWellParenth("<<s<<") ?"<<endl;
    bool ret=(_nbFun<=1);
    unsigned int nbFun=_nbFun;
    string actualMot="";
    string actualFun="";
    int numPar=0;
    bool isInQuo_=false;
    bool isInBra_=false;
    if(s.size()>0)
    {
        unsigned int k=0;
        for(k=0;k<s.size();k++)
        {
            if(s[k]=='"')
            {
                isInQuo_=!isInQuo_;
            }
            if(s[k]=='\'')
            {
                isInBra_=!isInBra_;
            }
            if(isInQuo_||isInBra_)
            {
                continue;
            }
            if(isNotSpe(s[k]))
            {
                actualMot+=s[k];
                //cout<<"isNotSpe: "<<s[k]<<endl;
            }
            else if(s[k]=='(')
            {
                numPar++;
                if(actualMot.compare("")!=0)
                {
                    actualFun=actualMot;
                    nbFun++;
                    actualMot="";
                    string tmp="";
                    unsigned int l;
                    int numPar2=1;
                    bool ok2=false;
                    if(k<s.size()-1)
                    {
                        for(l=k+1;l<s.size();l++)
                        {
                            if(ok2)
                            {
                                tmp+=s[l];
                            }
                            else
                            {
                                if(s[l]=='(')
                                {
                                    numPar2++;
                                }
                                if(s[l]==')' && numPar2==1)
                                {
                                    numPar2--;
                                    ok2=true;
                                    continue;
                                }
                            }
                        }
                    }
                    else return false;
                    return (nbFun<=1)&&isWellPar(tmp,nbFun);
                }
                else
                {
                    string tmpIn="";
                    string tmpOut="";
                    unsigned int l;
                    int numPar2=1;
                    bool ok2=false;
                    if(k<s.size()-1)
                    {
                        for(l=k+1;l<s.size();l++)
                        {
                            if(ok2)
                            {
                                tmpOut+=s[l];
                            }
                            else
                            {
                                tmpIn+=s[l];
                                if(s[l]=='(')
                                {
                                    numPar2++;
                                }
                                if(s[l]==')' && numPar2==1)
                                {
                                    numPar2--;
                                    ok2=true;
                                    continue;
                                }
                            }
                        }
                    }
                    else return false;
                    return (nbFun<=1)&&isWellPar(tmpIn)&&isWellPar(tmpOut,nbFun);
                }
            }
            else if(s[k]==')')
            {
                numPar--;
                actualMot="";
            }
            else
            {
                if(k<s.size()-1)
                {
                    if(s[k]=='+' && s[k+1]=='+')
                    {
                        actualFun="++";
                        k++;
                        continue;
                    }
                    else if(s[k]=='-' && s[k+1]=='-')
                    {
                        actualFun="--";
                        k++;
                        continue;
                    }
                    else if(s[k]=='=' && s[k+1]=='=')
                    {
                        actualFun="==";
                        k++;
                        continue;
                    }
                    else if(s[k]=='!' && s[k+1]=='=')
                    {
                        actualFun="!=";
                        k++;
                        continue;
                    }
                    else if(s[k]=='+' && s[k+1]=='=')
                    {
                        actualFun="++";
                        k++;
                        continue;
                    }
                    else if(s[k]=='-' && s[k+1]=='=')
                    {
                        actualFun="-=";
                        k++;
                        continue;
                    }
                    else if(s[k]=='*' && s[k+1]=='=')
                    {
                        actualFun="*=";
                        k++;
                        continue;
                    }
                    else if(s[k]=='/' && s[k+1]=='=')
                    {
                        actualFun="/=";
                        k++;
                        continue;
                    }
                    else if(s[k]=='>' && s[k+1]=='=')
                    {
                        actualFun=">=";
                        k++;
                        continue;
                    }
                    else if(s[k]=='<' && s[k+1]=='=')
                    {
                        actualFun="<=";
                        k++;
                        continue;
                    }
                    else if(s[k]=='-' && s[k+1]=='>')
                    {
                        actualFun="->";
                        k++;
                        continue;
                    }
                    else if(s[k]=='[')
                    {
                        actualFun="[]";
                        k++;
                        continue;
                    }
                    else if(s[k]=='&' && s[k+1]=='&')
                    {
                        actualFun="&&";
                        k++;
                        continue;
                    }
                    else if(s[k]=='|' && s[k+1]=='|')
                    {
                        actualFun="||";
                        k++;
                        continue;
                    }
                    else if(s[k]=='<' && s[k+1]=='<')
                    {
                        actualFun="<<";
                        k++;
                        continue;
                    }
                    else if(s[k]=='>' && s[k+1]=='>')
                    {
                        actualFun=">>";
                        k++;
                        continue;
                    }
                }
                if(k<s.size())
                {
                    if(s[k]=='.')
                    {
                        actualFun=".";
                        continue;
                    }
                    else if(s[k]=='=')
                    {
                        actualFun="=";
                        continue;
                    }
                    else if(s[k]=='!')
                    {
                        actualFun="!";
                        continue;
                    }
                    else if(s[k]=='+')
                    {
                        actualFun="+";
                        continue;
                    }
                    else if(s[k]=='-')
                    {
                        actualFun="-";
                        continue;
                    }
                    else if(s[k]=='*')
                    {
                        actualFun="*";
                        continue;
                    }
                    else if(s[k]=='/')
                    {
                        actualFun="/";
                        continue;
                    }
                    else if(s[k]=='%')
                    {
                        actualFun="%";
                        continue;
                    }
                    else if(s[k]=='>')
                    {
                        actualFun=">";
                        continue;
                    }
                    else if(s[k]=='<')
                    {
                        actualFun="<";
                        continue;
                    }
                }
            }
            if(actualFun.compare("")!=0)
            {
                //cout<<"instruction: "<<actualFun<<endl;
                nbFun++;
                actualFun="";
                actualMot="";
            }
        }
    }
    ret=(nbFun<=1);
    return ret;
}


string uselessPar(string s)
{
    int size=s.size();
    unsigned int k=0;
    int numPar=0;
    if(size>0)
    {
        if(s[0]=='(')
        {
            for(k=0;k<s.size();k++)
            {
                if(s[k]=='(')
                {
                    numPar++;
                }
                if(s[k]==')')
                {
                    numPar--;
                }
                if(numPar==0 && k<s.size()-1)
                {
                    return s;
                }
                if(numPar==0 && k==s.size()-1)
                {
                    if(s.size()>3)
                    {
                        return uselessPar(s.substr(1,k-1));
                    }
                }
            }
        }
    }
    return s;

}


void Instruction::determineType()
{
    type=Prog;
    //var="";
    if(cont.size()==1)
    {
        type="";
        if(cont[0].size()>2 && type.compare("")==0)
        {
           if(cont[0][0]=='I'&& cont[0][1]=='f' )
            {
               type=If_;
               return;
            }

        }
        if(cont[0].size()>3 && type.compare("")==0)
        {
            if(cont[0][0]=='F'&& cont[0][1]=='o' && cont[0][2]=='r')
            {
                type=For_;
                return;
            }

        }
        if(cont[0].size()>5 && type.compare("")==0)
        {
            if(cont[0][0]=='W'&& cont[0][1]=='h' && cont[0][2]=='i' && cont[0][3]=='l' && cont[0][2]=='e')
            {
                type=While_;
                return;
            }
            else if(cont[0][0]=='B'&& cont[0][1]=='r' && cont[0][2]=='e' && cont[0][3]=='a' && cont[0][2]=='k')
            {
                type=Break_;
                return;
            }

        }
        if(cont[0].size()>6 && type.compare("")==0)
        {
            if(cont[0][0]=='N'&& cont[0][1]=='e' && cont[0][2]=='w' && cont[0][3]=='V' && cont[0][4]=='a' && cont[0][5]=='r')
            {
                type=NewVar_;
                return;
            }

            else if(cont[0][0]=='R'&& cont[0][1]=='e' && cont[0][2]=='t' && cont[0][3]=='u' && cont[0][4]=='r' && cont[0][5]=='n')
            {
                type=Return_;
                return;
            }
            else if(cont[0][0]=='A'&& cont[0][1]=='d' && cont[0][2]=='d' && cont[0][3]=='A' && cont[0][4]=='t' && cont[0][5]=='t')
            {
                type=AddAtt_;
                return;
            }

        }
        if(cont[0].size()>7 && type.compare("")==0)
        {
            if(cont[0][0]=='A'&& cont[0][1]=='d' && cont[0][2]=='d' && cont[0][3]=='M' && cont[0][4]=='e' && cont[0][5]=='t' && cont[0][6]=='h')
            {
                type=AddMeth_;
                return;
            }
            else if(cont[0][0]=='N'&& cont[0][1]=='e' && cont[0][2]=='w' && cont[0][3]=='T' && cont[0][4]=='y' && cont[0][5]=='p' && cont[0][6]=='e')
            {
                type=NewType_;
                return;
            }
            else if(cont[0][0]=='N'&& cont[0][1]=='e' && cont[0][2]=='w' && cont[0][3]=='I' && cont[0][4]=='n' && cont[0][5]=='s' && cont[0][6]=='t')
            {
                type=NewInst_;
                return;
            }
        }
        if(cont[0].size()>8 && type.compare("")==0)
        {
            if(cont[0][0]=='M'&& cont[0][1]=='o' && cont[0][2]=='d' && cont[0][3]=='i' && cont[0][4]=='f' && cont[0][5]=='A' && cont[0][6]=='t' && cont[0][7]=='t')
            {
                type=ModifAtt_;
                return;
            }
            else if(cont[0][0]=='C'&& cont[0][1]=='o' && cont[0][2]=='n' && cont[0][3]=='t' && cont[0][4]=='i' && cont[0][5]=='n' && cont[0][6]=='u' && cont[0][7]=='e')
            {
                type=Continue_;
                return;
            }
        }
        if(cont[0].size()>9 && type.compare("")==0)
        {
            if(cont[0][0]=='D'&& cont[0][1]=='e' && cont[0][2]=='l' && cont[0][3]=='e' && cont[0][4]=='t' && cont[0][5]=='e' && cont[0][6]=='V' && cont[0][7]=='a' && cont[0][8]=='r')
            {
                type=DeleteVar_;
                return;
            }
            else if(cont[0][0]=='D'&& cont[0][1]=='e' && cont[0][2]=='l' && cont[0][3]=='e' && cont[0][4]=='t' && cont[0][5]=='e' && cont[0][6]=='A' && cont[0][7]=='t' && cont[0][8]=='t')
            {
                type=DeleteAtt_;
                return;
            }
        }
        if(cont[0].size()>10 && type.compare("")==0)
        {
            if(cont[0][0]=='D'&& cont[0][1]=='e' && cont[0][2]=='l' && cont[0][3]=='e' && cont[0][4]=='t' && cont[0][5]=='e' && cont[0][6]=='M' && cont[0][7]=='e' && cont[0][8]=='t' && cont[0][9]=='h')
            {
                type=DeleteMeth_;
                return;
            }
        }
        if(type.compare("")==0)
        {
            string crt=cont[0];
            vector<Instruction*> inst;
            unsigned int k=0;
            bool isInQuo_=false;
            bool isInBra_=false;
            int numPar=0;
            string actualMot="";
            string actualFun="";
            //cout<<"lol: "<<crt<<endl;
            crt=uselessPar(crt);
            if(!isWellPar(crt))
            {
                crt=parenth(crt);
                //crt=uselessPar(crt);
            }
            else
            {
                //cout<<"already well parenth"<<endl;
            }
            string s=crt;
            cont.pop_back();
            cont.push_back(s);
            for(k=0;k<crt.size();k++)
            {
                //cout<<"dans for:"<<s[k]<<endl;
                if(s[k]=='"')
                {
                    isInQuo_=!isInQuo_;
                }
                else if(s[k]=='\'')
                {
                    isInBra_=!isInBra_;
                }
                else if(s[k]=='(' && !isInBra_ && !isInQuo_ && actualMot.compare("")==0)
                {
                    numPar++;
                }
                else if(s[k]==')' && !isInBra_ && !isInQuo_)
                {
                    numPar--;
                }
                if(isInQuo_||isInBra_ || (numPar!=0))
                {
                    continue;
                }
                if(s[k]=='(' && actualMot.compare("")!=0)
                {
                    actualFun=actualMot;
                    actualMot="";
                }
                if(isNotSpe(s[k]))
                {
                    actualMot+=s[k];
                    //cout<<"isNotSpe: "<<s[k]<<endl;
                }
                else
                {
                    if(k<s.size()-1)
                    {
                        if(s[k]=='+' && s[k+1]=='+')
                        {
                            actualFun=Incr_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='-' && s[k+1]=='-')
                        {
                            actualFun=Decr_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='=' && s[k+1]=='=')
                        {
                            actualFun=Equal_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='!' && s[k+1]=='=')
                        {
                            actualFun=Diff_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='+' && s[k+1]=='=')
                        {
                            actualFun=PlusEqual_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='-' && s[k+1]=='=')
                        {
                            actualFun=MoinsEqual_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='*' && s[k+1]=='=')
                        {
                            actualFun=MultEqual_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='/' && s[k+1]=='=')
                        {
                            actualFun=DivEqual_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='>' && s[k+1]=='=')
                        {
                            actualFun=SupEqual_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='<' && s[k+1]=='=')
                        {
                            actualFun=InfEqual_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='-' && s[k+1]=='>')
                        {
                            actualFun=Fleche_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='[')
                        {
                            actualFun=Cro_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='&' && s[k+1]=='&')
                        {
                            actualFun=And_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='|' && s[k+1]=='|')
                        {
                            actualFun=Or_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='<' && s[k+1]=='<')
                        {
                            actualFun=Out_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='>' && s[k+1]=='>')
                        {
                            actualFun=In_;
                            type=actualFun;
                            return;
                        }
                    }
                    if(k<s.size() && actualFun.compare("")==0)
                    {
                        if(s[k]=='.')
                        {
                            actualFun=Point_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='=')
                        {
                            actualFun=Set_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='!')
                        {
                            actualFun=Neg_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='+')
                        {
                            actualFun=Plus_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='-')
                        {
                            actualFun=Moins_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='*')
                        {
                            actualFun=Mult_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='/')
                        {
                            actualFun=Div_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='%')
                        {
                            actualFun=Reste_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='>')
                        {
                            actualFun=Sup_;
                            type=actualFun;
                            return;
                        }
                        else if(s[k]=='<')
                        {
                            actualFun=Inf_;
                            type=actualFun;
                            return;
                        }
                    }
                }
                if(actualFun.compare("")!=0)
                {
                    type=actualFun;
                    break;
                }
            }

        }
        if(cont[0].size()>1 && type.compare("")==0)
        {
            string crt=cont[0];
            if(crt[0]=='?')
            {
                type=Arg_;
            }
        }
        if(type.compare("")==0)
        {
            type=Identity_;
        }
    }
    //cout<<"okType"<<endl;
}


void Instruction::preCompile(string inst)
{
    int k=0;
    int size=inst.size();
    //cout<<"contenu inst="<<inst<<endl;
    if(size>0)
    {
        if(inst[size-1]==';')
        {
            inst=inst.substr(0,size-1);
        }
    }
    //cout<<"contenu inst="<<inst<<endl;
    inst=uselessPar(inst);
    //cout<<"contenu inst="<<inst<<endl;
    size=inst.size();
    if(size>0)
    {
        if(inst[size-1]!=';')
        {
            inst+=";";
        }
    }
    cont=getArg(inst,';',0);
    //cout<<"size="<<cont.size()<<endl;
    if(type.compare("")==0)
    {
        determineType();
    }
    ok=((cont.size()>1)&&(type.compare(Prog)==0))||((cont.size()==1) && (type.compare(Prog)!=0));
    //cout<<"okPreCompile"<<endl;
}



/*
 *Constructeur de recherche dans la bdd
 */
Instruction::Instruction(string id)
{
    //name=_name;
    varDb.push_back(new DbVar());
    string req="select name, argT, retourT, cont, prior, assoc, isOp from instruction where ins_id=\""+id+"\";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            ok=true;
            name=getStringId(row[0]? row[0]: "");
            argT=makeArg(getStringId(row[1]? row[1]: ""));
            retourT=makeArg(getStringId(row[2]? row[2]: ""));
            brut=getStringId(row[3]? row[3] : "");
            string priorS=(row[4]? row[4]: "");
            char* priorStr=(char*)priorS.c_str();
            sscanf(priorStr,"%d",&prior);
            assoc=getStringId(row[5]? row[5]:"");
            string isOpS=(row[6]? row[6]: "");
            isOp=(isOpS.compare("1")==0);
            preCompile(brut);
        }
        else
        {
            Erreur("l'instruction \""+name+"\" n'existe pas!",context);
            ok=false;
        }
    }
    else
    {
        cout<<"Error requete sql: \""+req+"\""<<endl;
        ok=false;
    }

}


/*
 * Constructeur pour les instructions intermédiaires
 */

Instruction::Instruction(string _name, string _cont,vector<Vargen*> _arg, vector<DbVar*> _var)
{
    name=_name;
    prior=2;
    assoc="droite";
    brut=_cont;
    isOp=false;
    varDb=_var;
    arg=_arg;
    brut=setArgInString(brut,arg);

    initType();
    context=varDb;
    ok=true;
    type="";
    //cout<<"contenu inst="<<brut<<endl;
    preCompile(_cont);
    //cout<<"fin precompile "<<name<<endl;

}

/*
 * remplace les arguments présents dans brut par ?[k] où k est le numéro d'argument dans l'instruction
 */
string setArgInString(string brut, vector<Vargen*> arg)
{
    string s="";
    unsigned int k;
    string var="";
    bool isInQuo=false;
    bool isInBra=false;
    for(k=0;k<brut.size();k++)
    {
        if(brut[k]=='"' && !isInQuo)
        {
            if(k>0)
            {
                if(brut[k-1]!='\\')
                {
                   isInBra=!isInBra;
                }
            }
            else
            {
                isInBra=!isInBra;
            }
        }
        if(brut[k]=='\'' && !isInBra)
        {
            if(k>0)
            {
                if(brut[k-1]!='\\')
                {
                   isInQuo=!isInQuo;
                }
            }
            else
            {
                isInQuo=!isInQuo;
            }
        }
        if(isInQuo||isInBra)
        {
            s+=brut[k];
            continue;
        }
        if(isNotSpe(brut[k]))
        {
            var+=brut[k];
        }
        else
        {
           if(var.compare("")!=0)
           {
               unsigned int i;
               bool findVar=false;
               for(i=0;i<arg.size();i++)
               {
                   if(arg[i]!=NULL)
                   { 
                       if(var.compare(arg[i]->name)==0)
                        {
                            findVar=true;
                            break;
                        }
                   }
                   else
                   {
                       Erreur("argument NULL",context);
                   }
               }
               if(findVar)
               {
                   char numStr[4]={};
                   sprintf(numStr,"%d",i);
                   string num="?";
                   num+=numStr;
                   s+=num;
                   var="";
               }
               else
               {
                   s+=var;
                   var="";
               }
           }
           s+=brut[k];
        }
    }
    return s;
}


void Instruction::initType()
{
    varDb.push_back(new DbVar());
    Vargen* newL=new Vargen("endl",stringType,"endl");
    Vargen* boolVar=new Vargen(boolType,stringType,boolType);
    Vargen* intVar=new Vargen(intType,stringType,intType);
    Vargen* stringVar=new Vargen(stringType,stringType,stringType);
    Vargen* charVar=new Vargen(charType,stringType,charType);
    Vargen* floatVar=new Vargen(floatType,stringType,floatType);
    varDb.back()->insert(newL);
    varDb.back()->insert(boolVar);
    varDb.back()->insert(intVar);
    varDb.back()->insert(stringVar);
    varDb.back()->insert(charVar);
    varDb.back()->insert(floatVar);
}

string recopieString(string s)
{
    unsigned int k;
    string ret="";
    for(k=0;k<s.size();k++)
    {
        char c=s[k];
        ret+=c;
    }
    return ret;
}

void deleteString(string s)
{
    unsigned int k;
    for(k=0;k<s.size();k++)
    {
        char* ch=&(s[k]);
        delete ch;
    }
}

/*
 * Constructeur d'ajout de l'instruction à la bdd
 * argS: string décrivant les types d'argument: "name1:type1;name2:type2;...;"
 * retourS: string décrivant les types de retour: "type1;type2;"
 *
*/
Instruction::Instruction(string _name, string argS, string retourS, string inst, vector<DbVar*> varDb_, bool tmp_, unsigned int _prior, string _assoc, bool _isOp) {
    name=_name;    
    isOp=_isOp;
    //cout<<"retourS="<<retourS<<endl;
    string retourS2=recopieString(retourS);
    varDb=varDb_;
    initType();
    //cout<<"retourS="<<retourS<<endl;
    tmp=tmp_;
    assoc=((_assoc.compare("droite")==0)? "droite" : "gauche");    
    arg=makeArgVar(argS);
    brut=setArgInString(inst,arg);
    //brut=uselessPar(brut);
    //retourS=retourS2;
    //cout<<"brut= "<<brut<<endl;
    unsigned int k;
    string argStr="";
    for(k=0;k<arg.size();k++)
    {
        argT.push_back(arg[k]->type);
        argStr+=arg[k]->type->name;
        argStr+=";";
    }
    //cout<<"argT: "<<argStr<<endl;
    //cout<<"retourT: "<<retourS<<endl;
    retourT=makeArg(retourS2);
    //cout<<"retourT: "<<retourS<<endl;
    prior=_prior;
    string id=getIdInstruction(name,argStr,retourS2);
    //cout<<"argT: "<<argStr<<endl;
    if(id.compare("")==0)
    {
        ok=true;
        //cout<<"avant preCompile"<<endl;
        preCompile(inst);
        //cout<<"après preCompile"<<endl;
        unsigned int size=varDb.size();
        if(size>2)
        {
            char priorStr[4];
            sprintf(priorStr,"%d",prior);
            string priorS=priorStr;

            Vargen* nameVar=new Vargen(name,"string",name);
            varDb[size-2]->insert(nameVar);
            string nameId=getIdString(name);

            Vargen* argSvar=new Vargen(argStr,"string",argStr);
            varDb[size-2]->insert(argSvar);
            string argTId=getIdString(argStr);

            Vargen* retourSvar=new Vargen(retourS2,"string",retourS2);
            varDb[size-2]->insert(retourSvar);
            string retourId=getIdString(retourS2);

            Vargen* instvar=new Vargen(brut,"string",brut);
            varDb[size-2]->insert(instvar);
            string instId=getIdString(brut);

            Vargen* assocVar=new Vargen(assoc,"string",assoc);
            varDb[size-2]->insert(assocVar);
            string assocId=getIdString(assoc);

            string isOpS=(isOp? "true" : "false");
            string req="insert into instruction(name,argT,retourT,cont,prior,assoc,isOp) values("+nameId+","+argTId+","+retourId+","+instId+","+priorS+","+assocId+","+isOpS+");";
            //cout<<"requete inst:"<<req<<endl;
            memory->insert(req);
        }

    }
    else
    {
        ok=false;
        //cout<<"ok=false"<<endl;
    }

}


/*
 * renvoie le signal principal contenu dans les variables de l'instruction courante
 */
string getSignals()
{
    string ret="";
    if(context.size()>0)
    {
        Vargen* var=context[0]->find(Erreur_);
        if(var!=NULL)
        {
           if(var->type->name.compare(signalType)==0)
           {
               ret=Erreur_;
           }
        }
        else
        {
            var=context[0]->find(Retour_);
            if(var!=NULL)
            {
                if(var->type->name.compare(signalType)==0)
                {
                    ret=Retour_;
                }
            }
            else
            {
               unsigned int k;
               for(k=0;k<context.size();k++)
               {
                   var=context[k]->find(Break_);
                   if(var!=NULL)
                   {
                       break;
                   }
               }
               if(var!=NULL)
               {
                   if(var->type->name.compare(signalType)==0)
                   {
                       ret=Break_;
                   }
               }
               else
               {
                   for(k=0;k<context.size();k++)
                   {
                       var=context[k]->find(Continue_);
                       if(var!=NULL)
                       {
                           break;
                       }
                   }
                  if(var!=NULL)
                  {
                      if(var->type->name.compare(signalType)==0)
                      {
                          ret=Continue_;
                      }
                  }
               }
            }
        }
    }
    return ret;
}


void For(Instruction* deb, Instruction* stop, Instruction* incr,Instruction* boucle)
{
    stop->compile();
    if(stop->retour.size()==1)
    {
        Vargen* term=stop->retour[0];
        if(term->type->name.compare(boolType)==0)
        {
            for(deb->compile();term->valBool;incr->compile())
            {
                //boucle
                boucle->compile();

                // check si des erreurs, des Returns, des breaks ou des continues sont apparues
                string sig=getSignals();
                if(sig.compare(Erreur_)==0 || sig.compare(Return_)==0 || sig.compare(Break_)==0)
                {
                    break;
                }

                //préparation au nouveau tour de boucle
                stop->compile();
            }
        }
        else
        {
            Erreur("le deuxième argument de for n'est pas de type bool",context);
        }
    }
    else
    {
        Erreur("impossible de déterminer la valeur du second argument de for",context);
    }
    delete deb;
    delete stop;
    delete incr;
    delete boucle;
}


void While(Instruction* cond, Instruction* boucle)
{
    cond->compile();
    if(cond->retour.size()==1)
    {
        Vargen* stop=cond->retour[0];
        if(stop->type->name.compare(boolType)==0)
        {
            while(stop->valBool)
            {
                //boucle
                cout<<"boucle compile->"<<endl;
                boucle->compile();

                //check si des erreurs, des returns, des breaks ou des continues sont apparues
                string sig=getSignals();
                if(sig.compare(Erreur_)==0 || sig.compare(Retour_)==0 || sig.compare(Break_)==0)
                {
                    break;
                }

                //préparation au nouveau tour de boucle
                cond->compile();
                //cout<<"fin du premier tour de boucle"<<endl;
            }
        }
        else
        {
            Erreur("var1 de while n'est pas de type bool",context);
        }
    }
    else
    {
        Erreur("impossible de déterminer la valeur du premier argument de while",context);
    }
    delete cond;
    delete boucle;

}


void If(Instruction *cond, Instruction *boucle)
{
    cond->compile();
    if(cond->retour.size()==1)
    {
        Vargen* stop=cond->retour[0];
        if(stop->type->name.compare(boolType)==0)
        {
            if(stop->valBool)
            {
                //cout<<"Boucle compile!"<<endl;
                boucle->compile();
            }
        }
        else
        {
            Erreur("le premier argument de If n'est pas un booléen",context);
            stop->print();
        }
    }
    else
    {
        Erreur("le premier argument de If n'a pas le bon nombre d'arguments de retour",context);
    }
    delete boucle;
    delete cond;

}


void Else(Instruction *cond, Instruction *boucle)
{
    cond->compile();
    if(cond->retour.size()==1)
    {
        Vargen* stop=cond->retour[0];
        if(stop->type->name.compare(boolType)==0)
        {
            if(!stop->valBool)
            {
                boucle->compile();
            }
        }
        else
        {
            Erreur("La variable de condition du else n'est pas un booleen",context);
        }
    }
    else
    {
        Erreur("L'instruction n°1 dans else ne renvoit rien",context);
    }

}




std::vector<Instruction*> findInstruction(string name)
{
    std::vector<Instruction*> ret;
    string req="select id,name from instruction where name=\""+name+"\";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        //bool find=false;
        while(row=mysql_fetch_row(res))
        {
            //find=true;
            string id=(row[0]? row[0]: "");
            ret.push_back(new Instruction(id));
        }

    }
    else
    {
        cout<<"Erreur requete sql:"<<req<<endl;
    }
    return ret;
}

/*
 * name est une string naturelle (non identifiant d'une string dans la bdd)
 * idem argT
 * idem retourT
 */
string getIdInstruction(string name, string argT, string retourT)
{
    string id="";
    string nameId=getIdString(name);
    if(nameId.compare("")==0)
    {
        storeString(name);
        nameId=getIdString(name);
    }
    //cout<<"nameId="<<nameId<<endl;
    if(argT.compare("")!=0 && nameId.compare("")!=0)
    {
        string argTId=getIdString(argT);
        if(retourT.compare("")!=0 && argTId.compare("")!=0)
        {
            string retourTId=getIdString(retourT);
            if(retourTId.compare("")!=0)
            {
                string req="select ins_id from instruction where name="+nameId+" and argT="+argTId+" and retourT="+retourTId+";";
                MYSQL_RES* res=memory->request(req);
                if(res!=NULL)
                {
                   MYSQL_ROW row;
                   if(row=mysql_fetch_row(res))
                   {
                       id=(row[0]? row[0]:"");
                   }
                   else
                   {
                       //cout<<"l'instruction ("<<retourT<<") "<<name<<"("+argT+") n'existe pas"<<endl;
                   }
                }
                else
                {
                    cout<<"Erreur requete sql: "<<req<<endl;
                }
            }
        }
        else
        {
            if(nameId.compare("")!=0 && argTId.compare("")!=0)
            {
                string req="select ins_id from instruction where name="+nameId+" and argT="+argTId+";";
                MYSQL_RES* res=memory->request(req);
                if(res!=NULL)
                {
                    MYSQL_ROW row;
                    if(row=mysql_fetch_row(res))
                    {
                        id=(row[0]? row[0]:"");
                    }
                    else
                    {
                        //cout<<"l'instruction "<<name<<"("+argT+") n'existe pas"<<endl;
                    }
                    if(row=mysql_fetch_row(res))
                    {
                        cout<<"Attention: il existe plusieurs instruction de même prototype d'argument"<<endl;
                    }
                }
                else
                {
                    cout<<"Erreur requete sql: "<<req<<endl;
                }
            }
        }
    }
    else
    {
        string req="select ins_id from instruction where name="+nameId+";";
        MYSQL_RES* res=memory->request(req);
        if(res!=NULL)
        {
            MYSQL_ROW row;
            if(row=mysql_fetch_row(res))
            {
                id=(row[0]? row[0]:"");
            }
            else
            {
                //cout<<"l'instruction id:"<<id<<" n'existe pas"<<endl;
            }
        }
        else
        {
            cout<<"Erreur requete sql: "<<req<<endl;
        }
    }
    return id;
}


/*
 * renvoie un vector de string correspondant aux arguments contenus dans s
 * s=(arg1+sep+arg2+sep+...);
 * Ex: s=(arg1;arg2;...);
 * return: [arg1]::[arg2]::...
 */
vector<string> getArg(string s, char sep, unsigned int numPar_)
{
    vector<string> ret;
    unsigned int k;
    int numPar=0;
    int numSep=0;
    bool isInQuo=false;
    bool isInBra=false;
    string actual="";
    //cout<<"s="<<s<<endl;
    for(k=0;k<s.size();k++)
    {
        //cout<<"numPar="<<numPar<<endl;
        if(s[k]=='"' && !isInQuo)
        {
            if(k>0)
            {
                if(s[k]!='\\')
                {
                    isInBra=!isInBra;
                }
            }
            else
            {
                isInBra=!isInBra;
            }
        }
        if(s[k]=='\'' && !isInBra)
        {
            if(k>0)
            {
                if(s[k]!='\\')
                {
                    isInQuo=!isInQuo;
                }
            }
            else
            {
                isInQuo=!isInQuo;
            }
        }
        if(s[k]=='(' && !isInBra && !isInQuo)
        {
            if(numPar!=(numPar_-1))
            {
                actual+=s[k];
            }
            numPar++;
            continue;
        }
        if(s[k]==')' && !isInBra && !isInQuo)
        {
            if(numPar==(numPar_-1))
            {
                cout<<"Erreur: trop de parenthèses fermantes"<<endl;
                break;
            }
            else if(numPar==numPar_)
            {
                if(actual.compare("")!=0)
                {
                    //cout<<"actual="<<actual<<endl;
                    ret.push_back(actual);
                }
            }
            else
            {
                actual+=s[k];
            }
            numPar--;
            continue;
        }
        if(numPar>numPar_)
        {
            actual+=s[k];
        }
        if(numPar==numPar_)
        {
            if(s[k]==sep && !isInQuo && !isInBra)
            {
                if(sep==';')
                {
                    actual+=s[k];
                }
                //cout<<"actual="<<actual<<endl;
                ret.push_back(actual);
                actual="";
            }
            else
            {
                actual+=s[k];
            }
        }
    }
    return ret;
}

/*
 * si s=(arg1,arg2,...) return [Instruction(arg1)]::...
 */
vector<Instruction*> subInst(string s,string nameInst,vector<Vargen*> arg, vector<DbVar*> varDb)
{
    vector<Instruction*> ret;
    vector<string> retStr=getArg(s,',');
    unsigned int k;
    for(k=0;k<retStr.size();k++)
    {
        char kStr[4];
        sprintf(kStr,"%d",k);
        string kS=kStr;
        ret.push_back(new Instruction(nameInst+kS,retStr[k],arg,varDb));

    }
    return ret;
}


/*
 * return l'indice du début de name dans la string
 * name est le nom de l'opérateur
 * s=(arg1)name(arg2) ou s=arg1+name
 */
unsigned int indexInString2(string name, string s)
{
    unsigned int i=0;
    bool isInBra_=false;
    bool isInQuo_=false;
    int numPar=0;
    if(name.size()>1)
    {
        for(i=0;i<s.size()-1;i++)
        {
           unsigned int nbCom=0;
           unsigned int l;
           if(s[i]=='"')
           {
               isInQuo_=!isInQuo_;
           }
           if(s[i]=='\'')
           {
               isInBra_=!isInBra_;
           }
           if(isInQuo_||isInBra_)
           {
               continue;
           }
           if(s[i]=='(')
           {
               numPar++;
           }
           if(s[i]==')')
           {
               numPar--;
               continue;
           }
           if(numPar>0)
           {
               continue;
           }
           for(l=0;l<name.size();l++)
           {
               if(i+l<s.size())
               {
                   if(s[i+l]==name[l])
                   {
                       nbCom++;
                   }
                   else break;
               }
           }
           if(nbCom==name.size())
           {
               return i;
           }

        }
    }
    else if(name.size()==1)
    {
        char op=name[0];
        for(i=0;i<s.size();i++)
        {
            if(s[i]=='"')
            {
                isInQuo_=!isInQuo_;
            }
            if(s[i]=='\'')
            {
                isInBra_=!isInBra_;
            }
            if(isInQuo_||isInBra_)
            {
                continue;
            }
            if(s[i]=='(')
            {
                numPar++;
            }
            if(s[i]==')')
            {
                numPar--;
                continue;
            }
            if(numPar>0)
            {
                continue;
            }
            if(s[i]==op)
            {
                if(i>0 && i<s.size()-1)
                {
                    if(!isOpe(s[i-1]) && !isOpe(s[i+1]))
                    {
                        return i;
                    }
                }
                else if(i==0 && op=='!' && s.size()>0)
                {
                    if(!isOpe(s[1]))
                    {
                        return i;
                    }
                }
            }
        }
    }
    return i;
}



bool Instruction::interCompile()
{
    bool okCompile=true;
    if(argT.size()<arg.size() && !isKeyWord(type))
    {
        okCompile=false;
        //cout<<"type= "<<type<<endl;
        Erreur("Trop d'arguments dans "+name,context);
    }
    else if(argT.size()>arg.size() && !isKeyWord(type))
    {
        okCompile=false;
        Erreur("Trop peu d'arguments dans "+name,context);
    }
    else if(argT.size()==arg.size())
    {
        unsigned int k;
        for(k=0;k<argT.size();k++)
        {
            if(argT[k]->name.compare(arg[k]->type->name)!=0)
            {
                okCompile=false;
                stringstream num;
                num<<k;
                string numS="";
                num>>numS;
                Erreur("L'argument "+numS+" est de type "+arg[k]->type->name+" mais "+name+" attendait un argument de type "+argT[k]->name,context);
            }
        }
    }
    if(varDb.size()>0)
    {
        Vargen* noBuild=NULL;
        noBuild=varDb[0]->find(Erreur_);
        if(noBuild!=NULL)
        {
            if(noBuild->type->name.compare(signalType)==0)
            {
                okCompile=false;
            }
        }
        else
        {
            noBuild=varDb[0]->find(Retour_);
            if(noBuild!=NULL)
            {
                if(noBuild->type->name.compare(signalType)==0)
                {
                    okCompile=false;
                    if(varDb.size()==1) //si on est dans l'instruction primaire
                    {
                        if(noBuild->arg.size()>0)
                        {
                            unsigned int k;
                            for(k=0;k<noBuild->arg.size();k++)
                            {
                                retour.push_back(new Vargen(noBuild->arg[k]));
                            }
                        }
                    }
                }
            }
            else
            {
                int size=varDb.size();
                if(size>1)
                {
                    noBuild=varDb[size-2]->find(Break_);
                    if(noBuild!=NULL)
                    {
                        if(noBuild->type->name.compare(signalType)==0)
                        {
                            if(type.compare(For_)==0 || type.compare(While_)==0)
                            {
                                okCompile=false;
                                varDb[size-2]->erase(Break_);
                            }
                            else
                            {
                                okCompile=false;
                            }
                        }
                    }
                    else
                    {
                        noBuild=varDb[size-2]->find(Continue_);
                        if(noBuild!=NULL)
                        {
                            if(noBuild->type->name.compare(signalType)==0)
                            {
                                if(type.compare(For_)==0 || type.compare(While_)==0)
                                {
                                    okCompile=true;
                                    varDb[size-2]->erase(Continue_);
                                }
                                else
                                {
                                    okCompile=false;
                                }
                            }
                        }
                    }

                }
            }
        }
    }
    else
    {
        okCompile=false;
        Erreur("Une erreur s'est produite lors de la création de l'instruction: varDb est NULL",context);
    }
    return okCompile;
}

/*
 * Gère les flux input/Output lors de la compilation d'un << ou d'un >>
 */
void ManageStream(string s, char op, vector<Vargen*> exArg, vector<DbVar*> varDb)
{
    string stream="";
    vector<Vargen*> newArg=exArg;
    bool isInQuo=false;
    bool isInBra=false;
    bool streamOk=false;
    string arg="";
    unsigned int k;
    int numPar=0;
    unsigned int numChe=0;
    for(k=0;k<s.size();k++)
    {
        if(s[k]=='\'' && !isInBra)
        {
            if(k>0)
            {
                if(s[k-1]!='\\')
                {
                   isInQuo=!isInQuo;
                }
            }
            else
            {
                isInQuo=!isInQuo;
            }
        }
        if(s[k]=='"' && !isInQuo)
        {
            if(k>0)
            {
                if(s[k-1]!='\\')
                {
                   isInBra=!isInBra;
                }
            }
            else
            {
                isInBra=!isInBra;
            }
        }
        if(s[k]=='(' && !isInQuo && !isInBra)
        {
           numPar++;
        }
        if(s[k]==')' && !isInQuo && !isInBra)
        {
           numPar--;
           numChe=0;
           if(arg.compare("")!=0)
           {
               if(op=='<')
               {
                   Out(stream,new Instruction("Out_",arg,newArg,varDb));
               }
               else if(op=='>')
               {
                   In(stream,new Instruction("In_",arg,newArg,varDb));
               }
               arg="";
           }
        }
        if(isNotSpe(s[k]) && !isInQuo && !isInBra && !streamOk)
        {
            stream+=s[k];
        }
        if(s[k]==op && !isInQuo && !isInBra && !streamOk)
        {
            streamOk=true;
        }
        if(s[k]==op && streamOk)
        {
            numChe++;
        }
        if(numChe==2 && s[k]!=op && streamOk)
        {
            arg+=s[k];
        }
        if(k==s.size()-1)
        {
            numChe=0;
            if(arg.compare("")!=0)
            {
                if(op=='<')
                {
                    Out(stream,new Instruction("Out_",arg,newArg,varDb));
                }
                else if(op=='>')
                {
                    In(stream,new Instruction("In_",arg,newArg,varDb));
                }
                arg="";
            }
        }

    }
}


/*
 * renvoie le nom de la fonction dans s si une fonction y est appelée
 * sinon renvoie ""
 * si s de la forme inst(...) alors return "inst"
 */
string instInString(string s)
{
    string ret="";
    string tmp="";
    unsigned int k;
    for(k=0;k<s.size();k++)
    {
        if(isNotSpe(s[k]))
        {
            tmp+=s[k];
        }
        if(s[k]!='(' && tmp.compare("")!=0)
        {
            ret=tmp;
            break;
        }
    }
    return ret;
}

/*
 * si s=(var1,var2,...) return [var1]::[var2]::...
 */
vector<Vargen*> varInString(string s, vector<DbVar*> varDb_)
{
    vector<Vargen*> ret;
    unsigned int k;
    int numPar=0;
    bool okVar=false;
    string tmpVar="";
    bool isInQuo=false;
    bool isInBra=false;
    vector<Vargen*> newArg;
    for(k=0;k<s.size();k++)
    {
        if(s[k]=='"' && !isInQuo)
        {
            if(k>0)
            {
                if(s[k]!='\\')
                {
                    isInBra=!isInBra;
                }
            }
            else
            {
                isInBra=!isInBra;
            }
        }
        if(s[k]=='\'' && !isInBra)
        {
            if(k>0)
            {
                if(s[k]!='\\')
                {
                    isInQuo=!isInQuo;
                }
            }
            else
            {
                isInQuo=!isInQuo;
            }
        }
        if(s[k]=='(' && k==0 && !isInQuo && !isInBra)
        {
            numPar++;
            if(k==0)
            {
                continue;
            }
        }
        if(s[k]==')' && k==s.size()-1 && !isInQuo && !isInBra)
        {
            numPar--;
        }
        if(numPar==1)
        {
            if(s[k]==',' && !isInQuo && !isInBra)
            {
                if(tmpVar.compare("")!=0)
                {
                    Instruction* inst=new Instruction("var",tmpVar,newArg,varDb_);
                    inst->compile();
                    if(inst->retour.size()==1)
                    {
                        Vargen* var=inst->retour[0];
                        //var->print();
                        if(var!=NULL)
                        {
                            ret.push_back(new Vargen(var));
                        }
                        else
                        {
                            Erreur("la variable de retour de l'instruction: "+inst->brut+" est NULL",context);
                            break;
                        }
                    }
                    else
                    {
                        Erreur("L'instruction de contenu: "+tmpVar+" ne renvoie rien",context);
                    }
                    delete inst;
                }
                else
                {
                    Erreur("Trop de virgules dans l'expression: "+s,context);
                    break;
                }
                tmpVar="";
                continue;
            }
            if(k==s.size()-2)
            {
                if(tmpVar.compare("")!=0)
                {
                    Instruction* inst=new Instruction("var",tmpVar,newArg,varDb_);
                    //cout<<"toCompile"<<endl;
                    inst->compile();
                    if(inst->retour.size()==1)
                    {
                        Vargen* var=inst->retour[0];
                        //var->print();
                        if(var!=NULL)
                        {
                            ret.push_back(new Vargen(var));
                        }
                        else
                        {
                            Erreur("la variable de retour de l'instruction: "+inst->brut+" est NULL",context);
                            break;
                        }
                    }
                    else
                    {
                        Erreur("L'instruction de contenu: "+tmpVar+" ne renvoie rien",context);
                    }
                    delete inst;
                }
                break;
            }
            tmpVar+=s[k];
        }
        if(numPar<0)
        {
            Erreur("Trop de parenthèses fermantes",context);
            break;
        }

    }
    return ret;
}



/*
 * traduction du contenu de l'instruction en action cpp
 */
void Instruction::compile()
{
    unsigned int k=0;
    bool okCompile=interCompile();
    //cout<<"after interCompile"<<endl;
    if(okCompile)
    {
        //cout<<"compile name="<<name<<", type="<<type<<endl;
        vector<Vargen*> newArg;
        if(type.compare(Prog)==0)
        {
            vector<Instruction*> content;
            //cout<<"compile Prog"<<endl;
            for(k=0;k<cont.size();k++)
            {
                string name_=recopieString(name);
                //cout<<"Dans "<<name_<<" instruction  n°"<<k<<endl;
                string actual=cont[k];
                stringstream ss;
                ss<<name_<<k;
                string nameIns="";
                ss>>nameIns;
                Instruction* ins=new Instruction(nameIns,actual,arg,varDb);
                content.push_back(ins);
                ins->compile();
                //cout<<"fin d'instruction"<<endl;
                //cout<<ins->name<<" a fini de compiler"<<endl;
            }
            for(k=0;k<content.size();k++)
            {
                Instruction* ins=content[k];
                delete ins;
            }
            content.clear();
        }
        else if(type.compare(Arg_)==0) //accès à l'argument n°num
        {
            if(cont.size()==1)
            {
                //cout<<"compile "<<type<<endl;
                string crt=cont[0];
                crt=uselessPar(crt);
                crt=crt.substr(1);
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                //cout<<"crt= '"<<crt<<"'"<<endl;
                unsigned int num;
                unsigned int size=crt.size();
                char* str=(char*)crt.c_str();
                //cout<<"str: "<<str<<endl;
                int res=sscanf(str,"%u",&num);
                sscanf(str,"%*[\n]");
                if(res==1)
                {
                    if(num<arg.size())
                    {
                        Vargen* ret=arg[num];
                        if(ret!=NULL)
                        {
                            vector<Vargen*> exit;
                            //ret->print();
                            exit.push_back(ret);
                            retour=exit;
                        }
                        else
                        {
                            Erreur("Arg: la variable "+crt+" est NULL",context);
                        }

                    }
                    else
                    {
                        stringstream ss;
                        ss<<arg.size();
                        string sizeS="";
                        ss>>sizeS;
                        string strS=str;
                        Erreur("tentative d'accès à l'argument "+strS+" alors que l'instruction possède "+sizeS+" argument(s)",context);
                    }
                }
                else
                {
                    Erreur("lecture du numéro de l'argument",context);
                }

            }
        }

        //cas des instructions basiques
        else if(type.compare(Set_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                //cout<<arg1<<endl;
                string arg2=crt.substr(i+nameOp.size());
                //cout<<arg2<<endl;
                Set(identity(arg1,arg,varDb),new Instruction(name+"_Set_"+"d",arg2,arg,varDb));
            }
        }
        else if(type.compare(If_)==0)
        {
            //cout<<"compile: If"<<endl;
            if(cont.size()==1)
            {
                string actual=cont[0];
                if(actual.size()>3)
                {
                    string crt=actual.substr(2);
                    //cout<<crt<<endl;
                    vector<string> inst=getArg(crt,';');
                    if(inst.size()==2)
                    {
                        char zeroTmp[4];
                        sprintf(zeroTmp,"%d",0);
                        string zero=zeroTmp;
                        char unTmp[4];
                        sprintf(unTmp,"%d",1);
                        string un=unTmp;
                        //DbVar* db=new DbVar(varPar,varTmp);
                        If(new Instruction(name+"_"+zero,inst[0],arg,varDb),new Instruction(name+"_"+un,inst[1],arg,varDb));
                    }
                    else if(inst.size()<2)
                    {
                        Erreur("Pas assez d'arguments dans "+type,context);
                    }
                    else
                    {
                        Erreur("Trop d'arguments dans "+type,context);
                    }
                }
            }
        }
        else if(type.compare(For_)==0)
        {
            //cout<<"compile: For"<<endl;
            if(cont.size()==1)
            {
                string actual=cont[0];
                if(actual.size()>4)
                {
                    string crt=actual.substr(4);
                    //cout<<crt<<endl;
                    vector<string> inst=getArg(crt,';');
                    if(inst.size()==4)
                    {
                        char zeroTmp[4];
                        sprintf(zeroTmp,"%d",0);
                        string zero=zeroTmp;
                        char unTmp[4];
                        sprintf(unTmp,"%d",1);
                        string un=unTmp;
                        char deuxTmp[4];
                        sprintf(deuxTmp,"%d",2);
                        string deux=deuxTmp;
                        char troisTmp[4];
                        sprintf(troisTmp,"%d",3);
                        string trois=troisTmp;
                        //DbVar* db=new DbVar(varPar,varTmp);
                        For(new Instruction(name+"_"+zero,inst[0],newArg,varDb),new Instruction(name+"_"+un,inst[1],arg,varDb),new Instruction(name+"_"+deux,inst[2],arg,varDb),new Instruction(name+"_"+trois,inst[3],arg,varDb));
                    }
                    else if(inst.size()<4)
                    {
                        Erreur("Pas assez d'arguments dans "+type,context);
                    }
                    else
                    {
                        Erreur("Trop d'arguments dans "+type,context);
                    }
                }
            }
        }
        else if(type.compare(While_)==0)
        {
            cout<<"compile While"<<endl;
            if(cont.size()==1)
            {
                string actual=cont[0];
                if(actual.size()>4)
                {
                    string crt=actual.substr(4);
                    //cout<<crt<<endl;
                    vector<string> inst=getArg(crt,';');
                    if(inst.size()==2)
                    {
                        char zeroTmp[4];
                        sprintf(zeroTmp,"%d",0);
                        string zero=zeroTmp;
                        char unTmp[4];
                        sprintf(unTmp,"%d",1);
                        string un=unTmp;
                        //DbVar* db=new DbVar(varPar,varTmp);
                        While(new Instruction(name+"_"+zero,inst[0],arg,varDb),new Instruction(name+"_"+un,inst[1],arg,varDb));
                    }
                    else if(inst.size()<2)
                    {
                        Erreur("Pas assez d'arguments dans "+type,context);
                    }
                    else
                    {
                        Erreur("Trop d'arguments dans "+type,context);
                    }
                }
            }
        }
        else if(type.compare(NewVar_)==0)
        {
            //cout<<"compile: NewVar"<<endl;
            if(cont.size()==1)
            {
                string actual=cont[0];
                if(actual.size()>4)
                {
                    string crt=actual.substr(6);
                    //cout<<crt<<endl;
                    vector<string> inst=getArg(crt,',');
                    if(inst.size()==2)
                    {
                        NewVar(inst[0],inst[1],arg,varDb);
                    }
                    else if(inst.size()==3)
                    {
                        NewVar(inst[0],inst[1],arg,varDb,inst[2]);
                    }
                    else if(inst.size()==4)
                    {
                        bool tmp=(inst[3].compare("true")==0);
                        NewVar(inst[0],inst[1],arg,varDb,inst[2],tmp);
                    }
                    else if(inst.size()<2)
                    {
                        Erreur("Pas assez d'arguments dans "+type,context);
                    }
                    else
                    {
                        Erreur("Trop d'arguments dans "+type,context);
                    }
                }
            }
        }
        else if(type.compare(Identity_)==0)
        {
            //cout<<"Compile: Identity"<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                int size=crt.size();
                if(size>1)
                {
                    if(crt[size-1]==';')
                    {
                        string trueCrt=crt.substr(0,size-1);
                        //cout<<"trueCrt="<<trueCrt<<endl;
                        if(trueCrt.size()>0)
                        {
                            Vargen*ret=identity(trueCrt,arg,varDb);
                            if(ret!=NULL)
                            {
                                //ret->print();
                                retour.clear();
                                vector<Vargen*> exit;
                                exit.push_back(new Vargen(ret));
                                retour=exit;
                            }
                            else
                            {
                                Erreur("la variable "+trueCrt+" est NULL",context);
                            }
                        }
                    }
                    else
                    {
                        Vargen*ret=identity(crt,arg,varDb);
                        if(ret!=NULL)
                        {
                            vector<Vargen*> exit;
                            exit.push_back(new Vargen(ret));
                            retour=exit;
                        }
                        else
                        {
                            Erreur("Identity: la variable "+crt+" est NULL",context);
                        }
                    }
                }
            }
        }
        else if(type.compare(Return_)==0)
        {
            cout<<"Compile: Return"<<endl;
            if(cont.size()==1)
            {
               string crt=cont[0];
               crt=crt.substr(7);
               //cout<<"arg.size= "<<arg.size()<<endl;
               Vargen* var=Return(new Instruction("Return_"+name,crt,arg,varDb));
               Retour(var,varDb);
            }
        }
        else if(type.compare(DeleteVar_)==0)
        {
            cout<<"Compile: DeleteVar"<<endl;
            if(cont.size()==1)
            {
                int nbArg=1;
                string crt=cont[0];
                crt=crt.substr(6);
                vector<string> inst=getArg(crt,',');
                if(inst.size()==nbArg)
                {
                    deleteVar(inst[0],varDb);
                }
                else if(inst.size()<nbArg)
                {
                    Erreur("Pas assez d'arguments dans DeleteVar",context);
                }
                else
                {
                    Erreur("Trop d'arguments dans DeleteVar",context);
                }
            }
        }
        else if(type.compare(NewInst_)==0)
        {
            //cout<<"Compile: NewInst"<<endl;
            if(cont.size()==1)
            {
                int nbArg=4;
                string crt=cont[0];
                crt=crt.substr(6);
                vector<Instruction*> inst=subInst(crt,name,arg,varDb);
                Instruction* ret=NULL;
                if(inst.size()==nbArg+4)
                {
                    ret=NewInst(inst[0],inst[1],inst[2],inst[3],varDb,inst[4],inst[5],inst[6],inst[7]);

                }
                else if(inst.size()==nbArg+3)
                {
                    ret=NewInst(inst[0],inst[1],inst[2],inst[3],varDb,inst[4],inst[5],inst[6]);

                }
                else if(inst.size()==nbArg+2)
                {
                    ret=NewInst(inst[0],inst[1],inst[2],inst[3],varDb,inst[4],inst[5]);
                }
                else if(inst.size()==nbArg+1)
                {
                    ret=NewInst(inst[0],inst[1],inst[2],inst[3],varDb,inst[4]);
                }
                else if(inst.size()==nbArg)
                {
                    ret=NewInst(inst[0],inst[1],inst[2],inst[3],varDb);
                }
                else if(inst.size()<nbArg)
                {
                    Erreur("Pas assez d'arguments dans NewInst",context);
                }
                else
                {
                    Erreur("Erreur:Trop d'arguments dans NewInst",context);
                }
                if(ret!=NULL)
                {
                    newInst.push_back(ret);
                }
                int j;
                for(j=inst.size()-1;j>=0;j--) //
                {
                    Instruction* crtInst=inst[j];
                    delete crtInst;
                    inst.pop_back();
                }
            }
            //cout<<"fin "<<type<<endl;
        }
        else if(type.compare(NewType_)==0)
        {
            if(cont.size()==1)
            {
                string crt=cont[0];
                crt=crt.substr(7);
                vector<string> inst=getArg(crt,',');
                int nbArg=4;
                if(inst.size()==nbArg)
                {
                    newType(inst[0],inst[1],inst[2],inst[3]);
                }
                else if(inst.size()<nbArg)
                {
                   Erreur("Pas assez d'arguments dans "+type,context);
                }
                else
                {
                   Erreur("Trop d'arguments dans "+type,context);
                }
            }
        }
        else if(type.compare(AddAtt_)==0)
        {
            cout<<"Compile: AddAtt"<<endl;
            if(cont.size()==1)
            {
                int nbArg=3;
                string crt=cont[0];
                crt=crt.substr(6);
                vector<string> inst=getArg(crt,',');

                if(inst.size()==nbArg)
                {
                    addAtt(inst[0],inst[1],inst[3]);
                }
                else if(inst.size()<nbArg)
                {
                    Erreur("Pas assez d'arguments dans "+type,context);
                }
                else
                {
                    Erreur("Trop d'arguments dans "+type,context);
                }
            }
        }
        else if(type.compare(AddMeth_)==0)
        {
            if(cont.size()==1)
            {
                string crt=cont[0];
                crt=crt.substr(7);
                vector<string> inst=getArg(crt,',');
                int nbArg=2;
                if(inst.size()==nbArg)
                {
                    addMeth(inst[0],inst[1]);
                }
                else if(inst.size()<nbArg)
                {
                   Erreur("Pas assez d'arguments dans "+type,context);
                }
                else
                {
                   Erreur("Trop d'arguments dans "+type,context);
                }
            }
        }
        else if(type.compare(ModifAtt_)==0)
        {
            if(cont.size()==1)
            {
                string crt=cont[0];
                crt=crt.substr(8);
                vector<string> inst=getArg(crt,',');
                int nbArg=3;
                if(inst.size()==nbArg)
                {
                    modifAtt(inst[0],inst[1],inst[2]);
                }
                else if(inst.size()<nbArg)
                {
                   Erreur("Pas assez d'arguments dans "+type,context);
                }
                else
                {
                   Erreur("Pas assez d'arguments dans "+type,context);
                }
            }
        }
        else if(type.compare(DeleteAtt_)==0)
        {
            if(cont.size()==1)
            {
                string crt=cont[0];
                crt=crt.substr(9);
                vector<string> inst=getArg(crt,',');
                int nbArg=2;
                if(inst.size()==nbArg)
                {
                    deleteAtt(inst[0],inst[1]);
                }
                else if(inst.size()<nbArg)
                {
                   Erreur("Pas assez d'arguments dans "+type,context);
                }
                else
                {
                   Erreur("Trop d'arguments dans "+type,context);
                }
            }
        }
        else if(type.compare(DeleteMeth_)==0)
        {
            if(cont.size()==1)
            {
                string crt=cont[0];
                crt=crt.substr(10);
                vector<string> inst=getArg(crt,',');
                int nbArg=2;
                if(inst.size()==nbArg)
                {
                    deleteMeth(inst[0],inst[1]);
                }
                else if(inst.size()<nbArg)
                {
                   Erreur("Pas assez d'arguments dans "+type,context);
                }
                else
                {
                   Erreur("Trop d'arguments dans "+type,context);
                }
            }
        }

        //cas des opérateurs
        else if(type.compare(Cro_)==0)
        {
            //cout<<"Compile: "<<type<<endl;
            string nameOp="[";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }
               unsigned int i=indexInString2(nameOp,crt);
               string arg1=crt.substr(0,i);
               string arg2="";
               int nbCro=1;
               unsigned int l;
               bool isInQuo=false;
               bool isInBra=false;
               for(l=i+1;l<crt.size();l++)//l aura pour valeur l'indice juste après l'argument 2
               {
                   if(crt[l]=='"' && !isInQuo)
                   {
                        if(crt[l-1]!='\\')
                        {
                            isInBra=!isInBra;
                        }
                   }
                   if(crt[l]=='\'' && !isInBra)
                   {
                        if(crt[l-1]!='\\')
                        {
                            isInQuo=!isInQuo;
                        }
                   }
                   if(crt[l]=='[' && !isInBra && !isInQuo)
                   {
                       nbCro++;
                   }
                   if(crt[l]==']' && !isInBra && !isInQuo)
                   {
                       nbCro--;
                   }
                   if(nbCro==0)
                   {
                       break;
                   }
               }
               arg2=crt.substr(i+1,l-2);
               Vargen* ret=Cro(new Instruction(name+"_"+type+"_1",arg1,arg,varDb),new Instruction(name+"_"+type+"_num",arg2,arg,varDb));
               if(ret!=NULL)
               {
                   retour.clear();
                   vector<Vargen*> exit;
                   exit.push_back(ret);
                   retour=exit;
               }
               else
               {
                   Erreur("la variable "+crt+" est NULL",context);
               }
            }
        }
        else if(type.compare(Point_)==0)
        {
            string nameOp=".";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }


            }
        }
        else if(type.compare(Fleche_)==0)
        {
            string nameOp="->";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }
               unsigned int i=indexInString2(nameOp,crt);
               string arg1=crt.substr(0,i);
               Instruction* crtInst=new Instruction(name+"_"+type+"g",arg1,newArg,varDb);
               crtInst->compile();
               Vargen* var1=NULL;
               if(crtInst->retour.size()==1)
               {
                  if(var1!=NULL)
                  {
                      Vargen* var1=crtInst->retour[0];
                      unsigned int j=i+nameOp.size();
                      string arg2=crt.substr(j);
                      arg2=uselessPar(arg2);
                      string fun=instInString(arg2);
                      if(fun.compare("")!=0)
                      {
                          string argFun=crt.substr(j+fun.size());
                          vector<Vargen*> argMeth=varInString(argFun,varDb);
                          vector<Vargen*> ret=makeInstruction(fun,argMeth,arg,varDb,var1);
                          retour=ret;
                      }
                      else
                      {

                      }
                  }
                  else
                  {
                      Erreur("la variable "+arg1+" est NULL",varDb);
                  }
               }
               delete crtInst;

            }
        }
        else if(type.compare(Incr_)==0)
        {
            string nameOp="++";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }
               unsigned int i=indexInString2(nameOp,crt);
               string arg1=crt.substr(0,i);
               Decr(identity(arg1,arg,varDb));
            }
        }
        else if(type.compare(Decr_)==0)
        {
            string nameOp="--";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }
               unsigned int i=indexInString2(nameOp,crt);
               string arg1=crt.substr(0,i);
               Decr(identity(arg1,arg,varDb));

            }
        }
        else if(type.compare(In_)==0)
        {
            string nameOp=">>";
            if(cont.size()==1)
            {
                string crt=cont[0];
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                ManageStream(crt,'>',arg,varDb);
            }
        }
        else if(type.compare(Out_)==0)
        {
            string nameOp="<<";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }
               ManageStream(crt,'<',arg,varDb);
            }

        }
        else if(type.compare(Neg_)==0)
        {
            string nameOp="!";
            if(cont.size()==1)
            {
               string crt=cont[0];
               if(crt.size()>0)
               {
                   int size=crt.size();
                   if(crt[size-1]==';')
                   {
                       crt=crt.substr(0,size-1);
                   }
               }
               unsigned int i=indexInString2(nameOp,crt);
               string arg1=crt.substr(i+nameOp.size());
               //cout<<"arg="<<arg<<endl;
               Vargen* ret=Neg(new Instruction(name+"_Neg_"+"0",arg1,arg,varDb));
               if(ret!=NULL)
               {
                   retour.clear();
                   vector<Vargen*> exit;
                   exit.push_back(ret);
                   retour=exit;
               }
               else
               {
                   Erreur("la variable "+crt+" est NULL",context);
               }
            }
        }

        else if(type.compare(Or_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="||";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Or(new Instruction(name+"_Or_"+"g",arg1,arg,varDb),new Instruction(name+"_Or_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(And_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="&&";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=And(new Instruction(name+"_And_"+"g",arg1,arg,varDb),new Instruction(name+"_And_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Plus_)==0)
        {
            cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="+";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                //cout<<arg1<<endl;
                string arg2=crt.substr(i+nameOp.size());
                //cout<<arg2<<endl;
                //cout<<"arg.size= "<<arg.size()<<endl;
                Vargen* ret=Plus(new Instruction(name+"_Plus_"+"g",arg1,arg,varDb),new Instruction(name+"_Plus_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(SupEqual_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp=">=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=SupEqual(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Sup_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp=">";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Sup(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(InfEqual_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="<=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=InfEqual(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Inf_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="<";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Inf(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Equal_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="==";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Equal(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Diff_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="!=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Diff(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(PlusEqual_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="+=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                PlusEqual(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
            }
        }
        else if(type.compare(MoinsEqual_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="-=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                MoinsEqual(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));

            }
        }
        else if(type.compare(Moins_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="-";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Moins(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(MultEqual_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="*=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                MultEqual(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));

            }
        }
        else if(type.compare(Mult_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="*";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Mult(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(DivEqual_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="/=";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                DivEqual(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));

            }
        }
        else if(type.compare(Div_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="/";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Div(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Reste_)==0)
        {
            //cout<<"compile: "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string nameOp="%";
                if(crt.size()>0)
                {
                    int size=crt.size();
                    if(crt[size-1]==';')
                    {
                        crt=crt.substr(0,size-1);
                    }
                }
                unsigned int i=indexInString2(nameOp,crt);
                string arg1=crt.substr(0,i);
                string arg2=crt.substr(i+nameOp.size());
                Vargen* ret=Reste(new Instruction(name+"_"+type+"_"+"g",arg1,arg,varDb),new Instruction(name+"_"+type+"_"+"d",arg2,arg,varDb));
                if(ret!=NULL)
                {
                    retour.clear();
                    vector<Vargen*> exit;
                    exit.push_back(ret);
                    retour=exit;
                }
                else
                {
                    Erreur("la variable "+crt+" est NULL",context);
                }
            }
        }
        else if(type.compare(Continue_)==0)
        {
            Continue(varDb);
        }
        else if(type.compare(Break_)==0)
        {
            Break(varDb);
        }

        //cas des instructions générales
        else
        {
            cout<<"Compile: appel de "<<type<<endl;
            if(cont.size()==1)
            {
                string crt=cont[0];
                string argFun=crt.substr(type.size());
                vector<Vargen*> argMeth=varInString(argFun,varDb);
                //cout<<"Avant makeInst, arg.size="<<argMeth.size()<<endl;
                vector<Vargen*> ret=makeInstruction(type,argMeth,arg,varDb);
                retour=ret;
            }
        }
    }
    if(varDb.size()==1)
    {
        Vargen* noBuild=varDb[0]->find(Erreur_);
        if(noBuild!=NULL)
        {
            if(noBuild->type->name.compare(signalType)==0)
            {
                cout<<"Une erreur s'est produite lors de la compilation! Exit..."<<endl;
            }
        }
        else
        {
            noBuild=varDb[0]->find(Retour_);
            if(noBuild!=NULL)
            {
                if(noBuild->type->name.compare(signalType)==0)
                {
                    if(noBuild->arg.size()>0)
                    {
                        unsigned int k;
                        for(k=0;k<noBuild->arg.size();k++)
                        {
                            retour.push_back(new Vargen(noBuild->arg[k]));
                        }
                        cout<<"La compilation s'est déroulée avec succès!"<<endl;
                    }

                }
            }
            else
            {
                cout<<"La compilation s'est déroulée avec succès!"<<endl;
            }
        }
    }
}


void Instruction::print()
{
    cout<<"Instruction name: "<<name<<endl;
    cout<<"Type d'instruction: "<<type<<endl;
    string isOpS=(isOp? "true":"false");
    cout<<"Opérateur:"<<isOpS<<endl;
    cout<<"Type d'entrée: ";
    unsigned int k=0;
    for(k=0;k<argT.size();k++)
    {
        if(k!=argT.size()-1)
        {
            cout<<"Arg["<<k<<"]="<<argT[k]->name<<", ";
        }
        else
        {
            cout<<"Arg["<<k<<"]="<<argT[k]->name<<endl;
        }
    }
    if(argT.size()==0)
    {
        cout<<"void"<<endl;
    }
    cout<<"Type de sortie: ";
    for(k=0;k<retourT.size();k++)
    {
        if(k!=retourT.size()-1)
        {
            cout<<"Arg["<<k<<"]="<<retourT[k]->name<<", ";
        }
        else
        {
            cout<<"Arg["<<k<<"]="<<retourT[k]->name<<endl;
        }
    }
    if(retourT.size()==0)
    {
        cout<<"void"<<endl;
    }
    cout<<"Contenu: "<<endl;
    for(k=0;k<cont.size();k++)
    {
        cout<<cont[k]<<endl;
    }
    char priorId[4];
    sprintf(priorId,"%d",prior);
    string priorStr=priorId;
    cout<<"Priorité: "+priorStr<<endl;
    cout<<"Associativité à "<<assoc<<endl;
}

Instruction::Instruction(const Instruction& orig) {
}


/*
 * retire l'instruction de la bdd
 */
void Instruction::deleteInst()
{
    if(tmp)
    {
        string argTS="";
        unsigned int k;
        for(k=0;k<arg.size();k++)
        {
            argTS+=arg[k]->name;
            argTS+=";";
        }
        //cout<<"delete de l'instruction "<<name<<endl;
        string id=getIdInstruction(name,argTS);
        string req="delete from instruction where ins_id="+id+";";
        memory->insert(req);
    }
}




Instruction::~Instruction() {
    //cout<<"delete "<<name<<endl;

    /*while(arg.size()>0)
    {
        unsigned int size=arg.size();
        arg[size-1]->deleteVar();
        delete arg[size-1];
        arg.pop_back();
    }*/
    arg.clear();
    while(argT.size()>0)
    {
        unsigned int size=argT.size();
        delete argT[size-1];
        argT.pop_back();
    }
    while(retourT.size()>0)
    {
        unsigned int size=retourT.size();
        delete retourT[size-1];
        retourT.pop_back();
    }
    int size=varDb.size();
    if(size>0)
    {
        DbVar* last=varDb[size-1];
        delete last;
        varDb.pop_back();
        context=varDb;
    }
    while(retour.size()>0)
    {
       Vargen* var=retour.back();
       //cout<<"delete retour, name="<<var->name<<endl;
       var->deleteVar();
       delete var;
       retour.pop_back();
    }
    //retour.clear();
    while(newInst.size()>0)
    {
        unsigned int size=newInst.size();
        Instruction* actualInst=newInst[size-1];
        //cout<<"delete inst: "<<actualInst->name<<endl;
        actualInst->deleteInst();
        delete actualInst;
        newInst.pop_back();
    }
}

