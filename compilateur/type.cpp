#include "type.h"
#include "Database.h"
#include <string>
#include <sstream>

extern Database* memory;
using namespace std;

string stringType="string";
string intType="int";
string charType="char";
string boolType="bool";
string doubleType="double";
string signalType="signal";

string listType="list";
string queueType="queue";
string stackType="stack";

extern vector<DbVar*> context;

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
    return (s.compare(intType)==0)||(s.compare(charType)==0)||(s.compare(boolType)==0)||(s.compare(doubleType)==0)||(s.compare(signalType)==0);
}

vector<Attribut*> makeTrueAtt(string contS)
{
    vector<Attribut*> cont;
    unsigned int k=0;
    //unsigned int l=0;
    bool okNameAtt=false;
    bool okTypeAtt=false;
    string nameAtt="";
    string typeAtt="";
    for(k=0;k<contS.size();k++)//contS="x:int;y:float;..."
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
                if(nameAtt.compare("")!=0 && typeAtt.compare("")!=0)
                {
                    cont.push_back(new Attribut(nameAtt,typeAtt));
                }
                nameAtt="";
                typeAtt="";
            }
            if(k==contS.size()-1 && nameAtt.compare("")!=0 && typeAtt.compare("")!=0)
            {
                cont.push_back(new Attribut(nameAtt,typeAtt));
            }
        }
    }
    return cont;
}


vector<Attribut*> makeAtt(string contS) //lis les attributs d'une string codée sous la forme "nameAtt1:typeAtt1;nameAtt2:typeAtt2;..."
{
    vector<Attribut*> cont;
    unsigned int k=0;
    //unsigned int l=0;
    bool okNameAtt=false;
    bool okTypeAtt=false;
    string nameAtt="";
    string typeAtt="";
    for(k=0;k<contS.size();k++)//contS="x:int;y:float;..."
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
                if(nameAtt.compare("")!=0 && typeAtt.compare("")!=0)
                {
                    if(cont.size()==0)
                    {
                        if(!isBasic(typeAtt) && typeAtt.compare(stringType)!=0)
                        {
                            Type* type=new Type(typeAtt);
                            cont=type->cont;
                            delete type;
                        }
                        else
                        {
                            cont.push_back(new Attribut(nameAtt,typeAtt));
                        }
                    }
                    else
                    {
                        cont.push_back(new Attribut(nameAtt,typeAtt));
                    }
                }
                nameAtt="";
                typeAtt="";
            }
            if(k==contS.size()-1 && nameAtt.compare("")!=0 && typeAtt.compare("")!=0)
            {
                if(cont.size()==0)
                {
                    if(!isBasic(typeAtt) && typeAtt.compare(stringType)!=0)
                    {
                        Type* type=new Type(typeAtt);
                        cont=type->cont;
                        delete type;
                    }
                    else
                    {
                        cont.push_back(new Attribut(nameAtt,typeAtt));
                    }
                }
                else
                {
                    cont.push_back(new Attribut(nameAtt,typeAtt));
                }
            }
        }
    }
    //cout<<"l="<<l<<endl;
    return cont;
}

vector<unsigned int> makeMeth(string cont)
{
    vector<unsigned int> ret;
    unsigned int k=0;
    string nameT="";//nom du type
    //cout<<"cont=\""<<cont<<"\""<<endl;
    for(k=0;k<cont.size();k++)
    {
        if(cont[k]!=';') nameT+=cont[k];
        else
        {
            if(nameT.compare("")!=0)
            {
                //cout<<"nameId="<<nameT<<endl;
                stringstream ss;
                ss<<nameT;
                unsigned int actualId;
                ss>>actualId;
                ret.push_back(actualId);
                //cout<<"post new Inst dans makeMeth"<<endl;
                nameT="";
            }
        }
    }
    return ret;
}



Type::Type(Type *type)
{
    name=type->name;
    cont=type->cont;
    trueCont=type->trueCont;
    desc=type->desc;
    meth=type->meth;
    tmp=type->tmp;
}


/*
 * cette méthode permet de rendre un type non temporaire en bdd mais pas l'inverse!
 */
void Type::setTmp(bool tmp_)
{
    if(tmp && !tmp_)
    {
        tmp=tmp_;
        string tmpS=tmp? "true" : "false";
        string req="update table type set tmp="+tmpS+" where name="+name+";";
        memory->insert(req);
    }
}

/*
 * Ce constructeur sert à rechercher les types dans la bdd
 */
Type::Type(std::string _name, bool tmp_)
{
    name=_name;
    //cout<<"typename="<<name<<endl;
    //cout<<"isBasic="<<(name.compare(intType)==0)<<endl;
    if(_name.compare(stringType)!=0 && !isBasic(_name) && !isContainer())
    {
        string req="select cont, descr, meth, tmp from type where name=\""+_name+"\";";
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
                trueCont=makeTrueAtt(contS);
                desc=(row[1]? row[1]: "");
                //cout<<"go meth"<<endl;
                meth=makeMeth(row[2]? row[2]: "");
                string tmpS=(row[3]? row[3]: "");
                //cout<<"tmpS="<<tmpS<<endl;
                tmp=(tmpS.compare("0")!=0);
                //cout<<"tmp="<<boolalpha<<tmp<<endl;
            }
            else
            {
                Erreur("le type "+name+" n'existe pas!",context);
            }
        }
        else
        {
            cout<<"erreur requête: \""+req+"\""<<endl;
        }
        mysql_free_result(result);
    }
    else if(isContainer()) //on créé le type de container correspondant dans la bdd s'il n'existe pas déja
    {
         string container="";
         string content="";
         unsigned int k;
         bool okUnder=false;
         for(k=0;k<_name.size();k++)
         {
             if(_name[k]=='_')
             {
                 okUnder=true;
                 continue;
             }
             if(okUnder)
             {
                 content+=_name[k];
             }
             else
             {
                 container+=_name[k];
             }
         }
         if(container.compare("")!=0 && content.compare("")!=0)
         {
            createContainer(container,content,tmp_);
         }
    }
}


bool isContain(string name)
{
    bool ret=false;
    if(name.size()>5)
    {
        if(name[0]=='l' && name[1]=='i' && name[2]=='s' && name[3]=='t' && name[4]=='_')
        {
            ret=true;
        }
    }
    else if(name.size()>6)
    {
        if(name[0]=='q' && name[1]=='u' && name[2]=='e' && name[3]=='u' && name[4]=='e' && name[5]=='_')
        {
            ret=true;
        }
        else if(name[0]=='s' && name[1]=='t' && name[2]=='a' && name[3]=='c' && name[4]=='k' && name[5]=='_')
        {
            ret=true;
        }
    }
    return ret;
}


/*
 * Ce constructeur sert à créer des types dans la bdd
 */
Type::Type(string _name, string _desc, string _cont, string _meth, bool tmp_)
{
    name=_name;
    desc=_desc;
    tmp=tmp_;
    string tmpS=tmp? "true": "false";
    cont=makeAtt(_cont);
    trueCont=makeTrueAtt(_cont);
    cont.push_back(new Attribut("tmp",boolType));
    meth=makeMeth(_meth);
    if(!alreadyExist())
    {
        unsigned int k=0;
        string contS=_cont+"tmp:bool;";
        string req="insert into type(name,descr,cont,meth,tmp) values(\""+_name+"\",\""+_desc+"\",\""+contS+"\",\""+_meth+"\","+tmpS+");";
        //cout<<req<<endl;
        memory->insert(req);
        string req2="create table "+name+"(";
        for(k=0;k<cont.size();k++)
        {
            //cout<<"k="<<k<<endl;
            string actualType=cont[k]->type; //Gérer les clés étrangères
            //Si le type n'est pas un type de bases
            if((actualType.compare(intType)!=0) && (actualType.compare(boolType)!=0) && (actualType.compare(charType)!=0) && (!isContain(actualType)))
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
                       for(t=0;t<contTmp.size();t++)
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
                       }
                   }
                   else
                   {
                       Erreur("le type "+exType+" n'existe pas!",context);
                       break;
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
                       //cout<<"requete secondaire \""<<req_pk<<"\""<<endl;
                       MYSQL_RES* res_pk=memory->request(req_pk);
                       if(res_pk!=NULL)
                       {
                           MYSQL_ROW row_pk;
                           if(row_pk=mysql_fetch_row(res_pk))
                           {
                               string contPk=(row_pk[0]? row_pk[0]: "");
                               unsigned int l=0;
                               string exPk=pk;
                               //cout<<"pk basic: "<<pk<<endl;
                               //cout<<"resultat req: "<<contPk<<endl;
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
                               //cout<<"pk type: "<<pk<<endl;
                               if(pk.compare("")==0)
                               {
                                   Erreur("empty type!",context);
                                   break;
                               }
                               else if(exPk.compare(pk)==0)
                               {
                                   Erreur("le type reste inchangé",context);
                                   break;
                               }
                           }
                           else Erreur("Le type "+pk+" n'existe pas!",context);
                       }
                       //else cout<<"Error requete sql: \""+req_pk+"\""<<endl;
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
            else if(isContain(actualType))
            {
                string container="";
                string content="";
                bool okUnder=false;
                unsigned int l;
                //cout<<"actualType: "<<actualType<<endl;
                for(l=0;l<actualType.size();l++)
                {
                    if(actualType[l]=='_')
                    {
                        okUnder=true;
                        continue;
                    }
                    if(okUnder)
                    {
                        //cout<<"content="+content+"+"<<actualType[l]<<endl;
                        content+=actualType[l];
                    }
                    else
                    {
                        //cout<<"container="+container+"+"<<actualType[l]<<endl;
                        container+=actualType[l];
                    }
                }
                //cout<<"container="<<container<<" content="<<content<<endl;
                if(content.compare("")!=0 && container.compare("")!=0)
                {
                    string namePk=actualType+"_id";
                    createContainer(container,content,tmp_);
                    string exType=actualType;
                    actualType="int unsigned, constraint `fk_"+cont[k]->name+"` foreign key ("+cont[k]->name+") references "+exType+"("+namePk+") on delete cascade";
                }
                else
                {
                    Erreur("impossible de créer le type: "+actualType,context);
                }
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

            if(k==cont.size()-1)
            {
                req2+=cont[k]->name+" "+actualType+");";
            }
            else
            {
                req2+=cont[k]->name+" "+actualType+",";
            }
        }
        //Création de la table correspondante
        memory->request(req2);
    }
    else
    {
        Erreur("le type existe déja en bdd",context);
    }

}


bool Type::alreadyExist()
{
    bool ret=false;
    string req="select * from type where name=\""+name+"\";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            ret=true;
        }
    }
    else
    {
        cout<<"erreur requete sql: "<<req<<endl;
    }
    mysql_free_result(res);
    return ret;
}


/*
 * création de type container<content>
 * content=type déja connu dans la base
 */
void Type::createContainer(string container, string content, bool tmp_)
{
    name=container+"_"+content;
    string req="select name from type where name=\""+name+"\";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            //cout<<"le type "<<name<<" existe déja!"<<endl;
            if(!tmp_)
            {
                setTmp(tmp_);
            }
        }
        else
        {
            string idCro=getIdInstruction("[]");
            string idSet=getIdInstruction("=");
            string idEq=getIdInstruction("==");
            string idPlus=getIdInstruction("+");
            string idPlusEqual=getIdInstruction("+=");
            string tmpS=tmp_? "true": "false";
            tmp=tmp_;
            if(container.compare(listType)==0)
            {
                string req1="insert into type(name,descr,cont,meth,tmp) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int;tmp:bool;\",\""+idCro+";"+idSet+";"+idEq+";"+idPlus+";"+idPlusEqual+";\","+tmpS+");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int, tmp bool);";
                //cout<<req2<<endl;
                memory->request(req2);
            }
            else if(container.compare(queueType)==0)
            {
                string req1="insert into type(name,descr,cont,meth,tmp) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int;tmp:bool;\",\""+idCro+";"+idSet+";"+idEq+";"+idPlus+";"+idPlusEqual+";\","+tmpS+");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int, tmp bool);";
                //cout<<req2<<endl;
                memory->request(req2);
            }
            else if(container.compare(stackType)==0)
            {
                string req1="insert into type(name,descr,cont,meth,tmp) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int;tmp:bool;\",\""+idCro+";"+idSet+";"+idEq+";"+idPlus+";"+idPlusEqual+";\","+tmpS+");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int, tmp bool);";
                //cout<<req2<<endl;
                memory->request(req2);
            }
            else
            {
                Erreur("container: "+container+" doesn't exist!",context);
            }
        }
    }
    else
    {
        cout<<"Error requête sql: \""<<req<<"\""<<endl;
    }
    mysql_free_result(res);
}




/*
 * création de type container<content>
 * content=type déja connu dans la base
 */
Type::Type(string container, string content)
{
    name=container+"_"+content;
    string req="select name from type where name=\""+name+"\";";
    MYSQL_RES* res=memory->request(req);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            //cout<<"le type "<<name<<" existe déja!"<<endl;
        }
        else
        {

            if(container.compare(listType)==0)
            {
                string req1="insert into type(name,descr,cont,meth,tmp) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int;tmp:bool;\",\"[];=;>=;==;+;\");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int, tmp bool);";
                //cout<<req2<<endl;
                memory->request(req2);
            }
            else if(container.compare(queueType)==0)
            {
                string req1="insert into type(name,descr,cont,meth,tmp) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int;tmp:bool;\",\"[];=;>=;==;+;\");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int, tmp bool);";
                //cout<<req2<<endl;
                memory->request(req2);
            }
            else if(container.compare(stackType)==0)
            {
                string req1="insert into type(name,descr,cont,meth,tmp) values(\""+name+"\",\"\",\""+name+"_id:int;cont:string;size:int;tmp:bool;\",\"[];=;>=;==;+;\");";
                memory->insert(req1);
                string req2="create table "+name+"("+name+"_id int unsigned auto_increment primary key,cont int unsigned, constraint `fk_constraint_"+name+"_cont` foreign key (cont) references list_char(list_char_id) on delete cascade,size int, tmp bool);";
                //cout<<req2<<endl;
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
    bool okCreate=true;
    unsigned int k;
    for(k=0;k<cont.size();k++)
    {
        okCreate=!(cont[k]->name.compare(_name)==0);
        if(!okCreate)
        {
            Erreur("l'attribut "+_name+" existe déja dans le type "+_name,context);
            break;
        }
    }
    if(okCreate)
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
                Erreur("le type "+name+" n'existe pas",context);
            }
        }
        else
        {
           Erreur("requête sql: \""+req+"\"",context);
        }
        mysql_free_result(res);
        if(exCont.compare("")!=0)
        {
            exCont+=_name+":"+_type+";";
            req="update type set cont=\""+exCont+"\" where name=\""+name+"\";";
            memory->insert(req);
            if(isBasic(_type))
            {
                string req2="alter table "+name+" add "+_name+" "+_type+";";
                memory->insert(req2);
            }
            else if(_type.compare("string")==0)
            {
                string req2="alter table "+name+" add "+_name+" int unsigned;";
                memory->insert(req2);
                string req3="alter table "+name+" add constraint `fk_"+_name+"` foreign key ("+_name+") references string(string_id) on delete cascade;";
                memory->insert(req3);
            }
            else
            {
                Type* crtType=new Type(_type);
                if(crtType->cont.size()>0)
                {
                    Attribut* crtAtt=crtType->cont[0];
                    string req2="alter table "+name+" add "+_name+" "+crtAtt->name+";";
                    memory->insert(req2);
                    string req3="alter table "+name+" add constraint `fk_"+_name+"` foreign key ("+_name+") references "+_type+"("+crtAtt->name+") on delete cascade;";
                    memory->insert(req3);
                }
            }
            cont.push_back(new Attribut(_name,_type));
        }
        else
        {
            Erreur("lors de la modification du type "+name,context);
        }
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
        Erreur("l'attribut "+_name+" n'existe pas dans la table "+name,context);
    }
}


void Type::modifAtt(string _name, string _type)
{
    deleteAtt(_name);
    addAtt(_name,_type);
}

void Type::deleteType()
{
    if(tmp)
    {
        string req="delete from type where name=\""+name+"\";";
        memory->insert(req);
        string req2="drop table "+name+";";
        memory->insert(req2);
    }
}

void Type::addMeth(unsigned int id_)
{
    meth.push_back(id_);
    string newCont="";
    unsigned int k=0;
    string methS="";
    for(k=0;k<meth.size();k++)
    {
        stringstream ss;
        ss<<meth[k];
        string idS="";
        ss>>idS;
        methS+=idS+";";
    }
    string req="update type set meth=\""+methS+"\" where name=\""+name+"\";";
    memory->insert(req);
}

void Type::deleteMeth(unsigned int id_)
{
    string newCont="";
    unsigned int k=0;
    vector<unsigned int> newMeth;
    for(k=0;k<meth.size();k++)
    {
        if(meth[k]!=id_)
        {
            newMeth.push_back(meth[k]);
            stringstream ss;
            ss<<meth[k];
            string s="";
            ss>>s;
            newCont+=s+";";
        }
    }
    meth=newMeth;
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
        if(name[0]=='l' && name[1]=='i' && name[2]=='s' && name[3]=='t' && name[4]=='_')
        {
            ret=true;
        }
    }
    else if(name.size()>6)
    {
        if(name[0]=='q' && name[1]=='u' && name[2]=='e' && name[3]=='u' && name[4]=='e' && name[5]=='_')
        {
            ret=true;
        }
        else if(name[0]=='s' && name[1]=='t' && name[2]=='a' && name[3]=='c' && name[4]=='k' && name[5]=='_')
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
