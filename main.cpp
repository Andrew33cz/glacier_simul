#include <iostream>
#include "functions.h"


using namespace std;

int main()
{
    //              0  1  2 3 4  5 6 7 8 9 10 11 12 13 14
    vector<double> v= {4, 2, 2,2,3, 5,6,7,9,5, 3, 1, 1, 1, 3,6,8,9,8,6,3,2,1,3,5,5};
    /*
     0123456789abcde|
    9        X      |
    8        v      |
    7       Xv      |
    6      X v      |
    5     X   X     |
    4X    v   v     |
    3v   X     X   X|
    2 XXX      v   v|
    1           XXX |
    */

    RunVectorWithVisual(v,3600,10);
    return 0;
}
