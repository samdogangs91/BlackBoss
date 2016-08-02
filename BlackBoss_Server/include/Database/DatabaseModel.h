/* 
 * File:   DatabaseModel.h
 * Author: mblack
 *
 * Created on 29 octobre 2015, 02:29
 */

#ifndef DATABASEMODEL_H
#define	DATABASEMODEL_H
#include "../IA/comprehension.h"
#include "../Environnement/Modele.h"
#include <map>


class DatabaseModel {
public:
    DatabaseModel();
    std::map<std::string,Modele*> db;
    void insert(Modele* mod);
    Modele* find(std::string name);
    DatabaseModel(const DatabaseModel& orig);
    virtual ~DatabaseModel();
private:

};

#endif	/* DATABASEMODEL_H */

