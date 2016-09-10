#include "vargen.h"
#include "Database.h"
extern Database* memory;

using namespace std;

extern string intType;
extern string charType;
extern string boolType;
extern string floatType;
extern string stringType;
extern string signalType;

extern vector<DbVar*> context;


bool isContainer(string name)
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



/*
 *_arg est de la forme "name1:type1;name2:type2;..."
 */
std::vector<Vargen*> makeArgVar(string _arg)
{
    //cout<<"dans makeArgvar: "<<_arg<<endl;
    std::vector<Vargen*> ret;
    unsigned int k=0;
    string name="";
    string type="";
    bool okName=true;// true quand le caractère courant doit etre dans le nom courant
    bool okType=false;// idem
    string nameVar="";//nom de la variable
    for(k=0;k<_arg.size();k++)
    {
        if(_arg[k]==';')
        {
            if(isBasic(type))
            {
                //cout<<"name: "<<name<<", type:"<<type<<endl;
                ret.push_back(new Vargen(name,type));
            }
            else if(type.compare(stringType)==0)
            {
                ret.push_back(new Vargen(name,type,name));
            }
            else
            {
                ret.push_back(new Vargen(name,type));
            }
            okType=false;
            okName=true;
            name="";
            type="";
            continue;
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
        if(k==_arg.size()-1)
        {
            if(type.compare("")!=0 && name.compare("")!=0)
            {
                if(isBasic(type))
                {
                   ret.push_back(new Vargen(name,type));
                }
                else if(type.compare(stringType)==0)
                {
                    ret.push_back(new Vargen(name,type,name));
                }
                else
                {
                    ret.push_back(new Vargen(name,type));
                }
            }
        }
    }
    return ret;
}

/*
 * _arg est de la forme "var1;var2..."
 */
std::vector<Vargen*> makeArgCont(string _arg, string type)
{
    std::vector<Vargen*> ret;
    string name="";
    bool okName=true;
    unsigned int k=0;
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
            okName=true;
        }
        else if(okName)
        {
            name+=_arg[k];
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
        string idTmp=insertIntoList(_arg);
        char sizeStr[4];
        sprintf(sizeStr,"%d",_arg.size());
        string sizeTmp=sizeStr;
        if(idTmp.compare("")!=0)
        {
            string req1="insert into string(string_id,size) values("+idTmp+","+sizeTmp+");";
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



Vargen::Vargen(Vargen *var)
{
    name=var->name;
    ok=var->ok;
    type=var->type;
    if(isBasic(type->name))
    {
        string _type=type->name;
        if(_type.compare(intType)==0)
        {
            valInt=var->valInt;
        }
        else if(_type.compare(charType)==0)
        {
            valChar=var->valChar;
        }
        else if(_type.compare(floatType)==0)
        {
            valFloat=var->valFloat;
        }
        else if(_type.compare(boolType)==0)
        {
            valBool=var->valBool;
        }        
    }
    else if(type->name.compare(stringType)==0)
    {
        valStr=var->valStr;
    }
    else
    {
        unsigned int k;
        for(k=0;k<var->arg.size();k++)
        {
            arg.push_back(new Vargen(var->arg[k]));
        }
    }
}


/*
 * Insertion dans la table list_char
 * return l'identifiant
 */
string insertIntoList(string _arg)
{
    string idTmp="";
    char sizeTmp[4];
    sprintf(sizeTmp,"%d",_arg.size());
    string req1="insert into list_char(cont,size) values(\""+_arg+"\","+sizeTmp+");";
    memory->insert(req1);
    string req2="select list_char_id,cont from list_char where list_char_id in (select max(list_char_id) from list_char);";
    MYSQL_RES* res=memory->request(req2);
    if(res!=NULL)
    {
        MYSQL_ROW row;
        if(row=mysql_fetch_row(res))
        {
            idTmp=(row[0]? row[0]: "");
            string storeCont=(row[1]? row[1]: "");
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
    }
    else
    {
        cout<<"Erreur requete sql:"<<req2<<endl;
    }
    return idTmp;
}

/*
 * ce constructeur crée une variable dans la bdd ou en recherche une existante de nom _name
 * _type="nameType"
 * Si _arg="nameVar1:type1;nameVar2:type2;..." alors il créé une nouvelle variable sinon il la recherche dans la bdd
 * arg="val" ssi la variable est basique ou si son type est string
 * isNew n'est pas encore utilisé
 */
Vargen::Vargen(string _name, string _type, string _arg, bool _tmp)
{
    name=_name;
    type=new Type(_type);
    tmp=_tmp;
    if(_type.compare("string")==0)
    {
        valStr=_arg;
        storeString(_arg);
    }
    else if(type->isContainer())
    {
        string typeCont="";
        unsigned int i;
        bool under=false;
        while(i<_type.size())
        {
            if(_type[i]=='_' && !under)
            {
                under=true;
                continue;
            }
            if(under)
            {
                typeCont+=_type[i];
            }
            else
            {
                continue;
            }
        }
        arg=makeArgCont(_arg,typeCont);
        //on recherche si le conteneur existe déja en bdd _arg=val1;val2;...;
        string req="select list_char_id from list_char where cont=\""+_arg+"\";";
        MYSQL_RES* res=memory->request(req);
        bool createCont=false;
        if(res!=NULL)
        {
            MYSQL_ROW row;
            if(row=mysql_fetch_row(res))//si le conteneur existe déja, il n'y a rien à faire
            {

            }
            else// sinon il faut le créer
            {
                createCont=true;
            }
        }
        else
        {
           cout<<"Erreur requete sql: \""+req+"\""<<endl;
        }
        mysql_free_result(res);
        if(createCont)
        {
            string idTmp=insertIntoList(_arg);
            if(idTmp.compare("")!=0)
            {
                char sizeTmp[4];
                sprintf(sizeTmp,"%d",arg.size());
                string req1="insert into "+_type+"("+_type+"_id,size) values("+idTmp+","+sizeTmp+");";
                //cout<<"requete finale: "<<req1<<endl;
                memory->insert(req1);
            }

        }
    }
    else
    {
        //cout<<"avant makeArgvar"<<endl;
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
                    delete type;
                    type=new Type(_type,true);
                    char* valTmp=(char*)val.c_str();
                    int res=sscanf(valTmp,"%d",&valInt);
                    sscanf(valTmp,"%*[\n]");
                    if(res==1)
                    {
                        ok=true;
                    }
                    else
                    {
                        valInt=0;
                        ok=false;
                    }
                }
                else if(_type.compare("char")==0)
                {
                    delete type;
                    type=new Type(_type,true);
                    char* valTmp=(char*)val.c_str();
                    char c='\0';
                    int res=sscanf(valTmp,"%c",&c);
                    valChar=c;
                    sscanf(valTmp,"%*[\n]");
                    ok=(res==1);
                    //cout<<"valChar="<<valChar<<endl;
                    if(!ok)
                    {
                        valChar='\0';
                    }
                }
                else if(_type.compare("bool")==0)
                {
                    delete type;
                    type=new Type(_type,true);
                    //cout<<"bool="<<val<<endl;
                    valBool=(val.compare("true")==0);
                    ok=true;
                }
                else if(_type.compare("string")==0)
                {
                    delete type;
                    type=new Type(_type,true);
                    valStr=val;
                    ok=true;
                }
                else if(_type.compare("float")==0)
                {
                    delete type;
                    type=new Type(_type,true);
                    char* valTmp=(char*)val.c_str();
                    int res=sscanf(valTmp,"%f",&valFloat);
                    sscanf(valTmp,"%*[\n]");
                    ok=(res==1);
                    if(!ok)
                    {
                        valFloat=0;
                    }
                }
                else if(_type.compare(signalType)==0)
                {
                    delete type;
                    type=new Type(_type,true);
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
                        else if(isContainer(crtAtt->type))
                        {

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
                                   Erreur("la variable "+_name+" n'existe pas dans la table "+_type,context);
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
                                    Erreur("la variable "+_name+" n'existe pas dans la table "+_type,context);
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
                        Erreur("la variable "+val+" n'existe pas dans la table "+type->name,context);
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
                    Erreur("Erreur de lecture de l'attribut "+crtAtt->name+" de la variable "+val,context);
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
    if(!isBasic(type->name) && tmp)
    {
        if(type->cont.size()>0)
        {
            string req="delete from "+type->name+" where "+type->cont[0]->name+"="+name+";";
            memory->insert(req);
        }
        else if(type->name.compare(stringType)==0)
        {
            string id=getIdString(valStr);
            string req="delete from list_char where list_char_id="+id+";";
            memory->insert(req);
        }
    }
    //~Vargen();
}


void Vargen::setVal(Vargen *val)
{
    if(type->name.compare(intType)==0)
    {
        if(val->type->name.compare(intType)==0)
        {
            valInt=val->valInt;
        }
        else
        {
            Erreur("Aucun opérateur = entre "+type->name+" et "+val->type->name,context);
        }
    }
    else if(type->name.compare(floatType)==0)
    {
        if(val->type->name.compare(intType)==0)
        {
            valFloat=val->valInt;
        }
        else if(val->type->name.compare(floatType)==0)
        {
            valFloat=val->valInt;
        }
        else
        {
            Erreur("Aucun opérateur = entre "+type->name+" et "+val->type->name,context);
        }
    }
    else if(type->name.compare(charType)==0)
    {
        if(val->type->name.compare(charType)==0)
        {
           valChar=val->valChar;
        }
        else
        {
            Erreur("Aucun opérateur = entre "+type->name+" et "+val->type->name,context);
        }
    }
    else if(type->name.compare(boolType)==0)
    {
        if(val->type->name.compare(boolType)==0)
        {
           valBool=val->valBool;
        }
        else
        {
            Erreur("Aucun opérateur = entre "+type->name+" et "+val->type->name,context);
        }
    }
    else if(type->name.compare(stringType)==0)
    {
        if(val->type->name.compare(charType)==0)
        {
           valStr=val->valChar;
        }
        else if(val->type->name.compare(intType)==0)
        {
            char tmpInt[4];
            sprintf(tmpInt,"%d",val->valInt);
            valStr=tmpInt;
        }
        else if(val->type->name.compare(floatType)==0)
        {
            char tmpFloat[8];
            sprintf(tmpFloat,"%f",val->valFloat);
            valStr=tmpFloat;
        }
        else if(val->type->name.compare(stringType)==0)
        {
            valStr=val->valStr;
        }
        else
        {
            Erreur("Aucun opérateur = entre "+type->name+" et "+val->type->name,context);
        }
    }
    else if(type->name.compare(val->type->name)==0)
    {
        if(arg.size()==val->arg.size())
        {
            unsigned int k;
            for(k=0;k<arg.size();k++)
            {
                arg[k]->setVal(val->arg[k]);
            }
        }
    }
    else
    {
        Erreur("Aucun opérateur = entre "+type->name+" et "+val->type->name,context);
    }
}


Instruction* Vargen::getMeth(string name, string argT, string retourT)
{
    string id=getIdInstruction(name,argT,retourT);
    return type->getMeth(id);
}


void Vargen::print()
{
    cout<<"->Name:"<<name<<endl;
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
