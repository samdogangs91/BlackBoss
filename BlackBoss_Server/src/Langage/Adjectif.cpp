#include "../../include/Langage/Adjectif.h"
#include "../../include/Langage/Mot.h"
#include "../../include/Database/Database.h"
#include "../../include/Database/DatabaseFun.h"
#include "../../include/Database/DatabaseMot.h"
#include "../../include/IA/phrase.h"
//#include "../../include/IA/BlackIa.h"


using namespace std;

//extern InBlackIa* InBoss;

extern std::string Adj;
extern Database* memory;
extern DatabaseFun* Dfun;
extern DatabaseMot* Dmots;

Adjectif::Adjectif( string fname)
{
    basicName=fname;
  ident=fname;
    catMot=Adj;
    isDenm=false;
    isMasc=true;
    isFem=true;
    string query="select sing,fem,plur,plurFem,isMasc,isFem,isSing,isPlur,canMasc,canFem,canSing,isDenm,fun from adjectif natural join mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
    MYSQL_RES* result=memory->request(query);
    MYSQL_ROW row;
    if(result!=NULL)
    {
        if(row=mysql_fetch_row(result))
        {
            sing=row[0]? row[0]:"";
            fem=row[1]? row[1]:"";
            plur=row[2]? row[2]:"";
            plurFem=row[3]? row[3]:"";
            int nMasc;
            sscanf(row[4],"%d",&nMasc);
            sscanf(row[4],"%*[^\n]");
            isMasc=(nMasc==1);
            int nFem;
            sscanf(row[5],"%d",&nFem);
            sscanf(row[5],"%*[^\n]");
            isFem=(nFem==1);
            int nSing;
            sscanf(row[6],"%d",&nSing);
            sscanf(row[6],"%*[^\n]");
            isSing=(nSing==1);
            int nPlur;
            sscanf(row[7],"%d",&nPlur);
            sscanf(row[7],"%*[^\n]");
            isPlur=(nPlur==1);
            int ncanMasc;
            sscanf(row[8],"%d",&ncanMasc);
            sscanf(row[8],"%*[^\n]");
            canMasc=(ncanMasc==1);
            int ncanFem;
            sscanf(row[9],"%d",&ncanFem);
            sscanf(row[9],"%*[^\n]");
            canFem=(ncanFem==1);
            int ncanSing;
            sscanf(row[10],"%d",&ncanSing);
            sscanf(row[10],"%*[^\n]");
            canSing=(ncanSing==1);
            int nDenm;
            sscanf(row[11],"%d",&nDenm);
            sscanf(row[11],"%*[^\n]");
            isDenm=(nDenm==1);
            string ffun=row[12]? row[12]:"";
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
    Dmots->insert(this);
}


Adjectif::Adjectif(Mot*m)
{
    def=m->def; //d�finition du mot
    isMasc=m->isMasc;//masculin
    isFem=m->isFem;
    isSing=m->isSing;
    isDenm=m->isDenm;//d�nombrable ou pas
    sing=m->sing;
    fem=m->fem;
    plur=m->plur;
    plurFem=m->plurFem;
    catMot=m->catMot; //cat�gorie du mot : adjectif, pr�nom, etc...
    //foncs=m->foncs; //m�thodes propres au mot
}


Adjectif::Adjectif(std::string fname1, std::string fname2, std::string fname3, std::string fname4, bool isMascu, bool isFemi, bool fIsSing, bool fIsPlur, fstream* dataF, bool fIsDenm, bool fCanSing, bool fCanMasc, bool fCanFem, string fun )
{
   basicName=fname1;
  ident=fname1;
  catMot=Adj;
  isMasc=isMascu;
  isFem=isFemi;
  isSing=fIsSing;
  isPlur=fIsPlur;
  canMasc=fCanMasc;
  canFem=fCanFem;
  canSing=fCanSing;
  isDenm=fIsDenm;
  sing="";
  fem="";
  plur="";
  plurFem="";
  int motId;
if(isSing)
    {
        if(canMasc)
        {
            if(isMasc) //si le mot est singulier masculin
                {
                sing=fname1;
                }
            if(isFem) //si le mot est singulier feminin
            {
               sing=fname2; 
            }
        }
        if(canFem)
        {
            if(isMasc)
            {
            fem=fname2;
            }
            if(isFem)
            {
                fem=fname1;
            }
        }
        if(isDenm)
        {
            if(canMasc)
            {
                plur=fname3;
            }
            if(canFem)
            {
                plurFem=fname4;
            }
        }
    }
  if(isPlur)
    {
        if(canMasc)
        {
            if(isMasc) //si le mot est pluriel masculin
                {
                plur=fname1;
                }
            if(isFem) //si le mot est pluriel feminin 
            {
               plurFem=fname2; 
            }
        }
        if(canFem)
        {
            if(isMasc)
            {
                plurFem=fname2;
            }
            if(isFem)
            {
                plur=fname1;
            }
        }
        if(canSing)
        {
            if(canMasc)
            {
                sing=fname3;
            }
            if(canFem)
            {
                fem=fname4;
            }
        }
    }
  
  //foncs.push_back(fun);
  map<string,Mot*>::iterator it;
  it=Dmots->db.find(fname1);
  if(it==Dmots->db.end())
  {
      //BlackBoss->mots->insert(this);
      //cout<<"mot :"<<basicName<<" inséré"<<endl;
      string query="insert into mot(basicName,type,fun) values(\""+basicName+"\",\""+catMot+"\",\""+fun+"\");";
        string query2="select basicName,type from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
        memory->insert(query,query2,dataF);
        string query4="select mot_id from mot where basicName=\""+basicName+"\" and type=\""+catMot+"\";";
        int mot_id=memory->getIntAt(query4);
        char charMotId[4];
        sprintf(charMotId,"%d",mot_id);
        string s_id=charMotId;
        string s_isMasc=(isMasc)? ("true"):("false");
        string s_isFem=(isFem)? ("true"):("false");
        string s_isSing=(isSing)? ("true"):("false");
        string s_isPlur=(isPlur)? ("true"):("false");
        string s_canMasc=(canMasc)? ("true"):("false");
        string s_canFem=(canFem)? ("true"):("false");
        string s_canSing=(canSing)? ("true"):("false");
        string s_isDenm=(isDenm)? ("true"):("false");
        string query3="insert into adjectif(adj_id, basicName,sing,fem,plur,plurFem,isMasc,isFem,isSing,isPlur,canMasc,canFem,canSing,isDenm) values("+s_id+",\""+basicName+"\",\""+sing+"\",\""+fem+"\",\""+plur+"\",\""+plurFem+"\","+s_isMasc+","+s_isFem+","+s_isSing+","+s_isPlur+","+s_canMasc+","+s_canFem+","+s_canSing+","+s_isDenm+");";
        memory->insert(query3,dataF);
  }
  
  if(isSing)
  {
    if(isMasc)
    {
        if(canFem)
        {
          string query="select mot_id from mot where basicName=\""+fname2+"\" and type=\""+catMot+"\";"; 
          motId=memory->getIntAt(query);
          if(motId<0) //on créé son nom féminin singulier s'il n'existe pas
          {
              new Adjectif(fname2,fname1,fname3,fname4,isFemi,true,true,fIsPlur,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
          }
        }
        if(isDenm)
        {
            string query="select mot_id from mot where basicName=\""+fname3+"\" and type=\""+catMot+"\";"; 
            motId=memory->getIntAt(query);
            if(motId<0) //on créé son pluriel
            {
                new Adjectif(fname3,fname4,fname1,fname2,true,isFemi,fIsPlur,true,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
            }
            if(canFem)
            {
               string query="select mot_id from mot where basicName=\""+fname4+"\" and type=\""+catMot+"\";"; 
                motId=memory->getIntAt(query);
                if(motId<0)
                {
                    new Adjectif(fname4,fname3,fname1,fname2,isFemi,true,fIsPlur,true,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
                } 
            }

        }
    }
    //cas où le mot est féminin singulier
    if(isFem)
    {
        if(canMasc)
        {
          string query="select mot_id from mot where basicName=\""+fname2+"\" and type=\""+catMot+"\";"; 
          motId=memory->getIntAt(query);
          if(motId<0) //on créé son nom masculin singulier s'il n'existe pas
          {
              new Adjectif(fname2,fname1,fname3,fname4,true,isMasc,true,fIsSing,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
          }
        }
        if(isDenm)
        {
            string query="select mot_id from mot where basicName=\""+fname4+"\" and type=\""+catMot+"\";"; 
            motId=memory->getIntAt(query);
            if(motId<0)
            {
                new Adjectif(fname4,fname3,fname1,fname2,isFem,true,fIsPlur,true,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
            }
            if(canMasc)
            {
               string query="select mot_id from mot where basicName=\""+fname3+"\" and type=\""+catMot+"\";"; 
                motId=memory->getIntAt(query);
                if(motId<0)
                {
                    new Adjectif(fname3,fname4,fname1,fname2,true,isMascu,fIsPlur,true,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
                } 
            }

        }
    }
  }

if(isPlur)
  {
    if(isMasc)
    {
        if(canFem)
        {
          string query="select mot_id from mot where basicName=\""+fname2+"\" and type=\""+catMot+"\";"; 
          motId=memory->getIntAt(query);
          if(motId<0) //on créé son nom féminin pluriel s'il n'existe pas
          {
              new Adjectif(fname2,fname1,fname3,fname4,isFemi,true,fIsSing,true,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
          }
        }
        if(canSing)
        {
            string query="select mot_id from mot where basicName=\""+fname3+"\" and type=\""+catMot+"\";"; 
            motId=memory->getIntAt(query);
            if(motId<0) //on créé son singulier masculin
            {
                new Adjectif(fname3,fname4,fname1,fname2,true,isFemi,true,fIsSing,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
            }
            if(canFem)
            {
               string query="select mot_id from mot where basicName=\""+fname4+"\" and type=\""+catMot+"\";"; 
                motId=memory->getIntAt(query);
                if(motId<0)
                {
                    new Adjectif(fname4,fname3,fname1,fname2,isFemi,true,true,fIsSing,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
                } 
            }

        }
    }
    //cas où le mot est féminin pluriel
    if(isFem)
    {
        if(canMasc)
        {
          string query="select mot_id from mot where basicName=\""+fname2+"\" and type=\""+catMot+"\";"; 
          motId=memory->getIntAt(query);
          if(motId<0) //on créé son nom masculin pluriel s'il n'existe pas
          {
              new Adjectif(fname2,fname1,fname3,fname4,true,isMasc,fIsSing,true,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
          }
        }
        if(canSing)
        {
            string query="select mot_id from mot where basicName=\""+fname4+"\" and type=\""+catMot+"\";"; 
            motId=memory->getIntAt(query);
            if(motId<0)
            {
                new Adjectif(fname4,fname3,fname1,fname2,isMasc,true,true,fIsSing,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
            }
            if(canMasc)
            {
               string query="select mot_id from mot where basicName=\""+fname3+"\" and type=\""+catMot+"\";"; 
                motId=memory->getIntAt(query);
                if(motId<0)
                {
                    new Adjectif(fname3,fname4,fname1,fname2,true,isMascu,true,fIsSing,dataF,fIsDenm,fCanSing,fCanMasc,fCanFem,fun);
                } 
            }

        }
    }
  }   
}

Adjectif::~Adjectif()
{
    //dtor
}
