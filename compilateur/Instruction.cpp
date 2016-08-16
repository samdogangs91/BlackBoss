/* 
 * File:   Instruction.cpp
 * Author: mblack
 * 
 * Created on 23 février 2016, 18:37
 */

#include "Instruction.h"
#include "Database.h"

using namespace std;

string If_="If";
string Set_="Set";
string For_="For";
string While_="While";
string NewVar_="NewVar";
string Bool_="Bool";
string Prog="Program";

extern string stringType;
extern string intType;
extern string charType;
extern string boolType;
extern Database* memory;


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
            cout<<"Erreur le string_id:"<<id<<" n'existe pas"<<endl;
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
            cout<<"Erreur la string:"<<cont<<" n'existe pas"<<endl;
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
    while(k<cont.size())
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
        k++;
    }
    return ret;
}

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
    return (c=='+')||(c=='-')||(c=='*')||(c=='∕')||(c=='=')||(c=='!')||(c=='>')||(c=='<');
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
int indexInString(string name, string s, vector<int> index, unsigned int nbAtten)
{
    unsigned int i=0;
    //cout<<"tab size: "<<index.size()<<endl;
    unsigned int nbVu=0;
    if(name.size()>1)
    {
        for(i=0;i<s.size()-1;i++)
        {
           unsigned int nbCom=0;
           unsigned int l;
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
    for(k=0;k<s.size();k++)
    {
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
            cout<<"instruction: "<<actualFun<<endl;
            string idFun=getIdInstruction(actualFun);
            cout<<"idFun:"<<idFun<<endl;
            inst.push_back(new Instruction(idFun));
            actualFun="";
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
            cout<<s<<endl;
            if(k<inst.size()-1)
            {
                ret="";
                string name;
                bool isInst1=false;
                bool isInst2=false;
                bool isEqual=false;
                bool isLeft=false;
                bool isOp=false;
                bool sameName=false;
                if(inst[k]->prior> inst[k+1]->prior)
                {
                    isInst1=true;
                    name=inst[k+1]->name;
                    isOp=inst[k+1]->isOp;
                    inst.erase(inst.begin()+1);
                    cout<<"plus fort à droite: name= "<<name<<endl;
                }
                else if(inst[k]->prior==inst[k+1]->prior)
                {
                    isEqual=true;
                    if(inst[k]->assoc.compare("gauche")==0)
                    {                        
                        name=inst[k]->name;
                        isOp=inst[k]->isOp;
                        inst.erase(inst.begin());
                        isLeft=true;
                        cout<<"isEqual, assoc gauche et name="<<name<<endl;
                    }
                    else
                    {
                        name=inst[k+1]->name;
                        isOp=inst[k+1]->isOp;
                        inst.erase(inst.begin()+1);
                        sameName=(inst[k]->name.compare(inst[k+1]->name)==0);
                        cout<<"isEqual, assoc droite et name="<<name<<endl;
                        //isRight=true;
                    }
                }
                else if(inst[k]->prior < inst[k+1]->prior)
                {
                    isInst2=true;
                    name=inst[k]->name;
                    isOp=inst[k]->isOp;
                    inst.erase(inst.begin());
                    cout<<"plus fort à gauche: name= "<<name<<endl;
                }
                if(isOp)
                {
                   if(name.compare("++")==0 || name.compare("--")==0)
                   {
                       unsigned int i;
                       if(sameName)
                       {
                           i=indexInString(name,s,tab,2);
                       }
                       else
                       {
                           i=indexInString(name,s,tab);
                       }
                       tab.push_back(i+1);
                       int j;
                       int numPar2=0;
                       if(i>0)
                       {
                           for(j=i-1;j>=0;j--)
                           {
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
                               if(!isNotSpe(s[j]) && numPar2==0) //si le caractère ne peut pas être dans une variable
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
                           i=indexInString(name,s,tab,2);
                       }
                       else
                       {
                           i=indexInString(name,s,tab);
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
                                   if(!isNotSpe(s[l]) && numPar2==0)
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
                           if(isInst1 || (isEqual && isLeft))
                           {
                               ret+='(';
                               for(l=0;l<s.size();l++)
                               {
                                   if(l>i && !ok)
                                   {
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
                                       if(!isNotSpe(s[l]) && numPar2==0)
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
                           i=indexInString("[",s,tab,2);
                       }
                       else
                       {
                           i=indexInString("[",s,tab);
                       }
                       tab.push_back(i+1);
                       int j;
                       int nbCro=0;
                       if(i>0)
                       {
                           int numPar2=0;
                           for(j=i-1;j>=0;j--)
                           {
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
                               if(!isNotSpe(s[j]) && numPar2==0 && nbCro==0) //si le caractère ne peut pas être dans une variable
                               {
                                   break;
                               }
                           }
                           unsigned int l;
                           int nbCro=0;
                           bool ok=false;
                           int nbOk=0;
                           for(l=0;l<s.size();l++)
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
                           i=indexInString(name,s,tab,2);
                       }
                       else
                       {
                           i=indexInString(name,s,tab);
                       }
                        tab.push_back(i+1);
                        //int size=tab.size();
                        //tab.pop_back();
                        cout<<"tab size: "<<tab.size()<<endl;
                        //int val=i+1;
                        //tab.push_back(val);
                        cout<<"indexInString: "<<i<<endl;
                        //tab[size-1]=i+1;
                        if(i>0)
                        {
                            int j=0;
                            int numPar2=0;
                            int nbCro=0;
                            for(j=i-1;j>=0;j--) //j est l'indice juste avant le debut de l' argument 1
                            {
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
                                if(!isNotSpe(s[j]) && numPar2==0 && nbCro==0) //si le caractère ne peut pas être dans une variable
                                {
                                    break;
                                }
                            }
                            cout<<"j="<<j<<endl;
                            unsigned int m;
                            numPar2=0;
                            nbCro=0;
                            for(m=i+name.size();m<s.size();m++) //m aura pour valeur l'indie juste après la fin du deuxième argument
                            {
                                if(s[m]=='(')
                                {
                                    numPar2++;
                                }
                                if(s[m]==')' && numPar2==1)
                                {
                                    numPar2--;
                                    continue;
                                }
                                if(s[m]==')')
                                {
                                    numPar2--;
                                }
                                if(s[m]=='[')
                                {
                                    nbCro++;
                                }
                                if(s[m]==']' && nbCro==1)
                                {
                                    nbCro--;
                                    continue;
                                }
                                if(s[m]==']')
                                {
                                    nbCro--;
                                }
                                if(!isNotSpe(s[m]) && numPar2==0 && nbCro==0) //si le caractère ne peut pas être dans une variable
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
                            //cout<<"retour: "<<ret<<endl;

                        }
                   }
                }
                else
                {
                    unsigned int i;
                    if(sameName)
                    {
                        i=indexInString(name,s,tab,2);
                    }
                    else
                    {
                        i=indexInString(name,s,tab);
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
                        if(isInst1 || (isEqual && isLeft))
                        {
                            ret+='(';
                            ret+=name;
                            ret+='(';
                            int numPar2=0;
                            for(j=i+name.size();j<s.size();j++)
                            {
                                ret+=s[j];
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

            }

        }
    }
    //cout<<"retour: "<<ret<<endl;
    inst.clear();
    return ret;
}


void Instruction::preCompile(string inst)
{
    int k=0;
    int numPar=0; //nombre de paranthèses non fermées
    string actual;
    while(k<inst.size())
    {
        if(inst[k]==';' && numPar==0)
        {
            actual+=inst[k];
            //char id[4];
            //sprintf(id,"%d",k);
            //string nameId=name+"_"+id;
            cont.push_back(actual);
            actual="";
        }
        else if(inst[k]=='(')
        {
            numPar++;
            actual+=inst[k];
        }
        else if(inst[k]==')')
        {
            numPar--;
            actual+=inst[k];
        }
        else
        {
            actual+=inst[k];
        }
        k++;
    }
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
            }

        }
        if(cont[0].size()>3 && type.compare("")==0)
        {
            if(cont[0][0]=='F'&& cont[0][1]=='o' && cont[0][2]=='r')
            {
                type=For_;
            }

        }
        if(cont[0].size()>5 && type.compare("")==0)
        {
            if(cont[0][0]=='W'&& cont[0][1]=='h' && cont[0][2]=='i' && cont[0][3]=='l' && cont[0][2]=='e')
            {
                type=While_;
            }

        }
        if(cont[0].size()>6 && type.compare("")==0)
        {
            if(cont[0][0]=='N'&& cont[0][1]=='e' && cont[0][2]=='w' && cont[0][3]=='V' && cont[0][4]=='a' && cont[0][5]=='r')
            {
                type=NewVar_;
            }

        }
        if(type.compare("")==0)
        {
            string crt=cont[0];
            vector<Instruction*> inst;
            unsigned int k=0;


        }
    }
    ok=((cont.size()>1)&&(type.compare(Prog)==0))||((cont.size()==1) && (type.compare(Prog)!=0));
}



/*
 *Constructeur de recherche dans la bdd
 */
Instruction::Instruction(string id)
{
    //name=_name;
    varTmp=new DbVar();
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
            cout<<"Error instruction \""+name+"\" doesn't exist!"<<endl;
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

Instruction::Instruction(string _name, string _cont)
{
    name=_name;
    prior=2;
    assoc="droite";
    brut=_cont;
    isOp=false;
    ok=true;
    //cout<<"contenu inst="<<brut<<endl;
    preCompile(_cont);

}


/*
 * Constructeur d'ajout de l'instruction à la bdd
 * argS: string décrivant les types d'argument: "type1;type2;"
 * retourS: string décrivant les types de retour: "type1;type2;"
*/
Instruction::Instruction(string _name, string argS, string retourS, string inst,unsigned int _prior, string _assoc, bool _isOp, bool tmp) {
    name=_name;
    varTmp=new DbVar();
    brut=inst;
    isOp=_isOp;
    assoc=((_assoc.compare("droite")==0)? "droite" : "gauche");
    //cout<<"assoc:"<<assoc<<endl;
    argT=makeArg(argS);
    retourT=makeArg(retourS);
    prior=_prior;
    string id=getIdInstruction(name,argS,retourS);
    if(id.compare("")==0)
    {
        ok=true;
        //cout<<"avant preCompile"<<endl;
        preCompile(inst);
        //cout<<"après preCompile"<<endl;
        if(!tmp)
        {
            char priorStr[4];
            sprintf(priorStr,"%d",prior);
            string priorS=priorStr;

            Vargen* nameVar=new Vargen(name,"string",name);
            delete nameVar;
            string nameId=getIdString(name);
            //cout<<"name id:"<<nameId<<endl;

            Vargen* argSvar=new Vargen(argS,"string",argS);
            delete argSvar;
            string argTId=getIdString(argS);

            Vargen* retourSvar=new Vargen(retourS,"string",retourS);
            delete retourSvar;
            string retourId=getIdString(retourS);

            Vargen* instvar=new Vargen(inst,"string",inst);
            delete instvar;
            string instId=getIdString(inst);

            Vargen* assocVar=new Vargen(assoc,"string",assoc);
            delete assocVar;
            string assocId=getIdString(assoc);

            string isOpS=(isOp? "true" : "false");
            string req="insert into instruction(name,argT,retourT,cont,prior,assoc,isOp) values("+nameId+","+argTId+","+retourId+","+instId+","+priorS+","+assocId+","+isOpS+");";
            cout<<"requete inst:"<<req<<endl;
            memory->insert(req);
        }
    }
    else
    {
        ok=false;
    }

}


void For(Instruction* deb, Instruction* stop, Instruction* incr,Instruction* boucle)
{
    if(deb->type.compare(Set_)==0 && stop->type.compare(Bool_)==0 && incr->type.compare(Set_)==0)
    {
        if(stop->arg.size()==1)
        {
            Vargen* term=stop->arg[0];
            if(term->type->name.compare(boolType)==0)
            {
                deb->compile();
                while(term->valBool)
                {
                    boucle->compile();
                    stop->compile();
                }

            }
        }
    }
}


void While(Instruction* cond, Instruction* boucle)
{
    if(cond->type.compare(Bool_)==0)
    {
        if(cond->arg.size()==1)
        {
            Vargen* stop=cond->arg[0];
            if(stop->type->name.compare(boolType)==0)
            {
                while(stop->valBool)
                {
                    boucle->compile();
                }
            }
        }
    }
}


void If(Instruction *cond, Instruction *boucle)
{
    if(cond->type.compare(Bool_)==0)
    {
        if(cond->arg.size()==1)
        {
            Vargen* stop=cond->arg[0];
            if(stop->type->name.compare(boolType)==0)
            {
                if(stop->valBool)
                {
                    boucle->compile();
                }
            }
        }
        else
        {
            cout<<"l'instruction "<<cond->name<<" n'a pas le bon nombre d'argument!"<<endl;
            cond->print();
            boucle->print();
        }
    }
    else
    {
        cout<<"Instruction "<<cond->name<<" n'est pas de type bool"<<endl;
        cond->print();
        boucle->print();
    }
}


void Else(Instruction *cond, Instruction *boucle)
{
    if(cond->type.compare(Bool_)==0)
    {
        if(cond->arg.size()==1)
        {
            Vargen* stop=cond->arg[0];
            if(stop->type->name.compare(boolType)==0)
            {
                if(!stop->valBool)
                {
                    boucle->compile();
                }
            }
        }
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
    //cout<<"nameId="<<nameId<<endl;
    if(argT.compare("")!=0)
    {
        string argTId=getIdString(argT);
        if(retourT.compare("")!=0)
        {
            string retourTId=getIdString(retourT);
            string req="select ins_id from instruction where name="+nameId+" and "+argTId+" and "+retourTId+";";
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
                   cout<<"l'instruction ("<<retourT<<") "<<name<<"("+argT+") n'existe pas"<<endl;
               }
            }
            else
            {
                cout<<"Erreur requete sql: "<<req<<endl;
            }
        }
        else
        {
            string req="select ins_id from instruction where name="+nameId+" and "+argTId+";";
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
                    cout<<"l'instruction "<<name<<"("+argT+") n'existe pas"<<endl;
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


void Instruction::compile()
{
    unsigned int k=0;
    //cout<<"type="<<type<<endl;
    if(type.compare(Prog)==0)
    {
        //cout<<"compile Prog"<<endl;
        for(k=0;k<cont.size();k++)
        {
            string actual=cont[k];
            char id[4];
            sprintf(id,"%d",k);
            string idStr=id;
            string nameIns=name+idStr;
            Instruction* ins=new Instruction(nameIns,actual);
            ins->compile();
        }
    }
    else if(type.compare(If_)==0)
    {
        //cout<<"compile If"<<endl;
        if(cont.size()==1)
        {
            string actual=cont[0];
            if(actual.size()>3)
            {
                unsigned int numIns=0;
                string crtInst1="";
                string crtInst2="";
                unsigned int nbPtVir=0; //nb de pt virgule
                int numPar=0;
                bool okPars=true;
                for(k=3;k<actual.size();k++)
                {
                    //cout<<"k="<<k<<endl;
                    if(numPar<0)
                    {
                        okPars=false;
                        break;
                    }
                    if(actual[k]==';' && numPar==0)
                    {
                        if(nbPtVir==0)
                        {
                            nbPtVir++;
                            crtInst1+=actual[k];
                            k++;
                            continue;
                        }
                        else if(nbPtVir==1)
                        {
                            crtInst2+=actual[k];
                            break;
                        }
                    }
                    if(actual[k]=='(')
                    {
                        if(nbPtVir==0)
                        {
                            crtInst1+=actual[k];
                        }
                        else if(nbPtVir==1)
                        {
                            crtInst2+=actual[k];
                        }
                        numPar++;
                    }
                    else if(actual[k]==')')
                    {
                        if(nbPtVir==0)
                        {
                            crtInst1+=actual[k];
                        }
                        else if(nbPtVir==1)
                        {
                            crtInst2+=actual[k];
                        }
                        numPar--;
                    }
                    else
                    {
                        if(nbPtVir==0)
                        {
                            if(crtInst1.size()==0 && actual[k]==' ')
                            {
                                k++;
                                continue;
                            }
                            else
                            {
                                crtInst1+=actual[k];
                            }
                        }
                        else if(nbPtVir==1)
                        {
                            if(crtInst2.size()==0 && actual[k]==' ')
                            {
                                k++;
                                continue;
                            }
                            else
                            {
                                crtInst2+=actual[k];
                            }



                        }
                    }
                }
                if(crtInst1.compare("")!=0 && crtInst2.compare("")!=0 && okPars)
                {
                    char zeroTmp[4];
                    sprintf(zeroTmp,"%d",0);
                    string zero=zeroTmp;
                    char unTmp[4];
                    sprintf(unTmp,"%d",1);
                    string un=unTmp;
                    If(new Instruction(name+"_"+zero,crtInst1),new Instruction(name+"_"+un,crtInst2));
                }
            }
        }
    }
    else if(type.compare(For_)==0)
    {

    }
    else if(type.compare(While_)==0)
    {

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
            cout<<"Arg["<<k<<"]="<<argT[k]->name<<", ";
        }
        else
        {
            cout<<"Arg["<<k<<"]="<<argT[k]->name<<endl;
        }
    }
    if(retourT.size()==0)
    {
        cout<<"void"<<endl;
    }
    cout<<"Contenu: "+brut<<endl;
    char priorId[4];
    sprintf(priorId,"%d",prior);
    string priorStr=priorId;
    cout<<"Priorité: "+priorStr<<endl;
    cout<<"Associativité à "<<assoc<<endl;
}

Instruction::Instruction(const Instruction& orig) {
}

Instruction::~Instruction() {
}

