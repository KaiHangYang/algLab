#include <iostream>
#include "utility.h"

using namespace std;


int main() {
    int ** tmp = initArray(4, 2);
    tmp[0][0] = 3;
    tmp[1][0] = 5;
    tmp[2][0] = 4;
    tmp[3][0] = 1;

    tmp[0][1] = 6;
    tmp[1][1] = 8;
    tmp[2][1] = 3;
    tmp[3][1] = 4;

    for (int i=0; i != 4; ++i) {
        cout << tmp[i][0] << endl;
        cout << tmp[i][1] << endl;
    }

    NearestPoint np(tmp, 4);
    freeArray(tmp, 4);


    return 0;
}