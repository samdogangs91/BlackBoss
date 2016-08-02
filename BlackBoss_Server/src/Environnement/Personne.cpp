#include "../../include/Environnement/Personne.h"
#include "../../include/Database/Database.h"
#include "../../include/IA/phrase.h"
#include "../../include/Environnement/User.h"

using namespace std;

extern User* Vous;

extern Database* memory;

Personne::Personne()
{
    //ctor
}


Personne::Personne(std::string fname)
{
    Phrase* ph=new Phrase(fname);
    string query="";
    knowNom=false;
    knowPrenom=false;
    knowSexe=false;
    if(ph->size==1)
    {
        query="select sexe,prenom,nom,pseudo,def,age,taille,poids,hairColor,eyeColor,skinColor,id from personne where prenom=\""+fname+"\" or pseudo=\""+fname+"\";";
    }
    else if(ph->size=3)
    {
        query="select sexe,prenom,nom,pseudo,def,age,taille,poids,hairColor,eyeColor,skinColor,id from personne where prenom=\""+ph->exp[0]+"\" and nom=\""+ph->exp[2]+"\";";
    }
    if(query.compare("")!=0)
    {
        MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        result=memory->request(query);
        if(result!=NULL)
        {
            if(row=mysql_fetch_row(result))
            {
                knowSexe=true;
                knowPrenom=true;
                knowNom=true;
                string s_sexe=row[0]? row[0]:"";
                sexe=(s_sexe.compare("M")==0);
                string s_pre=row[1]? row[1]: "";
                prenom=new Prenom(s_pre);
                string s_nom=row[2]? row[2]: "";
                nom= new NomPropre(s_nom);
                string s_pse=row[3]? row[3]: "";
                pseudo= new Prenom(s_pse);
                def=row[4]? row[4]:"";
                string s_age=row[5]? row[5]: "";
                sscanf(s_age.c_str(),"%d",&age);
                string s_taille=row[6]? row[6]: "";
                sscanf(s_taille.c_str(),"%d",&taille);
                string s_poids=row[7]? row[7]: "";
                sscanf(s_poids.c_str(),"%d",&poids);
                hairColor=row[8]? row[8]:"";
                eyeColor=row[9]? row[9]:"";
                skinColor=row[10]? row[10]:"";
                ident=row[11]? row[11] : "";
                /*ident=prenom->basicName;
                ident+=" ";
                ident+=nom->basicName;
                ident+=" ";
                int k=0;
                string ident_tmp;
                while(1)
                    {
                    char buff[4];
                    sprintf(buff,"%d",k);
                    ident_tmp=ident;
                    ident_tmp+=buff;
                    if(InBoss->pers->db.count(ident_tmp)==0)
                        {
                        break;
                        }
                    k++;
                    }
                ident=ident_tmp;
                identPre=prenom->basicName;
                identPre+=" ";
                k=0;
                while(1)
                {
                    char buff[4];
                    sprintf(buff,"%d",k);
                    ident_tmp=identPre;
                    ident_tmp+=buff;
                    if(InBoss->persPre->db.count(ident_tmp)==0)
                        {
                        break;
                        }
                    k++;
                }
                identPre=ident_tmp;
                InBoss->pers->db.insert(pair<string,Personne*>(this->ident,this));
                InBoss->persPre->db.insert(pair<string,Personne*>(identPre,this));*/
            }
        }
        mysql_free_result(result);
    }
    delete ph;
    
}


Personne::Personne(Prenom* _pre, NomPropre* _nom, Prenom* _pse, bool _sexe, std::string _def, int _age, int _taille, int _poids, std::string _skin, std::string _hair, std::string _eye, std::fstream* dataF) {
    
    prenom=_pre;
    nom=_nom;
    pseudo=_pse;
    sexe=_sexe;
    def=_def;
    age=_age;
    taille=_taille;
    poids=_poids;
    skinColor=_skin;
    hairColor=_hair;
    eyeColor=_eye;
    ident=prenom->basicName;
    ident+=" ";
    ident+=nom->basicName;
    ident+=" ";
    /*int k=0;
    string ident_tmp;
    while(1)
        {
        char buff[4];
        sprintf(buff,"%d",k);
        ident_tmp=ident;
        ident_tmp+=buff;
        if(InBoss->pers->db.count(ident_tmp)==0)
            {
            break;
            }
        k++;
        }
    ident=ident_tmp;
    identPre=prenom->basicName;
    identPre+=" ";
    k=0;
    while(1)
    {
        char buff[4];
        sprintf(buff,"%d",k);
        ident_tmp=identPre;
        ident_tmp+=buff;
        if(InBoss->persPre->db.count(ident_tmp)==0)
            {
            break;
            }
        k++;
    }
    identPre=ident_tmp;
    //write(file);
     */
    string s_sexe=(sexe)? "M": "F";
    char c_age[4];
    sprintf(c_age,"%d",age);
    string s_age=c_age;
    char c_taille[4];
    sprintf(c_taille,"%d",taille);
    string s_taille=c_taille;
    char c_poids[4];
    sprintf(c_poids,"%d",poids);
    string s_poids=c_poids;
    string queryId="select prenom where id like \""+prenom->basicName+" "+nom->basicName+"%\";";
    //cout<<"queryId= "<<queryId<<endl;
    int int_id=memory->num_rows(queryId);
    char c_id[4];
    sprintf(c_id,"%d",int_id);
    string s_id=prenom->basicName+" "+nom->basicName+" "+c_id;
    string query="insert into personne(id,sexe,prenom,nom,pseudo,def,age,taille,poids,hairColor,eyeColor,skinColor) values(\""+s_id+"\",\""+s_sexe+"\",\""+prenom->basicName+"\",\""+nom->basicName+"\",\""+pseudo->basicName+"\",\""+def+"\","+s_age+","+s_taille+","+s_poids+",\""+hairColor+"\",\""+eyeColor+"\",\""+skinColor+"\");";
    string query2="select * from personne where sexe=\""+s_sexe+"\" and prenom=\""+prenom->basicName+"\" and nom=\""+nom->basicName+"\" and pseudo=\""+prenom->basicName+"\";";
    //cout<<"query= "<<query<<endl;
    memory->insert(query,query2,dataF);
    /*InBoss->pers->db.insert(pair<string,Personne*>(this->ident,this));
    InBoss->persPre->db.insert(pair<string,Personne*>(identPre,this));*/
}









Personne::~Personne()
{
    //dtor
}
