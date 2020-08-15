// --using base class and derived class
#include <iostream>
#include "1-1.h"

int main(void)
{
    using std::cout;
    using std::endl;
    TableTennisPlayer player1("Tara", "Boomdea", false);
    RatedPlayer rplayer1(1140, "Mollory", "Duck", true);
    rplayer1.Name(); // derived object uses base method
    if (rplayer1.HasTable()) {
        cout << ": has a table.\n";
    }else cout << "hasn't a table.\n";
    player1.Name(); // base object uses base method
    if (player1.HasTable()) {
        cout << ": has a table.\n" ;
    }else cout << ": hasn't a table.\n" ;
    cout << "Name: " ;
    rplayer1.Name();
    cout << ", Rating :" << rplayer1.Rating() << endl ;
    // initialize RatedPlayer using TableTennisplayer Object
    RatedPlayer rplayer2(1212,player1) ;
    cout << "Name:";
    rpalyer2.Name();
    cout << ", Rating: " << rplayer2.Rating() << endl;

    return 0 ;
}
