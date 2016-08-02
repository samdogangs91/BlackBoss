/* 
 * File:   Monome.cpp
 * Author: mblack
 * 
 * Created on 4 novembre 2015, 00:50
 */

#include "../../include/IA/Monome.h"

using namespace std;



template<typename T>
Monome<T>::Monome(float _coeff, std::vector<Puissance<T>*> _cont) {
    coeff=_coeff;
    cont=ordre(_cont);
}

template <typename T>
vector<Puissance<T>*> Monome<T>::ordre(std::vector<Puissance<T>*> _cont)
{
    vector<Puissance<T>*> rescont;
    string min;
    int pos_min=0;
    int size=_cont.size();
    while(rescont.size()!=size)
    {
        typedef vector<Puissance<T>*> Cont;
        typename Cont::iterator it;
        it=_cont.begin();
        min=it.base()->vari->name;
        int k=0;
        for(it=_cont.begin();it!=_cont.end();it++)
        {
            string actual=it.base()->vari->name;
            if(actual.compare(min)<0)
            {
                min=actual;
                pos_min=k;
            }
            k++;
        }
        rescont.push_back(_cont[pos_min]);
        it=_cont.begin();
        for(k=0;k<pos_min;k++)
        {
            it++;
        }
        _cont.erase(it); //on efface la variable "la plus petite par ordre alphabétique"
    }
    rescont=_cont;
    return rescont;
}

template<typename T>
Monome<T>* Monome<T>::operator +(Monome<T>* m)
{
    if(cont.size()==m->cont.size())
    {
        int k;
        int count=0;
        int size=cont.size();
        for(k=0;k<size;k++)
        {
            if(cont[k]->vari->name.compare(m->cont[k]->vari->name)==0 && (cont[k]->deg==m->cont[k]->deg))
            {
                count++;
            }
        }
        if(count==size)// si les 2 monomes ont les mêmes variables
        {
            float newCoeff=coeff+m->coeff;
            delete m;
            return new Monome(newCoeff,cont);
        }
        
    }
    return NULL;
}

template<typename T>
Monome<T>* Monome<T>::operator -(Monome<T>* m)
{
    if(cont.size()==m->cont.size())
    {
        int k;
        int count=0;
        int size=cont.size();
        for(k=0;k<size;k++)
        {
            if(cont[k]->vari->name.compare(m->cont[k]->vari->name)==0 && (cont[k]->deg==m->cont[k]->deg))
            {
                count++;
            }
        }
        if(count==size)// si les 2 monomes ont les mêmes variables
        {
            float newCoeff=coeff-m->coeff;
            delete m;
            return new Monome(newCoeff,cont);
        }
        
    }
    return NULL;
}

template<typename T>
Monome<T>* Monome<T>::operator *(Monome<T>* m)
{
    int k;
    int l=0;
    int size=cont.size();
    int size2=m->cont.size();
    std::vector<Monome<T>*> _cont;
    float newCoeff=coeff*m->coeff;
    for(k=0;k<size;k++)
    {
        if(l<size2)
        {
            Puissance<T>*p=cont[k]*m->cont[l];
            if(p!=NULL)
            {
                _cont.push_back(p);
                l++;
            }
            else if(cont[k]->vari->name.compare(m->cont[l]->vari->name)<0)
            {
                _cont.push_back(cont[k]);
            }
            else if(cont[k]->vari->name.compare(m->cont[l]->vari->name)>0)
            {
                _cont.push_back(cont[l]);
                l++;
                k--;
            }
        }
        else
        {
            _cont.push_back(cont[k]);
        }
    }
    int i;
    for(i=l;i<m->cont.size;i++)
    {
       _cont.push_back(cont[i]); 
    }
    delete m;
    return new Monome(newCoeff,_cont);
}

template<typename T>
Monome<T>::Monome(string s) {
    int k=0;
    while((s[k]>='0' && s[k]<='9')||s[k]==',') //
    {
        k++;
        if(k==s.size())
        {
            break;
        }
    }
    int taille=s.size()-k;
    if(taille>0)
    {
        char rest[taille];
        sscanf(s.c_str(),"%f%s",&coeff,rest);
        string s2=rest;
        k=0;
        while(k<s2.size())
        {
            if((s2[k]>='a' && s2[k]<='z')||(s2[k]>='A' && s2[k]<='Z')) //si un nom de variable est détecté
            {
                Variable<T>* var=new Variable<T>(s2[k]);
                Puissance<T>* p= new Puissance<T>(var);
                cont.push_back(p);
            }
            else if(s2[k]=='(')
            {
                if(k<(s2.size()-1))
                {
                    k++;
                    while(s2[k]!=')')
                    {
                        //poursuivre ici
                        k++;
                        if(k==s2.size())
                        {
                            break;
                        }
                    }                    
                }
            }
            k++;
        }
        
    }
    else if(s.size()>0)
    {
        sscanf(s.c_str(),"%d",&coeff);
    }
    
}

template<typename T>
Monome<T>::~Monome() {
    int k;
    for(k=0;k<cont.size();k++)
    {
        Puissance<T>* p=cont[cont.size()-1];
        cont.pop_back();
        delete p;
    }
}

