//
// Created by KaiHang Yang on 15/11/3.
//

#include "utility.h"


void freeppArr(pointpair * pp) {
    pointpair * ph = pp->next, * tmp;

    delete pp;
    while (ph != NULL) {
        tmp = ph;
        ph = ph->next;
        delete tmp;
    }
}
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


void NearestPoint::caleBound(bound &bd) {
    bd.sum = 0;

    for (int i=0; i != pSum; ++i) {
        if (sortedX[i][0] >= bd.left && sortedX[i][0] < bd.right) {
            if (bd.sum == 0) {
                bd.dotPos = i;
            }
            ++bd.sum;
        }
        else if (sortedX[i][0] >= bd.right){
            break;
        }
    }
//    for (int i = static_cast<int>(floor(bd.left)), pos = bd.dotPos; i < static_cast<int>(ceil(bd.right)); ++i) {
//        if (pos < pSum && pos >= 0 && sortedX[pos][0]) {
//            ++bd.sum;
//        }
//    }
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
        int pos = bdin.dotPos + bdin.sum/2 - 1;
        float splitLine = (sortedX[pos][0] + sortedX[pos+1][0]) / 2.0;

//        bdleft.dotPos = bdin.dotPos;
        bdleft.left = bdin.left;
        bdleft.right = splitLine;
        caleBound(bdleft);

//        bdright.dotPos = bdin.dotPos+bdin.sum;
        bdright.left = splitLine;
        bdright.right = bdin.right;
        caleBound(bdright);

        return 1;
    }
}
void NearestPoint::getMinMerge(const bound &bdl, const bound &bdr, const pointpair *pp1, const pointpair *pp2, pointpair *result) {
    // 求合并的最小值 核心
    float dis, splitX;
    if (pp1->distance < pp2->distance) {
        dis = pp1->distance;
    }
    else {
        dis = pp2->distance;
    }

    // 获取边界条件
    bound bmergel, bmerger;
    splitX = bdl.right;

    bmergel.left = splitX - dis;
    bmergel.right = splitX;
    bmerger.left = splitX;
    bmerger.right = splitX + dis;

    // 在这里如果边界变的太大的话就会出问题

    if (bmergel.left < bdl.left) {
        bmergel.left = bdl.left;
    }

    if (bmerger.right > bdr.right) {
        bmerger.right = bdr.right;
    }

    caleBound(bmergel);
    caleBound(bmerger);

    // 计算边界内部的最小边界
    vector<int> pyl, pyr;
    // 若是没有最小的话就设置result的长度为INFINITY
    for (int i=0; i != pSum; ++i) {
        for (int j=0, p=bmergel.dotPos; j != bmergel.sum; ++j, ++p) {
            if (sortedY[i][0] == sortedX[p][0] && sortedY[i][1] == sortedX[p][1]) {
                pyl.push_back(i);
            }
        }

        for (int j=0, p=bmerger.dotPos; j != bmerger.sum; ++j, ++p) {
            if (sortedY[i][0] == sortedX[p][0] && sortedY[i][1] == sortedX[p][1]) {
                pyr.push_back(i);
            }
        }
    }

    // 填装完毕,可以开始求值了;
    float minSize = INFINITY;
    int minppy[pyr.size()*pyl.size()][2];
    int currPos = 0;
    minppy[currPos][0] = -1;
    minppy[currPos][1] = -1;

    for (unsigned long i=0; i != pyl.size(); ++i) {
        int * dot1 = sortedY[pyl.at(i)];
        for (unsigned long j=0; j != pyr.size(); ++j) {
            int * dot2 = sortedY[pyr.at(j)];

            if (dot2[1] - dot1[1] > dis) {
                // 如果点在左边点之上 直接跳过后面的
                break;
            }
            else if (abs(dot2[1] - dot1[1]) > dis) {
                // 点不在范围内 跳过当前的点
                continue;
            }
            else {
                // 如果在界限内部的话就开始计算
                float len = pow(dot1[0] - dot2[0], 2) + pow(dot1[1] - dot2[1], 2);
                if (len < minSize) {
                    // 保存最小点
                    currPos = 0;
                    minSize = len;
                    minppy[currPos][0] = pyl.at(i);
                    minppy[currPos][1] = pyr.at(j);
                }
                else if (len == minSize) {
                    ++currPos;
                    minppy[currPos][0] = pyl.at(i);
                    minppy[currPos][1] = pyr.at(j);
                }
            }
        }
    }
    // 获取最小的点之后
    result->distance = minSize;

    if (minSize == INFINITY) {
        int tmp[2] = {-1, -1};
        dotCpy(result->p1, tmp);
        dotCpy(result->p2, tmp);
        result->next = NULL;
    }
    else {
        dotCpy(result->p1, sortedY[minppy[0][0]]);
        dotCpy(result->p2, sortedY[minppy[0][1]]);
        pointpair * tmp = result;
        for (int i=1; i <= currPos; ++i) {
            tmp->next = new pointpair;
            tmp = tmp->next;
            tmp->next = NULL;

            dotCpy(tmp->p1, sortedY[minppy[i][0]]);
            dotCpy(tmp->p2, sortedY[minppy[i][1]]);
            tmp->distance = minSize;
        }
    }

}

void NearestPoint::getMinPart(bound &bd, pointpair *result) {
    if (bd.sum == 2) {
        // 如果是两个点的话就直接求距离
        dotCpy(result->p1, sortedX[bd.dotPos]);
        dotCpy(result->p2, sortedX[bd.dotPos+1]);
        result->distance = pow(result->p1[0]-result->p2[0], 2) + pow(result->p1[1]-result->p2[1], 2);
    }
    else if (bd.sum == 1) {
        dotCpy(result->p1, sortedX[bd.dotPos]);
        result->p2[0] = -1;
        result->p2[1] = -1;
        result->distance = INFINITY;
    }
    else if (bd.sum == 0) {
        // 中间没有节点
        result->p1[0] = -1;
        result->p1[1] = -1;
        result->p2[0] = -1;
        result->p2[1] = -1;
        result->distance = INFINITY;
    }
    else {
        // 否则就先分割然后分别求
        bound bdleft, bdright;
        pointpair *ppleft, *ppright, *ppmerge;
        ppleft = new pointpair;
        ppright = new pointpair;
        ppmerge = new pointpair;

        ppleft->next = NULL;
        ppmerge->next = NULL;
        ppright->next = NULL;

        int flag = split(bd, bdleft, bdright);
        getMinPart(bdleft, ppleft);
        getMinPart(bdright, ppright);

        // 分割完毕 求分割边界的最小值
        // 如果是多个节点的话
        getMinMerge(bdleft, bdright, ppleft, ppright, ppmerge);
        int mflag = getMinPointPair(ppleft, ppmerge, ppright);

        if (mflag == -1) {
            // 左边的值为最小的点
            result->distance = ppleft->distance;
            dotCpy(result->p1, ppleft->p1);
            dotCpy(result->p2, ppleft->p2);
            result->next = ppleft->next;

            delete ppleft;
            // 释放内存有些不太好弄 除非全部变成深copy

//            freeppArr(ppright);
//            freeppArr(ppmerge);
        }
        else if (mflag == 0) {
            // 中间最小值
            result->distance = ppmerge->distance;
            dotCpy(result->p1, ppmerge->p1);
            dotCpy(result->p2, ppmerge->p2);
            result->next = ppmerge->next;

            delete ppmerge;

//            freeppArr(ppright);
//            freeppArr(ppleft);
        }
        else {
            // 右边的最小值
            result->distance = ppright->distance;
            dotCpy(result->p1, ppright->p1);
            dotCpy(result->p2, ppright->p2);
            result->next = ppright->next;

            delete ppright;
//            freeppArr(ppleft);
//            freeppArr(ppmerge);
        }

    }
}

int NearestPoint::getMinPointPair(pointpair *pp1, pointpair *pp2, pointpair *pp3) {
    if (pp1->distance <= pp2->distance && pp1->distance <= pp3->distance) {
        pointpair * hehe = pp1->next;
        pointpair * prev = pp1;

        if (pp1->distance == pp2->distance) {
            while (hehe != NULL) {
                prev = hehe;
                hehe = pp1->next;
            }
            // 将两个点中的所有点全部加进去
            prev->next = pp2;
        }

        if (pp1->distance == pp3->distance) {
            hehe = prev->next;
            while (hehe != NULL) {
                prev = hehe;
                hehe = pp1->next;
            }

            prev->next = pp3;
        }
        return -1;
    }
    else if (pp2->distance <= pp1->distance && pp2->distance <= pp3->distance) {
        pointpair * hehe = pp2->next;
        pointpair * prev = pp2;

        if (pp2->distance == pp1->distance) {
            while (hehe != NULL) {
                prev = hehe;
                hehe = pp2->next;
            }
            // 将两个点中的所有点全部加进去
            prev->next = pp1;
        }

        if (pp2->distance == pp3->distance) {
            hehe = prev->next;
            while (hehe != NULL) {
                prev = hehe;
                hehe = pp2->next;
            }

            prev->next = pp3;
        }
        return 0;
    }
    else if (pp3->distance <= pp1->distance && pp3->distance <= pp2->distance) {
        pointpair * hehe = pp3->next;
        pointpair * prev = pp3;

        if (pp3->distance == pp1->distance) {
            while (hehe != NULL) {
                prev = hehe;
                hehe = pp3->next;
            }
            // 将两个点中的所有点全部加进去
            prev->next = pp1;
        }

        if (pp3->distance == pp2->distance) {
            hehe = prev->next;
            while (hehe != NULL) {
                prev = hehe;
                hehe = pp3->next;
            }

            prev->next = pp2;
        }
        return 1;
    }
    // 返回2 表示出错
    return 2;
}

void NearestPoint::getNearestPoint(pointpair *pp) {

    bound bd;
    bd.sum = pSum;
    bd.left = sortedX[0][0] - 0.5;
    bd.right = sortedX[pSum - 1][0] + 0.5;
    bd.dotPos = 0;

    getMinPart(bd, pp);
}
void NearestPoint::printSortedX() {
    for (int i=0; i != pSum; ++i) {
        cout << sortedX[i][0] << ", " << sortedX[i][1] << endl;
    }
}
void NearestPoint::printSortedY() {
    for (int i=0; i != pSum; ++i) {
        cout << sortedY[i][0] << ", " << sortedY[i][1] << endl;
    }
}
int NearestPoint::loadNext() {
    if (arrNum > currProcess+1) {
        ++currProcess;
    }
    else {
        return -1;
    }
    freeArray(sortedX, pSum);
    freeArray(sortedY, pSum);

    pSum = arrSize[currProcess];
    sortedX = initArray(pSum, 2);
    sortedY = initArray(pSum, 2);

    deepCopy(initialDotArr.at(currProcess), sortedX, pSum);
    deepCopy(initialDotArr.at(currProcess), sortedY, pSum);

    // 先按照x进行排序 使用内置的排序算法
    sort(sortedX, sortedX+pSum, sortFunc_x);
    sort(sortedY, sortedY+pSum, sortFunc_y);

    return 0;
}
void NearestPoint::output(char * path) {
    outputFile.open(path);
    if (outputFile.fail()) {
        cout << "Open output File failed!" << endl;
        exit(-1);
    }
    // 读取完毕之后就开始处理
    for (unsigned long i=currProcess; i < arrNum; ++i) {
        pointpair * pp = new pointpair;
        pp->next = NULL;
        printSortedX();
        getNearestPoint(pp);
        // 写入最小点集合
        pointpair * tmp = pp;
        while (tmp != NULL) {
            outputFile << tmp->p1[0] << ' ' << tmp->p1[1] << ' ' << tmp->p2[0] << ' ' << tmp->p2[1] << '|';
            tmp = tmp->next;
        }
        outputFile << '\n';
        freeppArr(pp);
        loadNext();
    }
    outputFile.close();
}
void NearestPoint::printData() {

    //输出所有获取的数据
    for (int i=currProcess; i < arrNum; ++i) {
        cout << "Size:" << arrSize[i] << "--";
        for (int j=0; j != arrSize[i]; ++j) {
            cout << "<" << initialDotArr.at(i)[j][0] << " " << initialDotArr.at(i)[j][1] << ">" << ' ';
        }
        cout << endl;
    }
}
