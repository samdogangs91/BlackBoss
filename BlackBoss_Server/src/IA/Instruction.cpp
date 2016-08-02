/* 
 * File:   Instruction.cpp
 * Author: mblack
 * 
 * Created on 23 février 2016, 18:37
 */

#include "../../include/IA/Instruction.h"

using namespace std;
#define If_ "If"
#define Set_ "Set"
#define For_ "For"
#define While_ "While"
#define NewVar_ "NewVar"
#define Bool_ "Bool"

Instruction::Instruction(string inst) {
    int k=0;
    int numPar=0; //nombre de paranthèses non fermées
    string actual;
    while(k<inst.size())
    {
        if(inst[k]==';' && numPar==0)
        {
            actual+=inst[k];
            cont.push_back(actual);
            actual="";
        }
        else if(inst[k]=='(')
        {
            numPar++;
            actual+=inst[k];
        }
        else if(inst[k]==')')
        {
            numPar--;
            actual+=inst[k];
        }
        else
        {
            actual+=inst[k];
        }
        k++;
    }
    type="Program";
    var="";
    if(cont.size()==1)
    {
        type="";
        if(cont[0].size()>2 && type.compare("")==0)
        {
           if(cont[0][0]=='I'&& cont[0][1]=='f' )
            {
               type=If_;
            }
           else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            var=vname;
                            break;
                        }
                   }                  
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }  
           }
        }
        if(cont[0].size()>3 && type.compare("")==0)
        {
            if(cont[0][0]=='F'&& cont[0][1]=='o' && cont[0][2]=='r')
            {
                type=For_;
            }
            else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }  
           }
        }
        if(cont[0].size()>5 && type.compare("")==0)
        {
            if(cont[0][0]=='W'&& cont[0][1]=='h' && cont[0][2]=='i' && cont[0][3]=='l' && cont[0][2]=='e')
            {
                type=While_;
            }
            else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {                   
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }  
           }
        }
        if(cont[0].size()>6 && type.compare("")==0)
        {
            if(cont[0][0]=='N'&& cont[0][1]=='e' && cont[0][2]=='w' && cont[0][3]=='V' && cont[0][4]=='a' && cont[0][5]=='r')
            {
                type=NewVar_;
            }
            else
           {
               int i=0;
               string vname="";
               while(i<cont[0].size())
               {
                   if(cont[0][i]=='=')
                   {
                       type=Set_;
                       var=vname;
                       break;
                   }
                   else if(cont[0][i]=='<' || cont[0][i]=='>')
                   {
                       type=Bool_;
                       var=vname;
                       break;
                   }
                   else if(i+1<cont.size())
                   {
                        if((cont[0][i]=='+' && cont[0][i+1]=='+') || (cont[0][i]=='-' && cont[0][i+1]=='-'))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='+' && cont[0][i+1]=='=') || (cont[0][i]=='-' && cont[0][i+1]=='=') || (cont[0][i]=='*' && cont[0][i+1]=='=') || (cont[0][i]=='/' && cont[0][i+1]=='='))
                        {
                            type=Set_;
                            var=vname;
                            break;
                        }
                        else if((cont[0][i]=='<' && cont[0][i+1]=='=') || (cont[0][i]=='>' && cont[0][i+1]=='=') || (cont[0][i]=='=' && cont[0][i+1]=='='))
                        {
                            type=Bool_;
                            var=vname;
                            break;
                        }
                   }
                   else if(cont[0][i]=='(')
                   {
                       break;
                   }
                   vname+=cont[0][i];
                   i++;
               }  
           }
        }
    }
}


void For(Instruction* deb, Instruction* stop, Instruction* incr,Instruction* boucle)
{
    if(deb->type.compare(Set_)==0 && stop->type.compare(Bool_)==0 && incr->type.compare(Set_)==0)
    {
        if(deb->var.compare("")!=0 && deb->var.compare(incr->var)==0 && deb->var.compare(stop->var)==0)
        {
            
        }
    }
}



void Instruction::compile()
{
    int k;
    
}


Instruction::Instruction(const Instruction& orig) {
}

Instruction::~Instruction() {
}

