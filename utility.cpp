//
// Created by KaiHang Yang on 15/11/3.
//

#include "utility.h"
int ** initArray(int m, int n) {
    int ** tmp = new int* [m];

    for (int i=0; i != m; ++i) {
        tmp[i] = new int[n];
    }

    return tmp;
}
void freeArray(int ** arr, int m) {

    for (int i=0; i != m; ++i) {
        delete arr[i];
    }
    delete arr;
}
bool sortFunc_x(const int *a, const int *b) {
    return a[0] < b[0];
}
bool sortFunc_y(const int *a, const int *b) {
    return a[1] < b[1];
}

void NearestPoint::deepCopy(int **in, int **out, int sum, int start) {
    for (int i=start, j=0; i != sum; ++i, ++j) {
        out[j][0] = in[i][0];
        out[j][1] = in[i][1];
    }
}
int ** NearestPoint::getNearestPoint(float * dis) {
    int ** result = initArray(2, 2);

}

void NearestPoint::getSum(bound &bd) {
    bd.sum = 0;
    for (int i = static_cast<int>(floor(bd.left)), pos = bd.dotPos; i < static_cast<int>(ceil(bd.right)); ++i) {
        if (pos < pSum && pos >= 0&& sortedX[pos][0]) {
            ++bd.sum;
        }
    }
}

int NearestPoint::split(const bound &bdin, bound &bdleft, bound &bdright) {
    if (bdin.sum == 2 && bdin.sum == 1) {
        // 因为只剩下两个点的话就不需要分化了
        return 0;
    }
    else if (bdin.sum <= 0) {
        // 分化失败 集合内部没有点
        return -1;
    }
    else {
        // 成功，进行划分

    }
}