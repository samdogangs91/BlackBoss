#include "../../include/Database/DatabaseMot.h"
#include "../../include/Database/Database.h"

#include "../../include/Langage/Adjectif.h"
#include "../../include/Langage/Determinant.h"
#include "../../include/Langage/Nom.h"
#include "../../include/Langage/NomPropre.h"
#include "../../include/Langage/Prenom.h"


using namespace std;

extern Database* memory;
extern MYSQL data;

extern std::string Adj;
extern std::string Adv;
extern std::string Conj;
extern std::string Det;
extern std::string Nom;
extern std::string NomP;
extern std::string Pre;
extern std::string Pro;
extern std::string ProP;
extern std::string Verb;

DatabaseMot::DatabaseMot()
{
    
}

void DatabaseMot::insert(Mot* mot)
{
    //cout<<"avant insert"<<endl;
    db.insert(pair<string,Mot*>(mot->ident,mot));
    //cout<<"après insert"<<endl;
}


Mot* DatabaseMot::findMot(std::string mot)
{
    Mot* res;
    map<string,Mot*>::iterator it;
    it=db.find(mot);
    if(it==db.end())
    {
    string query="select basicName,type,fun from mot where basicName=\""+mot+"\";";
        mysql_query(&data,query.c_str());
        MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        result=mysql_store_result(&data);
        if(result!=NULL)
        {
            if(row=mysql_fetch_row(result)) // A changer
            {
                string type=row[1]? row[1]: "";
                string fname=row[0]? row[0]:"";
                string ffun=row[2]? row[2]:"";
                if(type.compare(Verb)!=0)
                {
                    if(type.compare(Nom)==0)
                    {
                       res=new NomC(mot); 
                    }
                    else if(type.compare(Adj)==0)
                    {
                       res=new Adjectif(mot); 
                    }
                    else if(type.compare(Det)==0)
                    {
                       res=new Determinant(mot);  
                    }
                    else if(type.compare(Pre)==0)
                    {
                        res=new Prenom(mot); 
                    }
                    else
                    {
                        res=new Mot(fname,type,ffun);
                    }
                }
            }
            else
            {
               res=new Mot("","","rien"); 
            }

            mysql_free_result(result);
        }
        else
        {
            res=new Mot("","","rien");
        }
    }
    else
    {
        res=it->second;
    }
    return res;
}

DatabaseMot::~DatabaseMot()
{
    //dtor
}
