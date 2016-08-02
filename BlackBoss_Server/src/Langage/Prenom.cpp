#include "../../include/Langage/Prenom.h"
//#include "../../include/IA/BlackIa.h"
#include "../../include/IA/phrase.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/DatabaseFun.h"
//extern InBlackIa* InBoss;

using namespace std;

extern Database* memory;
extern MYSQL data;
extern string login;
extern string mdp;
extern string dbname;
extern string Adr;
extern string Pre;

extern DatabaseMot* Dmots;
extern DatabaseFun* Dfun;


Prenom::Prenom(std::string fname)
{
    basicName=fname;
    ident=fname;
    catMot=Pre;
    isDenm=false;
    isMasc=true;
    isFem=true;
    string query="select isMasc,isFem,fun from prenom natural join mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    //cout<<"query :"<<query<<endl;
    MYSQL_RES* result=memory->request(query);
    MYSQL_ROW row;
    if(result!=NULL)
    {
        if(row=mysql_fetch_row(result))
        {
;
            int nMasc;
            sscanf(row[0],"%d",&nMasc);
            sscanf(row[0],"%*[^\n]");
            isMasc=(nMasc==1);
            int nFem;
            sscanf(row[1],"%d",&nFem);
            sscanf(row[1],"%*[^\n]");
            isFem=(nFem==1);
            string ffun=row[2]? row[2]:"";
            Phrase* ph=new Phrase(ffun);
            int k;
            for(k=0;k<ph->size;k++)
            {
                Pf actualFun=Dfun->find(ph->exp[k]);
                foncs.push_back(actualFun);
            }
            delete ph;
        }
    }
   
    mysql_free_result(result);
    //cout<<"ok prenom!"<<endl;
    Dmots->insert(this);
}

Prenom::Prenom(std::string fname, bool fIsMasc, bool fIsFem)
{
    basicName=fname;
    ident=fname;
    //cout<<"nouveau prenom :"<<basicName<<endl;
    catMot="prenom";
    isDenm=false;
    isMasc=fIsMasc;
    isFem=fIsFem;
    string query="insert into mot(basicName,type) values(\""+basicName+"\",\""+catMot+"\");";
    //cout<<"requete sql: "<<query<< endl;
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2);
    
    string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    string c_isMasc=(isMasc)? ("true") : ("false");
    string c_isFem=(isFem)? ("true") : ("false");
    string query3="insert into prenom(pre_id,basicName,isMasc,isFem) values(\""+s_id+"\",\""+basicName+"\","+c_isMasc+","+c_isFem+");";
    //cout<<"query= "<<query3<<endl;
    memory->insert(query3);
    Dmots->insert(this);
}

Prenom::Prenom(string fname, bool sexe, fstream* dataF)
{
    basicName=fname;
    ident=fname;
    //cout<<"nouveau prenom :"<<basicName<<endl;
    catMot="prenom";
    isDenm=false;
    isMasc=sexe;
    isFem=!sexe;
    string query="insert into mot(basicName,type) values(\""+basicName+"\",\""+catMot+"\");";
    //cout<<"requete sql: "<<query<< endl;
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2,dataF);
    
    string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    string c_isMasc=(isMasc)? ("true") : ("false");
    string c_isFem=(isFem)? ("true") : ("false");
    string query3="insert into prenom(pre_id,basicName,isMasc,isFem) values("+s_id+"\""+basicName+"\",\""+catMot+"\","+c_isMasc+","+c_isFem+");";
    memory->insert(query3,dataF);
    
    //BlackBoss->mots->insert(this);
    //mot=fname;
}

Prenom::Prenom(std::string fname, bool fIsMasc, bool fIsFem, fstream* dataF)
{
    basicName=fname;
    ident=fname;
    //cout<<"nouveau prenom :"<<basicName<<endl;
    catMot="prenom";
    isDenm=false;
    isMasc=fIsMasc;
    isFem=fIsFem;
    string query="insert into mot(basicName,type) values(\""+basicName+"\",\""+catMot+"\");";
    //cout<<"requete sql: "<<query<< endl;
    string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    memory->insert(query,query2,dataF);
    
    string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    string c_isMasc=(isMasc)? ("true") : ("false");
    string c_isFem=(isFem)? ("true") : ("false");
    string query3="insert into prenom(pre_id,basicName,isMasc,isFem) values("+s_id+"\""+basicName+"\",\""+catMot+"\","+c_isMasc+","+c_isFem+");";
    memory->insert(query3,dataF);
    Dmots->insert(this);
    //mot=fname;
}

Prenom::Prenom(string fname, bool sexe, string define)
{
    basicName=fname;
    ident=fname;
    catMot="prenom";
    isDenm=false;
    isMasc=sexe;
    isFem=!sexe;
    //mot=fname;
    def=define;
    Dmots->insert(this);
}

Prenom::Prenom(std::string fname, fstream* dataF, bool fIsMasc, bool fIsFem)
{
    basicName=fname;
    catMot="prenom";
    ident=fname;
    
        MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        string query="select basicName,isMasc,isFem from prenom where basicName=\""+basicName+"\";";
        //cout<<query<<endl;
        mysql_query(&data,query.c_str());
        result=mysql_store_result(&data);
        if(result!=NULL)
        {
            if((row=mysql_fetch_row(result))) //Si le prenom est deja connu
            {
                basicName=row[0]? row[0]:"";
                int n,p;
                sscanf(row[1],"%d",&n);
                sscanf(row[1],"%*[^\n]");
                sscanf(row[2],"%d",&n);
                sscanf(row[2],"%*[^\n]");
                isMasc= (n==1);
                isFem=(p==1);
                //BlackBoss->mots->insert(this);
            }
            else //sinon on le créé
            {
                isMasc=fIsMasc;
                isFem=fIsFem;
                string query="insert into mot(basicName,type) values(\""+basicName+"\",\""+catMot+"\");";
                string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
                memory->insert(query,query2,dataF);
                string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
                int mot_id=memory->getIntAt(query4);
                char charMotId[4];
                sprintf(charMotId,"%d",mot_id);
                string s_id=charMotId;
                string s_isMasc=(isMasc)? ("true"):("false");
                string s_isFem=(isFem)? ("true"):("false");
                string query3="insert into prenom(pre_id, basicName,isMasc,isFem) values("+s_id+",\""+basicName+"\","+s_isMasc+","+s_isFem+");";

                memory->insert(query3,dataF);
                //BlackBoss->mots->insert(this);
            }
            mysql_free_result(result);
        }
    
}

Prenom::~Prenom()
{
    //dtor
}
