/* 
 * File:   Variable.cpp
 * Author: mblack
 * 
 * Created on 29 octobre 2015, 02:32
 */

#include "../../include/Environnement/Variable.h"

using namespace std;

string intType="int";
string stringType="string";
string floatType="float";

template <typename T>
Variable<T>::Variable(std::string _name,T inf, T sup, string ftype) {
    name=_name;
    domDef[0]=inf;
    domDef[1]=sup;
    type=ftype;
    isCste=false;
}

template <typename T>
Variable<T>::Variable(std::string _name, T _val)
{
    name=_name;
    val=_val;
    isCste=true;
}

/*template <typename T>
Variable<T>::Variable(std::string _name, std::string _type )
{
    name=_name;
    type=_type;
}*/

template <typename T>
Variable<T>* Variable<T>::operator +(Variable<T>* var1)
{
    if(var1->isCste && isCste)
    {
        if(var1->type.compare(stringType)!=0 && type.compare(stringType)!=0)
        {
        T newVal=var1->val+val;
        char cname[8];
        sprintf(cname,"%f",newVal);
        string sname=cname;
        return new Variable(sname,newVal);
        }
    }
    return NULL;
}

template <typename T>
Variable<T>* Variable<T>::operator -(Variable<T>* var1)
{
    if(var1->isCste && isCste)
    {
        if(var1->type.compare(stringType)!=0 && type.compare(stringType)!=0)
        {
        T newVal=var1->val-val;
        char cname[8];
        sprintf(cname,"%f",newVal);
        string sname=cname;
        return new Variable(sname,newVal);
        }
    }
    return NULL;
}

template <typename T>
Variable<T>* Variable<T>::operator *(Variable<T>* var1)
{
    
    if(var1->isCste && isCste)
    {
        if(var1->type.compare(stringType)!=0 && type.compare(stringType)!=0)
        {
        T newVal=var1->val*val;
        char cname[8];
        sprintf(cname,"%f",newVal);
        string sname=cname;
        return new Variable(sname,newVal);
        }
    }
    return NULL;
}

template <typename T>
Variable<T>* Variable<T>::operator /(Variable<T>* var1)
{
    if(var1->isCste && isCste)
    {
        if(var1->type.compare(stringType)!=0 && type.compare(stringType)!=0)
        {
            if(val!=0)
            {
                T newVal=var1->val/val;
                char cname[8];
                sprintf(cname,"%f",newVal);
                string sname=cname;
                return new Variable(sname,newVal);
            }
        }
    }
    return NULL;
}

template <typename T>
Variable<T>* Variable<T>::operator ^(Variable<T>* var1)
{
    if(var1->isCste && this->isCste)
    {
        if(var1->type.compare(stringType)!=0 && this->type.compare(stringType)!=0)
        {
        T newVal=var1->val^this->val;
        char cname[8];
        sprintf(cname,"%f",newVal);
        string sname=cname;
        return new Variable(sname,newVal);
        }
    }
    return NULL;
}

template <typename T>
Variable<T>* Variable<T>::operator =(Variable<T>* var1)
{
    name=var1->name;
    type=var1->type;
}

template <typename T>
Variable<T>::~Variable() {
}

