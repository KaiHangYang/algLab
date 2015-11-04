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
void boundCpy(bound &out, const bound &in) {
    out.left = in.left;
    out.sum = in.sum;
    out.right = in.right;
    out.dotPos = in.dotPos;
}
void dotCpy(int * p1, const int * p2) {
    p1[0] = p2[0];
    p1[1] = p2[1];
}

void NearestPoint::deepCopy(int **in, int **out, int sum, int start) {
    for (int i=start, j=0; i != sum; ++i, ++j) {
        out[j][0] = in[i][0];
        out[j][1] = in[i][1];
    }
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
    if (bdin.sum == 2) {
        // 因为只剩下两个点的话就不需要分化了
        return 0;
    }
    else if (bdin.sum <= 0) {
        // 分化失败 集合内部没有点
        return -1;
    }
    else {
        // 成功，进行划分
        int pos = bdin.dotPos + bdin.sum/2;
        float splitLine = (sortedX[pos][0] + sortedX[pos+1][0]) / 2.0;

        bdleft.dotPos = bdin.dotPos;
        bdleft.left = bdin.left;
        bdleft.right = splitLine;
        getSum(bdleft);

        bdright.dotPos = bdin.dotPos+bdin.sum;
        bdright.left = splitLine;
        bdright.right = bdin.right;
        bdright.sum = bdin.sum - bdleft.sum;
        return 1;
    }
}
void NearestPoint::getMinMerge(bound &bd, const pointpair &pp1, const pointpair &pp2, pointpair &result) {
    // 求合并的最小值
    float dis;
    if (pp1.distance < pp2.distance) {
        dis = pp1.distance;
    }
    else {
        dis = pp2.distance;
    }

}

void NearestPoint::getMinPart(bound &bd, pointpair &result) {
    if (bd.sum == 2) {
        // 如果是两个点的话就直接求距离
        dotCpy(result.p1, sortedX[bd.dotPos]);
        dotCpy(result.p2, sortedX[bd.dotPos+1]);
        result.distance = pow(result.p1[0]-result.p2[0], 2) + pow(result.p1[1]-result.p2[1], 2);
    }
    else if (bd.sum == 1) {
        dotCpy(result.p1, sortedX[bd.dotPos]);
        result.p2[0] = -1;
        result.p2[1] = -1;
        result.distance = INFINITY;
    }
    else if (bd.sum == 0) {
        // 中间没有节点
        result.p1[0] = -1;
        result.p1[1] = -1;
        result.p2[0] = -1;
        result.p2[1] = -1;
        result.distance = INFINITY;
    }
    else {
        // 否则就先分割然后分别求
        bound bdleft, bdright;
        pointpair ppleft, ppright;

        int flag = split(bd, bdleft, bdright);
        getMinPart(bdleft, ppleft);
        getMinPart(bdright, ppright);

        // 分割完毕 求分割边界的最小值


    }
}

pointpair & NearestPoint::getMinPointPair(const pointpair &pp1, const pointpair &pp2, const pointpair &pp3) {

}

int NearestPoint::getNearestPoint(const bound & bd) {

}