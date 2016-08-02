
#include "../../include/IA/phrase.h"
#include <sstream>
#include <vector>
#include <string.h>
#include "../../include/IA/comprehension.h"

using namespace std;


Phrase::Phrase(std::string s)
{
int max_size=s.size();
//char reste[max_size];
//sprintf(reste,"%s",s.c_str());
int i=0;
char actual[max_size+1];
int c=0;
while(i<max_size)
    {
    if(isSeparateur(s[i]))
        {
            actual[c]='\0';
            exp.push_back(actual); //met le pr�c�dent mot dans la phrase
            c=0; //remett l'indice d'actual � 0
            actual[0]=s[i];
            actual[1]='\0';
        exp.push_back(actual); //met le s�parateur dans la phrase
        }
    else
        {if(i==max_size-1)
            {
                actual[c]=s[i];
                actual[c+1]='\0';
                exp.push_back(actual);//met le dernier mot dans la phrase
            }
        else
            {
            actual[c]=s[i];
            c++;
            }
        }
    i++;
    }
size=exp.size();
}

string Phrase::toString()
{
    string res="";
    int k=0;
    while(k!=size)
    {
    res+=exp[k];
    k++;
    }
    return res;
}

Phrase::~Phrase()
{

}
