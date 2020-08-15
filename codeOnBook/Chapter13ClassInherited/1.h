// tabnenno.h --a table-tennis base class
#ifndef TABTENNO_H_
#define TABTENNO_H_

#include <string>
// simple base class
class TableTennisPlayer
{
private :
    std::string firstname;
    std::string lastname;
    bool hasTable;
public :
    TableTennisPlayer(const std::string &fn = "none",
                      const std:string &ln = "none",bool ht = false);
    void Name() const;
    bool HasTable() const {return hasTable;}
    void ResetTable(bool v) {hasTable = v;}
};
