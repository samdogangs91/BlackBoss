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
 * retourne la string correspondant à l'id dans la base de données
 */
string getStringId(string id)
{
    string ret="";
    string req="select cont from string natural join list_char where string_id="+id+";";
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
    string req="select string_id from string natural join list_char where cont=\""+cont+"\";";
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
           else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       //var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       //var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            //var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }
           }
        }
        if(cont[0].size()>3 && type.compare("")==0)
        {
            if(cont[0][0]=='F'&& cont[0][1]=='o' && cont[0][2]=='r')
            {
                type=For_;
            }
            else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       //var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       //var=vname;
                       break;
                   }
                   else if(i<cont.size())
                   {
                        if((cont[0][i-1]=='+' && cont[0][i]=='+') || (cont[0][i-1]=='-' && cont[0][i]=='-'))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i-1]=='+' && cont[0][i]=='=') || (cont[0][i-1]=='-' && cont[0][i]=='=') || (cont[0][i-1]=='*' && cont[0][i]=='=') || (cont[0][i-1]=='/' && cont[0][i]=='='))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i-1]=='<' && cont[0][i]=='=') || (cont[0][i-1]=='>' && cont[0][i]=='=') || (cont[0][i-1]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            //var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }
           }
        }
        if(cont[0].size()>5 && type.compare("")==0)
        {
            if(cont[0][0]=='W'&& cont[0][1]=='h' && cont[0][2]=='i' && cont[0][3]=='l' && cont[0][2]=='e')
            {
                type=While_;
            }
            else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       //var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       //var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            //var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }
           }
        }
        if(cont[0].size()>6 && type.compare("")==0)
        {
            if(cont[0][0]=='N'&& cont[0][1]=='e' && cont[0][2]=='w' && cont[0][3]=='V' && cont[0][4]=='a' && cont[0][5]=='r')
            {
                type=NewVar_;
            }
            else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       //var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       //var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            //var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            //var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }
           }
        }
        if(type.compare("")==0)
        {

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
    assoc=((assoc.compare("droite")==0)? "droite" : "gauche");
    argT=makeArg(argS);
    retourT=makeArg(retourS);
    prior=_prior;
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


string getIdInstruction(string name, string argT, string retourT)
{
    string id="";
    string nameId=getIdString(name);
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

