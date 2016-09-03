#include "type.h"
#include "Database.h"
#include <string>

extern Database* memory;
using namespace std;

string stringType="string";
string intType="int";
string charType="char";
string boolType="bool";
string floatType="float";
string signalType="signal";

string listType="list";
string queueType="queue";
string stackType="stack";

string newString(string s)
{
    string ret="";
    unsigned int k=0;
    while(k<s.size())
    {
        char* c=new char(s[k]);
        ret+=c[0];
        k++;
    }
    //cout<<"new string: "<<ret<<endl;
    return ret;
}

bool isBasic(string s)
{
    return (s.compare(intType)==0)||(s.compare(charType)==0)||(s.compare(boolType)==0)||(s.compare(floatType)==0)||(s.compare(signalType)==0);
}

vector<Attribut*> makeAtt(string contS) //lis les attributs d'une string codée sous la forme "nameAtt1:typeAtt1;nameAtt2:typeAtt2;..."
{
    vector<Attribut*> cont;
    unsigned int k=0;
    unsigned int l=0;
    bool okNameAtt=false;
    bool okTypeAtt=false;
    string nameAtt="";
    string typeAtt="";
    while(k<contS.size())//contS="x:int;y:float;..."
    {
        if(!okNameAtt)
        {
            if(contS[k]!=':')  nameAtt+=contS[k];
            else
            {
                okNameAtt=true;
                okTypeAtt=false;

            }
        }
        else if(!okTypeAtt)
        {
            if(contS[k]!=';') typeAtt+=contS[k];
            else
            {
                //okTypeAtt=true;
                okNameAtt=false;
                okTypeAtt=true;
                cont.push_back(new Attribut(nameAtt,typeAtt));
                l++;
                nameAtt="";
                typeAtt="";
            }
        }
        k++;
    }
    //cout<<"l="<<l<<endl;
    return cont;
}

vector<Instruction*> makeMeth(string cont)
{
    vector<Instruction*> ret;
    unsigned int k=0;
    string nameT="";//nom du type
    while(k<cont.size())
    {
        if(cont[k]!=';') nameT+=cont[k];
        else
        {
           ret.push_back(new Instruction(nameT));
           nameT="";
        }
        k++;
    }
    return ret;
}


/*
 * Ce constructeur sert à rechercher les types dans la bdd
 */

Type::Type(std::string _name, bool estBasic)
{
    name=_name;
    //cout<<"typename="<<name<<endl;
    //cout<<"isBasic="<<(name.compare(intType)==0)<<endl;
    if(!estBasic && _name.compare(stringType)!=0 && !isBasic(_name))
    {
        string req="select cont, descr, meth from type where name=\""+_name+"\";";
        string contS="";
        MYSQL_RES* result=memory->request(req);
        if(result!=NULL)
        {
            MYSQL_ROW row;
            if(row=mysql_fetch_row(result))
            {
                //cout<<"avant makeAtt"<<endl;
                contS= (row[0]? row[0]: "");
                cont=makeAtt(contS);
                desc=(row[1]? row[1]: "");
                //cout<<"go meth"<<endl;
                meth=makeMeth(row[2]? row[2]: "");
            }
            else
            {
                cout<<"Erreur: le type "<<name<<" n'existe pas!"<<endl;
            }
        }
        else
        {
            cout<<"erreur requête: \""+req+"\""<<endl;
        }
        mysql_free_result(result);
    }
    /*else
    {

    }*/
}

/*
 * Ce constructeur sert à créer des types de base dans la bdd
 */

Type::Type(string _name, string _desc, string _cont, string _meth)
{
    name=_name;
    desc=_desc;
    cont=makeAtt(_cont);
    meth=makeMeth(_meth);
    unsigned int k=0;
    string contS="";
    while(k<cont.size())
    {
        contS+=cont[k]->name+":"+cont[k]->type+";";
        k++;
    }
    k=0;
    string req="insert into type(name,descr,cont,meth) values(\""+_name+"\",\""+_desc+"\",\""+contS+"\",\""+_meth+"\");";
    cout<<req<<endl;
    memory->insert(req);
    string req2="create table "+name+"(";
    while(k<cont.size())
    {
        //cout<<"k="<<k<<endl;
        string actualType=cont[k]->type; //Gérer les clés étrangères
        //cout<<"actualtype: \""+actualType+"\""<<endl;
        //Si le type n'est pas un type de bases
        if((actualType.compare(intType)!=0) && (actualType.compare(boolType)!=0) && (actualType.compare(charType)!=0))
        {
           string req_tmp="select cont from type where name=\""+actualType+"\";";
           MYSQL_RES* res=memory->request(req_tmp);
           string exType=actualType;
           string pk=""; //type primary key
           string namePk=""; //name primary key
           if(res!=NULL)
           {
               MYSQL_ROW row;
               if(row=mysql_fetch_row(res))
               {
                   string pContTmp=string(row[0]? row[0]: "");
                   string contTmp=newString(pContTmp);
                   unsigned int t=0;
                   bool okType=false;
                   bool okName=true;
                   while(t<contTmp.size())
                   {
                       if(contTmp[t]==';')
                       {
                           break;
                       }
                       if(contTmp[t]==':')
                       {
                          okType=true;
                          okName=false;
                       }
                       else if(okName)
                       {
                           namePk+=contTmp[t];
                       }
                       else if(okType)
                       {
                           pk+=contTmp[t];
                       }

                    t++;
                   }
               }
               else
               {
                   cout<<"Erreur: le type "<<exType<<" n'existe pas!"<<endl;
               //mysql_free_result(res);
               }

           }
           else
           {
               cout<<"Erreur requête SQL: \""<<req_tmp<<"\""<<endl;
           }
           mysql_free_result(res);
           bool okPk=false;
           //cout<<"pk="<<pk<<endl;
           //chercher le type de base car pk n'est toujours pas un type de base!
           if(pk.size()>5)
           {
               if(pk[0]=='l' && pk[1]=='i'&& pk[2]=='s'&& pk[3]=='t' && pk[4]=='_')
               {
                   pk="int unsigned";
                   okPk=true;
               }
           }
           if(!okPk && !isBasic(pk))
           {
               //cout<<"après free pk="<<pk<<endl;
               while(!isBasic(pk))//while
               {
                   string req_pk="select cont from type where name=\""+pk+"\";";
                   cout<<"requete secondaire \""<<req_pk<<"\""<<endl;
                   MYSQL_RES* res_pk=memory->request(req_pk);
                   if(res_pk!=NULL)
                   {
                       MYSQL_ROW row_pk;
                       if(row_pk=mysql_fetch_row(res_pk))
                       {
                           string contPk=(row_pk[0]? row_pk[0]: "");
                           unsigned int l=0;
                           string exPk=pk;
                           cout<<"pk basic: "<<pk<<endl;
                           cout<<"resultat req: "<<contPk<<endl;
                           pk="";
                           bool post2P=false; //vaut true quand le premier ':' est passé
                           while(l<contPk.size())
                           {
                               if(!post2P && contPk[l]!=':')
                               {
                                   l++;
                                   continue;
                               }
                               if(contPk[l]==';')
                               {
                                   break;
                               }
                               else if(post2P)
                               {
                                 pk+=contPk[l];
                               }
                               else if(contPk[l]==':')
                               {
                                   post2P=true;
                               }

                               l++;
                           }
                           cout<<"pk type: "<<pk<<endl;
                           if(pk.compare("")==0)
                           {
                               cout<<"error empty type!"<<endl;
                               break;
                           }
                           else if(exPk.compare(pk)==0)
                           {
                               cout<<"Error le type reste inchangé"<<endl;
                               break;
                           }
                       }
                       else cout<<"type "+pk+" doesn't exist!"<<endl;
                   }
                   else cout<<"Error requete sql: \""+req_pk+"\""<<endl;
                   mysql_free_result(res_pk);
               }


           }
           if(k==0)
           {
               pk+=" primary key";
           }
           actualType=pk+", constraint `fk_"+cont[k]->name+"` foreign key ("+cont[k]->name+") references "+exType+"("+namePk+") on delete cascade";





           //mysql_free_result(res);
        }
        else if(actualType.compare(charType)==0)
        {
            actualType="char(1)";
            if(k==0)
            {
                actualType+=" primary key";
            }
        }
        else
        {
            if(k==0)
            {
                actualType+=" primary key";
            }
        }
        /*if(k==0)
        {
            req2+=cont[k]->name+" "+actualType+" primary key, ";
        }*/
        if(k==cont.size()-1)
        {
            req2+=cont[k]->name+" "+actualType+");";
        }
        else
        {
            req2+=cont[k]->name+" "+actualType+",";
        }
        k++;
    }
    cout<<req2<<endl;
    //Création de la table correspondante
    memory->request(req2);

}


/*
 * création de type container<content>
 * content=type déja connu dans la base
 */

Type::Type(string container, string content)
{
    cout<<"lol"<<endl;
    name=container+"_"+content;
    string req="select name from type where name=\""+name+"\";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            cout<<"le type "<<name<<" existe déja!"<<endl;
        }
        else
        {

            if(container.compare(listType)==0)
            {
                string req1="insert into type(name,descr,cont,meth) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int\",\"[];=;>=;==;+;\");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int);";
                cout<<req2<<endl;
                memory->request(req2);
            }
            else if(container.compare(queueType)==0)
            {
                string req1="insert into type(name,descr,cont,meth) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int\",\"[];=;>=;==;+;\");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int);";
                cout<<req2<<endl;
                memory->request(req2);
            }
            else if(container.compare(stackType)==0)
            {
                string req1="insert into type(name,descr,cont,meth) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int\",\"[];=;>=;==;+;\");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int);";
                cout<<req2<<endl;
                memory->request(req2);
            }
            else
            {
                cout<<"error, container: "<<container<<" doesn't exist!"<<endl;
            }
        }
    }
    else
    {
        cout<<"Error requête sql: \""<<req<<"\""<<endl;
    }
    mysql_free_result(res);
}


void Type::addAtt(string _name, string _type)
{
    string req="select cont from type where name=\""+name+"\";";
    MYSQL_RES* res=memory->request(req);
    string exCont="";
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            exCont=(row[0]? row[0]:"");
        }
        else
        {
            cout<<"erreur le type "+name+" n'existe pas"<<endl;
        }
    }
    else
    {
       cout<<"Error requête sql: \""<<req<<"\""<<endl;
    }
    mysql_free_result(res);
    if(exCont.compare("")!=0)
    {
        exCont+=_name+":"+_type+";";
        req="update type set cont=\""+exCont+"\" where name=\""+name+"\";";
        cout<<req<<endl;
        memory->insert(req);
        if(isBasic(_type))
        {
            string req2="alter table "+name+" add "+_name+" "+_type+";";
            cout<<req2<<endl;
            memory->insert(req2);
        }
        else if(_type.compare("string")==0)
        {
            string req2="alter table "+name+" add "+_name+" int unsigned;";
            cout<<req2<<endl;
            memory->insert(req2);
            string req3="alter table "+name+" add constraint `fk_"+_name+"` foreign key ("+_name+") references string(string_id) on delete cascade;";
            cout<<req3<<endl;
        }
        else
        {
            Type* crtType=new Type(_type);
            if(crtType->cont.size()>0)
            {
                Attribut* crtAtt=crtType->cont[0];
                string req2="alter table "+name+" add "+_name+" "+crtAtt->name+";";
                cout<<req2<<endl;
                memory->insert(req2);
                string req3="alter table "+name+" add constraint `fk_"+_name+"` foreign key ("+_name+") references "+_type+"("+crtAtt->name+") on delete cascade;";
                cout<<req3<<endl;
            }
        }
        cont.push_back(new Attribut(_name,_type));
    }
    else
    {
        cout<<"Erreur modification du type "<<name<<endl;
    }
}


void Type::deleteAtt(string _name)
{
    string newCont="";
    unsigned int k=0;
    bool okDel=false;
    for(k=0;k<cont.size();k++)
    {
        if(cont[k]->name.compare(_name)!=0)
        {
           newCont+=cont[k]->name+":"+cont[k]->type+";";
        }
        else
        {
            okDel=true;
        }
    }
    if(okDel)
    {
        string req="update type set cont=\""+newCont+"\" where name=\""+name+"\" ;";
        memory->insert(req);
        string req2="alter table "+name+" drop "+_name+";";
        memory->insert(req2);
    }
    else
    {
        cout<<"l'attribut "+_name+" n'existe pas dans la table "+name<<endl;
    }
}


void Type::modifAtt(string _name, string _type)
{
    deleteAtt(_name);
    addAtt(_name,_type);
}

void Type::deleteType()
{
    string req="delete from type where name=\""+name+"\";";
    memory->insert(req);
    string req2="drop table "+name+";";
    memory->insert(req2);
    //~Type();
}

void Type::addMeth(string _name)
{
    Instruction* ins=new Instruction(_name);
    meth.push_back(ins);
    string newCont="";
    unsigned int k=0;
    for(k=0;k<meth.size();k++)
    {
        newCont+=meth[k]->name+";";
    }
    string req="update type set meth=\""+newCont+"\";";
    memory->insert(req);
}

void Type::deleteMeth(string _name)
{
    vector<Instruction*> newMeth;
    string newCont="";
    unsigned int k=0;
    Instruction* ins;
    for(k=0;k<meth.size();k++)
    {
        if(meth[k]->name.compare(_name)!=0)
        {
            newMeth.push_back(meth[k]);
        }
        else ins=meth[k];
    }
    meth=newMeth;
    delete ins;
    for(k=0;k<meth.size();k++)
    {
        newCont+=meth[k]->name+";";
    }
    string req="update type set meth=\""+newCont+"\";";
    memory->insert(req);
}

Instruction* Type::getMeth(string id)
{
    Instruction* ret=NULL;
    if(id.compare("")!=0)
    {
        Instruction* inst=new Instruction(id);
        if(inst->ok) ret=inst;
        else delete inst;
    }
    return ret;
}

bool Type::isContainer()
{
    bool ret=false;
    if(name.size()>5)
    {
        if(name[0]=='L' && name[1]=='i' && name[2]=='s' && name[3]=='t' && name[4]=='_')
        {
            ret=true;
        }
    }
    else if(name.size()>6)
    {
        if(name[0]=='Q' && name[1]=='u' && name[2]=='e' && name[3]=='u' && name[4]=='e' && name[5]=='_')
        {
            ret=true;
        }
        else if(name[0]=='S' && name[1]=='t' && name[2]=='a' && name[3]=='c' && name[4]=='k' && name[5]=='_')
        {
            ret=true;
        }
    }
    return ret;
}


void Type::print()
{
    cout<<"Type: "<<name<<endl;
    cout<<"Description: "<<desc<<endl;
    cout<<"Argument: "<<endl;
    unsigned int k=0;
    while(k<cont.size())
    {
        cont[k]->print();
        k++;
    }
}

Type::~Type()
{
    unsigned k=0;
    for(k=0;k<cont.size();k++)
    {
        cont.pop_back();
    }
    for(k=0;k<meth.size();k++)
    {
        meth.pop_back();
    }
}
