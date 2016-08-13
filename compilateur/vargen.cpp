#include "vargen.h"
#include "Database.h"
extern Database* memory;

using namespace std;


/*
 *_arg est de la forme "name1:type1;name2:type2;..."
 */
std::vector<Vargen*> makeArgVar(string _arg)
{
    std::vector<Vargen*> ret;
    unsigned int k=0;
    string name="";
    string type="";
    bool okName=true;// true quand le caractère courant doit etre dans le nom courant
    bool okType=false;// idem
    string nameVar="";//nom de la variable
    while(k<_arg.size())
    {
        if(_arg[k]==';')
        {
            if(isBasic(type) || type.compare("string")==0)
            {
               ret.push_back(new Vargen(name,type,name));
            }
            else
            {
                ret.push_back(new Vargen(name,type));
            }
            okType=false;
            okName=true;
        }
        if(_arg[k]==':')
        {
            okName=false;
            okType=true;
        }
        else if(okName)
        {
            name+=_arg[k];
        }
        else if(okType)
        {
            type+=_arg[k];

        }
        k++;
    }
    return ret;
}

/*
 *Créée la string _arg en bdd si elle n'existe pas
 */

void storeString(string _arg)
{
    //on recherche si la string existe déja en bdd
    string req="select cont from list_char where cont=\""+_arg+"\";";
    MYSQL_RES* res=memory->request(req);
    bool createStr=false;
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))//si la string exite déja, il n'y a rien à faire
        {

        }
        else// sinon il faut la créer
        {
            createStr=true;
        }
    }
    else
    {
       cout<<"Erreur requete sql: \""+req+"\""<<endl;
    }
    mysql_free_result(res);
    if(createStr)
    {
        char sizeTmp[4];
        sprintf(sizeTmp,"%d",_arg.size());
        string req1="insert into list_char(cont,size) values(\""+_arg+"\","+sizeTmp+");";
        memory->insert(req1);

        req1="select list_char_id, cont from list_char where cont=\""+_arg+"\";";
        string idTmp="";
        res=memory->request(req1);
        string storeCont="";
        if(res!=NULL)
        {
            MYSQL_ROW row;
            if(row=mysql_fetch_row(res))
            {
               idTmp=(row[0]? row[0]: "");
               storeCont=(row[1]? row[1]: "");
               char size2Tmp[4];
               unsigned int size2=2*_arg.size();
               sprintf(size2Tmp,"%d",size2);
               string size2Str=size2Tmp;
               if(storeCont.compare(_arg)!=0)
               {
                   cout<<"augmentation de la capacité des string"<<endl;
                   string req2="alter table list_char modify cont varchar("+size2Str+");";
                   memory->insert(req2);
                   req2="update list_char set cont=\""+_arg+"\" where list_char_id="+idTmp+";";
                   memory->insert(req2);
               }
            }
            else
            {
                cout<<"erreur insert into list_char a echoué"<<endl;
            }
        }
        else
        {
            cout<<"Erreur requete sql: \""+req+"\""<<endl;
        }
        if(idTmp.compare("")!=0)
        {
            req1="insert into string(string_id,size) values("+idTmp+","+sizeTmp+");";
            //cout<<"requete finale: "<<req1<<endl;
            memory->insert(req1);
        }

    }
}

/*
 * Ce constructeur cherche une variable de nom "_name" dans la bdd
 * val est non vide ssi la variable est basique
 */
/*Vargen::Vargen(string _name, string _type, string val)
{
    name=_name;
    if(_type.compare("")!=0)
    {

        if(_type.compare("int")==0)
        {
            type=new Type(_type,true);
            char* valTmp=(char*)val.c_str();
            sscanf(valTmp,"%d",&valInt);
        }
        else if(_type.compare("char")==0)
        {
            type=new Type(_type,true);
            char* valTmp=(char*)val.c_str();
            sscanf(valTmp,"%c",&valChar);
            ok=true;
        }
        else if(_type.compare("bool")==0)
        {
            type=new Type(_type,true);
            valBool=(val.compare("true")==0);
            ok=true;
        }
        else if(_type.compare("string")==0)
        {
            type=new Type(_type,true);
            valStr=val;
            ok=true;
        }
        else
        {
            type=new Type(_type);
            unsigned int k=0;
            std::vector<int> idString;
            for(k=0;k<type->cont.size();k++)
            {
                string crtName="";
                Attribut* crtAtt=type->cont[k];
                if(crtAtt->type.compare("string")==0)
                {
                    string req="select cont from list_char where list_char_id=\""+crtAtt->name+"\";";
                    MYSQL_RES* res=memory->request(req);
                    if(res!=NULL)
                    {
                       MYSQL_ROW row;
                       if(row=mysql_fetch_row(res))
                       {
                           string crtArg=(row[0]? row[0]:"");
                           arg.push_back(new Vargen(crtAtt->name,"string",crtArg));
                       }
                       else
                       {
                           cout<<"erreur: string id="<<crtAtt->name<<" introuvable"<<endl;
                       }
                    }
                    else
                    {
                       cout<<"Erreur requete sql: \""+req+"\""<<endl;
                    }
                    mysql_free_result(res);
                }
                else if(isBasic(crtAtt->type))
                {
                    string req="select "+crtAtt->name+" from "+_type+" where "+type->cont[0]->name+"="+_name+";";
                    string crtVal="";
                    MYSQL_RES* res=memory->request(req);
                    if(res!=NULL)
                    {
                        MYSQL_ROW row;
                        if(row=mysql_fetch_row(res))
                        {
                            crtVal=(row[0]? row[0]:"");
                        }
                        else
                        {
                           cout<<"erreur: la variable "+_name+" n'existe pas dans la table "+_type<<endl;
                        }
                    }
                    else
                    {
                       cout<<"Erreur requete sql: \""+req+"\""<<endl;
                    }
                    if(crtVal.compare("")!=0)
                    {
                        arg.push_back(new Vargen(crtAtt->name,crtAtt->type,crtVal));
                    }
                    mysql_free_result(res);
                }
                else
                {
                    string req="select "+crtAtt->name+" from "+_type+" where "+type->cont[0]->name+"="+_name+";";
                    MYSQL_RES*res=memory->request(req);
                    if(res!=NULL)
                    {
                        MYSQL_ROW row;
                        if(row=mysql_fetch_row(res))
                        {
                            crtName=(row[0]? row[0]:"");
                        }
                        else
                        {
                            cout<<"erreur: la variable "+_name+" n'existe pas dans la table "+_type<<endl;
                        }
                    }
                    else
                    {
                        cout<<"Erreur requete sql: \""+req+"\""<<endl;
                    }
                    mysql_free_result(res);
                    if(crtName.compare("")!=0)
                    {
                        arg.push_back(new Vargen(crtName,crtAtt->type));
                    }
                }

            }
            ok=(arg.size()==type->cont.size());
        }
    }
    else
    {
        ok=false;
    }
}
*/



/*
 * ce constructeur crée une variable dans la bdd ou en recherche une exitente de nom _name
 * _type="nameType"
 * Si _arg="nameVar1:type1;nameVar2:type2;..." alors il créé une nouvelle variable sinon il la recherche dans la bdd
 * arg="val" ssi la variable est basique ou si son type est string
 * isNew n'est pas encore utilisé
 */

Vargen::Vargen(string _name, string _type, string _arg)
{
    name=_name;
    type=new Type(_type);

    if(_type.compare("string")==0)
    {
        valStr=_arg;
        //on recherche si la string existe déja en bdd
        string req="select cont from list_char where cont=\""+_arg+"\";";
        MYSQL_RES* res=memory->request(req);
        bool createStr=false;
        if(res!=NULL)
        {
            MYSQL_ROW row;
            if(row=mysql_fetch_row(res))//si la string exite déja, il n'y a rien à faire
            {

            }
            else// sinon il faut la créer
            {
                createStr=true;
            }
        }
        else
        {
           cout<<"Erreur requete sql: \""+req+"\""<<endl;
        }
        mysql_free_result(res);
        if(createStr)
        {
            char sizeTmp[4];
            sprintf(sizeTmp,"%d",_arg.size());
            string req1="insert into list_char(cont,size) values(\""+_arg+"\","+sizeTmp+");";
            memory->insert(req1);

            req1="select list_char_id from list_char where cont=\""+_arg+"\";";
            string idTmp="";
            res=memory->request(req1);
            if(res!=NULL)
            {
                MYSQL_ROW row;
                if(row=mysql_fetch_row(res))
                {
                   idTmp=(row[0]? row[0]: "");
                }
                else
                {
                    cout<<"erreur insert into list_char a echoué"<<endl;
                }
            }
            else
            {
                cout<<"Erreur requete sql: \""+req+"\""<<endl;
            }
            if(idTmp.compare("")!=0)
            {
                req1="insert into string(string_id,size) values("+idTmp+","+sizeTmp+");";
                //cout<<"requete finale: "<<req1<<endl;
                memory->insert(req1);
            }

        }
    }
    else
    {
        arg=makeArgVar(_arg);
        if(arg.size()>0) //création d'une nouvelle variable dans la bdd
        {
            string req="insert into "+type->name;
            unsigned int k=0;
            req+="(";
            while(k<type->cont.size()) //on rasemble le nom des colonnes
            {
                string actualType=type->cont[k]->name;
                if(k==arg.size()-1)
                {
                   req+="\""+actualType+"\")";
                }
                else
                {
                    req+="\""+actualType+"\",";
                }
                k++;
            }
            k=0;
            req+=" values(";
            while(k<arg.size()) //on rasemble les valeurs des colonnes
            {
                string actualValue=arg[k]->name;
                if(k==arg.size()-1)
                {
                    req+="\""+actualValue+"\"";
                }
                else
                {
                    req+="\""+actualValue+"\",";
                }
                k++;

            }
            k=0;
            req+=");";
            memory->insert(req);
            ok=true;
        }
        else //tentative de recherhe d'une variable de nom "_name" dans la bdd
        {
            string val=_arg;
            if(_type.compare("")!=0)
            {

                if(_type.compare("int")==0)
                {
                    type=new Type(_type,true);
                    char* valTmp=(char*)val.c_str();
                    sscanf(valTmp,"%d",&valInt);
                    ok=true;
                }
                else if(_type.compare("char")==0)
                {
                    type=new Type(_type,true);
                    char* valTmp=(char*)val.c_str();
                    sscanf(valTmp,"%c",&valChar);
                    ok=true;
                }
                else if(_type.compare("bool")==0)
                {
                    type=new Type(_type,true);
                    valBool=(val.compare("true")==0);
                    ok=true;
                }
                else if(_type.compare("string")==0)
                {
                    type=new Type(_type,true);
                    valStr=val;
                    ok=true;
                }
                else
                {
                    type=new Type(_type);
                    unsigned int k=0;
                    std::vector<int> idString;
                    for(k=0;k<type->cont.size();k++)
                    {
                        string crtName="";
                        Attribut* crtAtt=type->cont[k];
                        if(crtAtt->type.compare("string")==0)
                        {
                            string req="select cont from list_char where list_char_id=\""+crtAtt->name+"\";";
                            MYSQL_RES* res=memory->request(req);
                            if(res!=NULL)
                            {
                               MYSQL_ROW row;
                               if(row=mysql_fetch_row(res))
                               {
                                   string crtArg=(row[0]? row[0]:"");
                                   arg.push_back(new Vargen(crtAtt->name,"string",crtArg));
                               }
                               else
                               {
                                   cout<<"erreur: string id="<<crtAtt->name<<" introuvable"<<endl;
                               }
                            }
                            else
                            {
                               cout<<"Erreur requete sql: \""+req+"\""<<endl;
                            }
                            mysql_free_result(res);
                        }
                        else if(isBasic(crtAtt->type))
                        {
                            string req="select "+crtAtt->name+" from "+_type+" where "+type->cont[0]->name+"="+_name+";";
                            string crtVal="";
                            MYSQL_RES* res=memory->request(req);
                            if(res!=NULL)
                            {
                                MYSQL_ROW row;
                                if(row=mysql_fetch_row(res))
                                {
                                    crtVal=(row[0]? row[0]:"");
                                }
                                else
                                {
                                   cout<<"erreur: la variable "+_name+" n'existe pas dans la table "+_type<<endl;
                                }
                            }
                            else
                            {
                               cout<<"Erreur requete sql: \""+req+"\""<<endl;
                            }
                            if(crtVal.compare("")!=0)
                            {
                                arg.push_back(new Vargen(crtAtt->name,crtAtt->type,crtVal));
                            }
                            mysql_free_result(res);
                        }
                        else
                        {
                            string req="select "+crtAtt->name+" from "+_type+" where "+type->cont[0]->name+"="+_name+";";
                            MYSQL_RES*res=memory->request(req);
                            if(res!=NULL)
                            {
                                MYSQL_ROW row;
                                if(row=mysql_fetch_row(res))
                                {
                                    crtName=(row[0]? row[0]:"");
                                }
                                else
                                {
                                    cout<<"erreur: la variable "+_name+" n'existe pas dans la table "+_type<<endl;
                                }
                            }
                            else
                            {
                                cout<<"Erreur requete sql: \""+req+"\""<<endl;
                            }
                            mysql_free_result(res);
                            if(crtName.compare("")!=0)
                            {
                                arg.push_back(new Vargen(crtName,crtAtt->type));
                            }
                        }

                    }
                    ok=(arg.size()==type->cont.size());
                }
            }
            else
            {
                ok=false;
            }
        }
    }
}

void Vargen::setValue(string val)
{
    if(isBasic(type->name))
    {
        if(type->name.compare("int")==0)
        {
            char* valTmp=(char*)val.c_str();
            sscanf(valTmp,"%d",&valInt);
            ok=true;
        }
        else if(type->name.compare("char")==0)
        {
            char* valTmp=(char*)val.c_str();
            sscanf(valTmp,"%c",&valChar);
            ok=true;
        }
        else if(type->name.compare("bool")==0)
        {
            valBool=(val.compare("true")==0);
            ok=true;
        }
    }
    else if(type->name.compare("string")==0)
    {
        valStr=val;
        storeString(val); //on stocke val en bdd si elle n'existe pas déja
    }
    else
    {
        unsigned int k=0;
        if(type->cont.size()==arg.size())
        {
            for(k=0;k<type->cont.size();k++)
            {
                Attribut* crtAtt=type->cont[k];
                string crtVal="";
                string req="select "+crtAtt->name+" from "+type->name+" where "+type->cont[0]->name+"="+val+";";
                MYSQL_RES* res=memory->request(req);
                if(res!=NULL)
                {
                    MYSQL_ROW row;
                    if(row=mysql_fetch_row(res))
                    {
                        crtVal=(row[0]? row[0]: "");
                    }
                    else
                    {
                        cout<<"Erreur la variable "<<val<<" n'existe pas dans la table "<<type->name<<endl;
                    }
                }
                else
                {
                   cout<<"Erreur requete sql: \""+req+"\""<<endl;
                }
                mysql_free_result(res);
                if(crtVal.compare("")!=0)
                {
                    Vargen* tmp=arg[k];
                    if(crtAtt->type.compare("string")==0)
                    {
                        crtVal=getStringId(crtVal); //on obtient le contenu de la string à l'aide de son id
                    }
                    arg[k]=new Vargen(crtVal,crtAtt->type,crtVal);
                    delete tmp;
                    string req2="update table "+type->name+" set "+crtAtt->name+"="+crtVal+" where "+type->cont[0]->name+"="+name+";";
                    memory->insert(req2);
                }
                else
                {
                    cout<<"Erreur de lecture de l'attribut "<<crtAtt->name<<" de la variable "<<val<<endl;
                }
            }

        }
    }
}


Vargen* Vargen::getAtt(string nameAtt)
{
    unsigned int k=0;
    Vargen* ret=NULL;
    if(arg.size()==type->cont.size())
    {
        for(k=0;k<type->cont.size();k++)
        {
            if(type->cont[k]->name.compare(nameAtt)==0)
            {
                break;
            }
        }
        ret=arg[k];
    }
    return ret;
}


Instruction* Vargen::getMeth(string id)
{
    return type->getMeth(id);
}


void Vargen::deleteVar()
{
    if(!isBasic(type->name))
    {
        if(type->cont.size()>0)
        {
            string req="delete from "+type->name+" where "+type->cont[0]->name+"="+name+";";
            memory->insert(req);
        }
    }
    //~Vargen();
}

void Vargen::print()
{
    cout<<"->Name:"<<endl;
    cout<<"->Type: "<<type->name<<endl;
    if(isBasic(type->name)||type->name.compare("string")==0)
    {
        if(type->name.compare("int")==0)
        {
            cout<<"->val="<<valInt<<endl;
        }
        else if(type->name.compare("string")==0)
        {
            cout<<"->val="<<valStr<<endl;
        }
        else if(type->name.compare("bool")==0)
        {
            cout<<"->val="<<valBool<<endl;
        }
        else if(type->name.compare("char")==0)
        {
            cout<<"->val="<<valChar<<endl;
        }

    }
    else if(ok)
    {
        unsigned int k=0;
        if(arg.size()==type->cont.size())
        {
            for(k=0;k<arg.size();k++)
            {
                string nameAtt=type->cont[k]->name;
                cout<<"->"<<nameAtt<<":";
                arg[k]->print();
            }
        }
    }
    else
    {
        cout<<"cette variable est mal initialisée"<<endl;
    }
}


Vargen::~Vargen()
{
    delete type;
    arg.clear();
}
