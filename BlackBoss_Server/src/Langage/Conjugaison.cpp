/* 
 * File:   Conjugaison.cpp
 * Author: mblack
 * 
 * Created on 3 septembre 2015, 04:16
 */

#include "../../include/Langage/Conjugaison.h"
#include "../../include/Database/Database.h"

using namespace std;
extern string part;
extern string passe;
extern string imp;
extern string condi;
extern string subjo;
extern string pres;
extern string imperat;
extern string fut;

extern string Verb;
extern Database* memory;

Conjugaison::Conjugaison(std::string cje, std::string ctu, std::string cil, std::string cnous, std::string cvous, std::string cils, std::string inf, std::string temps, std::string fun, bool fIsEtat, bool fIsAux, int groupe )
{
    if(temps.compare(imperat)!=0)
    {
        je=new Verbe(cje,inf,temps,1,groupe,fun,fIsEtat,fIsAux);
        tu=new Verbe(ctu,inf,temps,2,groupe,fun,fIsEtat,fIsAux);
        il=new Verbe(cil,inf,temps,3,groupe,fun,fIsEtat,fIsAux);
        nous=new Verbe(cnous,inf,temps,4,groupe,fun,fIsEtat,fIsAux);
        vous=new Verbe(cvous,inf,temps,5,groupe,fun,fIsEtat,fIsAux);
        ils=new Verbe(cils,inf,temps,6,groupe,fun,fIsEtat,fIsAux);
        
    }
    else 
    {
        tu=new Verbe(ctu,inf,temps,2,groupe,fun,fIsEtat,fIsAux);

        nous=new Verbe(cnous,inf,temps,4,groupe,fun,fIsEtat,fIsAux);
        vous=new Verbe(cvous,inf,temps,5,groupe,fun,fIsEtat,fIsAux); 
       
    }
}


Conjugaison::Conjugaison(std::string inf, fstream* dataF, int groupe, std::string temps, std::string fun, bool fIsEtat, bool fIsAux)
{
    int size=inf.size();
    string base;
    int k;
    for(k=0;k<size-2;k++)
    {
     base+=inf[k];   
    }
    //cout<<"base: "<<base<<endl;
    string cje;
    string ctu;
    string cil;
    string cnous;
    string cvous;
    string cils;
    switch(groupe)
    {
            case 2:
                if(temps.compare(passe)==0)
                {
                    cje=base+"is";
                    ctu=base+"is";
                    cil=base+"it";
                    cnous=base+"îmes";
                    cvous=base+"îtes";
                    cils=base+"irent";
                }
                else if(temps.compare(imp)==0)
                {
                    cje=base+"issais";
                    ctu=base+"issais";
                    cil=base+"issait";
                    cnous=base+"issions";
                    cvous=base+"issiez";
                    cils=base+"issaient";
                }
                else if(temps.compare(condi)==0)
                {
                    cje=base+"irais";
                    ctu=base+"irais";
                    cil=base+"irait";
                    cnous=base+"irions";
                    cvous=base+"iriez";
                    cils=base+"iraient";
                }
                else if(temps.compare(subjo)==0)
                {
                    cje=base+"isse";
                    ctu=base+"isses";
                    cil=base+"isse";
                    cnous=base+"issons";
                    cvous=base+"issez";
                    cils=base+"issent";
                }
                else if(temps.compare(pres)==0)
                {
                    cje=base+"is";
                    ctu=base+"is";
                    cil=base+"it";
                    cnous=base+"issons";
                    cvous=base+"issez";
                    cils=base+"issent";
                }
                else if(temps.compare(imperat)==0)
                {
                    ctu=base+"is";
                    cnous=base+"issons";
                    cvous=base+"issez";      
                }
                else if(temps.compare(fut)==0)
                {
                    cje=base+"irai";
                    ctu=base+"iras";
                    cil=base+"ira";
                    cnous=base+"irons";
                    cvous=base+"irez";
                    cils=base+"iront";
                }
                break;
            case 1:
                if(temps.compare(passe)==0)
                {
                    cje=base+"ai";
                    ctu=base+"as";
                    cil=base+"a";
                    cnous=base+"âmes";
                    cvous=base+"âtes";
                    cils=base+"èrent";
                }
                else if(temps.compare(imp)==0)
                {
                    cje=base+"ais";
                    ctu=base+"ais";
                    cil=base+"ait";
                    cnous=base+"ions";
                    cvous=base+"iez";
                    cils=base+"aient";
                }
                else if(temps.compare(condi)==0)
                {
                    cje=base+"erais";
                    ctu=base+"erais";
                    cil=base+"erait";
                    cnous=base+"erions";
                    cvous=base+"eriez";
                    cils=base+"eraient";
                }
                else if(temps.compare(subjo)==0)
                {
                    cje=base+"e";
                    ctu=base+"es";
                    cil=base+"e";
                    cnous=base+"ons";
                    cvous=base+"ez";
                    cils=base+"ent";
                }
                else if(temps.compare(pres)==0)
                {
                    cje=base+"e";
                    ctu=base+"es";
                    cil=base+"e";
                    cnous=base+"ons";
                    cvous=base+"ez";
                    cils=base+"ent";
                }
                else if(temps.compare(imperat)==0)
                {
                    ctu=base+"e";
                    cnous=base+"ons";
                    cvous=base+"ez";      
                }
                else if(temps.compare(fut)==0)
                {
                    cje=base+"erai";
                    ctu=base+"eras";
                    cil=base+"era";
                    cnous=base+"erons";
                    cvous=base+"erez";
                    cils=base+"eront";
                }
                break;
    }

    string query4="select mot_id from mot where basicName=\""+inf+"\" and type=\""+Verb+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    string s_Etat=(fIsEtat)? ("true"):("false");
    string s_Aux=(fIsEtat)? ("true"):("false");
    string query5="select vb_id from conjugaison where inf=\""+inf+"\" and temps=\""+temps+"\";";
    if(temps.compare(imperat)!=0)
    {
        string query3="insert into conjugaison(vb_id,inf,je,tu,il,nous,vous,ils,temps,isEtat,isAux) values(\""+s_id+"\",\""+inf+"\",\""+cje+"\",\""+ctu+"\",\""+cil+"\",\""+cnous+"\",\""+cvous+"\",\""+cils+"\",\""+temps+"\","+s_Etat+","+s_Aux+");";
        memory->insert(query3,query5,dataF);
        je=new Verbe(cje,inf,temps,1,dataF,groupe,fun,fIsEtat,fIsAux);
        tu=new Verbe(ctu,inf,temps,2,dataF,groupe,fun,fIsEtat,fIsAux);
        il=new Verbe(cil,inf,temps,3,dataF,groupe,fun,fIsEtat,fIsAux);
        nous=new Verbe(cnous,inf,temps,4,dataF,groupe,fun,fIsEtat,fIsAux);
        vous=new Verbe(cvous,inf,temps,5,dataF,groupe,fun,fIsEtat,fIsAux);
        ils=new Verbe(cils,inf,temps,6,dataF,groupe,fun,fIsEtat,fIsAux);
    }
    else
    {
        string query3="insert into conjugaison(vb_id,inf,tu,nous,vous,temps,isEtat,isAux) values(\""+s_id+"\",\""+inf+"\",\""+ctu+"\",\""+cnous+"\",\""+cvous+"\",\""+temps+"\","+s_Etat+","+s_Aux+");";
        memory->insert(query3,query5,dataF);
        tu=new Verbe(ctu,inf,temps,2,dataF,groupe,fun,fIsEtat,fIsAux);
        nous=new Verbe(cnous,inf,temps,4,dataF,groupe,fun,fIsEtat,fIsAux);
        vous=new Verbe(cvous,inf,temps,5,dataF,groupe,fun,fIsEtat,fIsAux);
    }
}

Conjugaison::Conjugaison(std::string cje, std::string ctu, std::string cil, std::string cnous, std::string cvous, std::string cils, std::string inf, std::string temps, fstream* dataF, std::string fun, bool fIsEtat, bool fIsAux, int groupe)
{
    string query4="select mot_id from mot where basicName=\""+inf+"\" and type=\""+Verb+"\";";
    int mot_id=memory->getIntAt(query4);
    char charMotId[4];
    sprintf(charMotId,"%d",mot_id);
    string s_id=charMotId;
    string s_Etat=(fIsEtat)? ("true"):("false");
    string s_Aux=(fIsEtat)? ("true"):("false");
    string query5="select vb_id from conjugaison where inf=\""+inf+"\" and temps=\""+temps+"\";";
    if(temps.compare(imperat)!=0)
    {
        je=new Verbe(cje,inf,temps,1,dataF,groupe,fun,fIsEtat,fIsAux);
        tu=new Verbe(ctu,inf,temps,2,dataF,groupe,fun,fIsEtat,fIsAux);
        il=new Verbe(cil,inf,temps,3,dataF,groupe,fun,fIsEtat,fIsAux);
        nous=new Verbe(cnous,inf,temps,4,dataF,groupe,fun,fIsEtat,fIsAux);
        vous=new Verbe(cvous,inf,temps,5,dataF,groupe,fun,fIsEtat,fIsAux);
        ils=new Verbe(cils,inf,temps,6,dataF,groupe,fun,fIsEtat,fIsAux);
        string query3="insert into conjugaison(vb_id,inf,je,tu,il,nous,vous,ils,temps,isEtat,isAux) values(\""+s_id+"\",\""+inf+"\",\""+cje+"\",\""+ctu+"\",\""+cil+"\",\""+cnous+"\",\""+cvous+"\",\""+cils+"\",\""+temps+"\","+s_Etat+","+s_Aux+");";
        memory->insert(query3,query5,dataF);
    }
    else 
    {
        tu=new Verbe(ctu,inf,temps,2,dataF,groupe,fun,fIsEtat,fIsAux);

        nous=new Verbe(cnous,inf,temps,4,dataF,groupe,fun,fIsEtat,fIsAux);
        vous=new Verbe(cvous,inf,temps,5,dataF,groupe,fun,fIsEtat,fIsAux); 
        string query3="insert into conjugaison(vb_id,inf,tu,nous,vous,temps,isEtat,isAux) values(\""+s_id+"\",\""+inf+"\",\""+ctu+"\",\""+cnous+"\",\""+cvous+"\",\""+temps+"\","+s_Etat+","+s_Aux+");";
        memory->insert(query3,query5,dataF);
    }
    
    
}

Conjugaison::Conjugaison(const Conjugaison& orig) {
}

Conjugaison::~Conjugaison() {
}

