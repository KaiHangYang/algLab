#include <iostream>
#include "utility.h"

using namespace std;


int main() {
//    int ** tmp = initArray(6, 2);
//
//    tmp[0][0] = 1;
//    tmp[1][0] = 1;
//    tmp[2][0] = 1;
//    tmp[3][0] = 1;
//    tmp[4][0] = 1;
//    tmp[5][0] = 1;
//
//    tmp[0][1] = 1;
//    tmp[1][1] = 1;
//    tmp[2][1] = 1;
//    tmp[3][1] = 2;
//    tmp[4][1] = 2;
//    tmp[5][1] = 2;
//    NearestPoint np(tmp, 6);
//    freeArray(tmp, 6);

//    pointpair * pp = new pointpair;
//    pp->next = NULL;
//    np.getNearestPoint(pp);
    NearestPoint np("/Users/kaihang/Desktop/Code/Cpp/alglab/in.dat");
    np.printData();
    np.output("/Users/kaihang/Desktop/Code/Cpp/alglab/out.dat");

    return 0;
}