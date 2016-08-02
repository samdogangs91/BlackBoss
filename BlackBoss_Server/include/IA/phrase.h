/*
 * File:   phrase.h
 * Author: enrop
 *
 * Created on 10 juillet 2015, 09:16
 */

#ifndef PHRASE_H
#define	PHRASE_H


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


class Phrase
{public:
    std::vector<std::string> exp;
    int size;
    Phrase(std::string);
    std::string toString();
    ~Phrase();
};


#endif	/* PHRASE_H */

