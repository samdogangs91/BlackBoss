#include "Database.h"
#include <iostream>
#include <fstream>
using namespace std;

extern MYSQL data;

extern fstream dataFile;

string Adr="localhost";
string dbname="BlackIa2";
string login="MBlack";
string mdp="weyPyjmXvb";

Database::Database()
{
    mysql_init(&data);
    ok=true;
    mysql_options(&data, MYSQL_READ_DEFAULT_GROUP,"option");
    if(mysql_real_connect(&data,Adr.c_str(),login.c_str(),mdp.c_str(),dbname.c_str(),0,NULL,0))
    {
        mysql_query(&data,"set names utf8");
    }
    else
    {
        cout<<"Erreur d'accès à la base de données"<<endl;
        ok=false;
    }
}


MYSQL_RES* Database::request(string query)
{
    MYSQL_RES* result=NULL;
    mysql_query(&data,query.c_str());
    result=mysql_store_result(&data);
    return result;
}


unsigned int Database::num_rows(std::string query)
{
    unsigned int res=0;
    MYSQL_RES* result=request(query);
    if(result!=NULL)
    {
        while(mysql_fetch_row(result))
        {
            res++;
        }
    }
    mysql_free_result(result);
    return res;
}

void Database::insert(std::string query, std::string query2, fstream* dataF)
{
    
    if(ok)
    {
        mysql_query(&data,query2.c_str());//test de présence dans la base de données de l'élément à ajouter
        int num_rows=0;
        MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        result=mysql_use_result(&data);
        if(result!=NULL)
        {
            while((row=mysql_fetch_row(result)))
            {
                num_rows++;
            }
            mysql_free_result(result);
            if(num_rows==0)
            {
              mysql_query(&data,query.c_str());//ajout à la base de données 

              if(mysql_affected_rows(&data)==1)
              {
                  //cout<<"requête '"<<query<<"' exécutée avec succès"<<endl;
                  
                  (*dataF)<<query<<endl;
              }
              else
              {
                  //cout<<"échec de la requête '"<<query<<"'"<<endl;
              }
            }
        }
        else
        {
            //cout<<"requête '"<<query2<<"' déja exécutée dans la base de données"<<endl;
        }
    }
    else
    {
        //cout<<"Fichier dataFile non ouvert"<<endl;
    }
}

void Database::insert(std::string query, std::string query2)
{
    if(ok)
    {
        mysql_query(&data,query2.c_str());//test de présence dans la base de données de l'élément à ajouter
        int num_rows=0;
        MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        result=mysql_use_result(&data);
        if(result!=NULL)
        {
            while((row=mysql_fetch_row(result)))
            {
                num_rows++;
            }
            mysql_free_result(result);
            if(num_rows==0)
            {
              mysql_query(&data,query.c_str());//ajout à la base de données 

              if(mysql_affected_rows(&data)==1)
              {
                  //cout<<"requête '"<<query<<"' exécutée avec succès"<<endl;         
              }
              else
              {
                  //cout<<"échec de la requête '"<<query<<"'"<<endl;
              }
            }
        }
        else
        {
            //cout<<"requête '"<<query2<<"' déja exécutée dans la base de données"<<endl;
        }
    }

}


void Database::insert(std::string query)
{
    if(ok)
    {
       mysql_query(&data,query.c_str());//ajout à la base de données  
    }
    else
    {
        //cout<<"Fichier dataFile non ouvert"<<endl;
    }
    
}

void Database::insert(std::string query, fstream* dataF)
{
    
    if(ok)
    {
       mysql_query(&data,query.c_str());//ajout à la base de données 

          if(mysql_affected_rows(&data)==1)
          {
              //cout<<"requête '"<<query<<"' exécutée avec succès"<<endl;
              (*dataF)<<query<<endl;
          }
          else
          {
              //cout<<"échec de la requête '"<<query<<"'"<<endl;
          } 
    }
    else
    {
        //cout<<"Fichier dataFile non ouvert"<<endl;
    }
}


int Database::getIntAt(string query)
{
    mysql_query(&data,query.c_str());
    MYSQL_RES* result=NULL;
        MYSQL_ROW row;
        int res=-1000;
        result=mysql_use_result(&data);
        if((row=mysql_fetch_row(result)))
        {
            if(row[0]) sscanf(row[0],"%d",&res);
        }
        
        mysql_free_result(result);
        return res;
}



Database::~Database()
{
    //dtor
}
