#include <iostream>
#include "utility.h"

using namespace std;


int main() {
    int ** tmp = initArray(6, 2);

    /*NearestPoint np(tmp, 6);
    freeArray(tmp, 6);

    pointpair pp;
    np.getNearestPoint(pp);*/
    NearestPoint np("/home/fog/Dev/algLab/in.dat");

    return 0;
}