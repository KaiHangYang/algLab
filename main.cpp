#include <iostream>
#include "utility.h"

using namespace std;


int main() {
    int ** tmp = initArray(6, 2);

    tmp[0][0] = 1;
    tmp[1][0] = 2;
    tmp[2][0] = 4;
    tmp[3][0] = 3;
    tmp[4][0] = 4;
    tmp[5][0] = 6;

    tmp[0][1] = 1;
    tmp[1][1] = 6;
    tmp[2][1] = 3;
    tmp[3][1] = 6;
    tmp[4][1] = 4;
    tmp[5][1] = 7;
//
    NearestPoint np(tmp, 6);
    freeArray(tmp, 6);

    pointpair * pp = new pointpair;
    pp->next = NULL;
    np.getNearestPoint(pp);
//    NearestPoint np("/home/fog/Dev/algLab/in.dat");
//    np.printData();
//    np.output("/home/fog/Dev/algLab/out.dat");

    return 0;
}