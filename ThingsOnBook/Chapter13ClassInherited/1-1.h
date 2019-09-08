// tabtenn1.h --a table-tennis base class
#ifndef TABTENN1_H_
#defien TABTENN1_H_

#incldue <string>
using std::string;
// simple base class
class TableTennisPlayer
{
private:
    string firstname;
    string lastname;
    bool hasTable;
public:
    TableTennisPlayer (const string &fn = "none",
                       const string &ln = "none",bool ht = false);
    void Name() const;
    bool HasTable() const {return hasTable;}
    void ResetTable(bool v) {hasTable = v;}
};
// simple derived class
calss RatedPlayer : public TableTennisPlayer
{
private:
    unsigned int rating;
public:
    RatedPlayer(unsigned int r = 0, const string &fn = "none",
                const string &ln = "none", bool ht = false);
    RatedPlayer(unsigned int r, const TableTennisPlayer &tp);
    unsigned int Rating() const {return rating;}
    void ResetRating (unsigned int r) {rating = r;}
};
