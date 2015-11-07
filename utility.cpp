//
// Created by KaiHang Yang on 15/11/3.
//

#include "utility.h"


/**
 * 最近点对部分
 **/

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


void NearestPoint::caleBound(bound &bd, int i) {
    bd.sum = 0;

    for (int i=0; i != pSum; ++i) {
        if (i == 0 && sortedX[i][0] >= bd.left && sortedX[i][0] < bd.right) {
            if (bd.sum == 0) {
                bd.dotPos = i;
            }
            ++bd.sum;
        }
        else if (i == 1 && sortedX[i][0] >= bd.left && sortedX[i][0] <= bd.right) {

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
//        caleBound(bdleft);
        bdleft.dotPos = bdin.dotPos;
        bdleft.sum = pos - bdin.dotPos+1;

//        bdright.dotPos = bdin.dotPos+bdin.sum;
        bdright.left = splitLine;
        bdright.right = bdin.right;
        bdright.dotPos = pos + 1;
        bdright.sum = bdin.sum - bdleft.sum;
//        caleBound(bdright);

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
    // caleBound函数根本不能用...

    if (bmergel.left < bdl.left) {
        bmergel.left = bdl.left;
        bmergel.dotPos = bdl.dotPos;
        bmergel.sum = bdl.sum;
    }
    else {
        // 如果边界小的话
        bmergel.sum = 0;
        bmergel.dotPos = -1;
        for (int i=bdl.dotPos; i != (bdl.sum+bdl.dotPos); ++i) {
            if (sortedX[i][0] >= bmergel.left && sortedX[i][0] <= bmergel.right) {
                if (bmergel.sum == 0) {
                    bmergel.dotPos = i;
                }
                ++bmergel.sum;
            }
        }
    }

    if (bmerger.right > bdr.right) {
        bmerger.right = bdr.right;
        bmerger.dotPos = bdr.dotPos;
        bmerger.sum = bdr.sum;
    }
    else {
        //如果边界小得话
        bmerger.sum = 0;
        for (int i=bdr.dotPos; i != (bdr.sum+bdr.dotPos); ++i) {
            if (sortedX[i][0] >= bmerger.left && sortedX[i][0] <= bmerger.right) {
                if (bmerger.sum == 0) {
                    bmerger.dotPos = i;
                }
                ++bmerger.sum;
            }
        }

    }

    // 计算边界内部的最小边界
    vector<int> pyl, pyr;
    int pylflag[bmergel.sum];
    int pyrflag[bmerger.sum];
    memset(pylflag, 0, bmergel.sum* sizeof(int));
    memset(pyrflag, 0, bmerger.sum* sizeof(int));
    // 若是没有最小的话就设置result的长度为INFINITY
    for (int i=0; i != pSum; ++i) {
        for (int j=0, p=bmergel.dotPos; j != bmergel.sum; ++j, ++p) {
            if (pylflag[j] == 0 && sortedY[i][0] == sortedX[p][0] && sortedY[i][1] == sortedX[p][1]) {
                pylflag[j] = 1;
                pyl.push_back(i);
                break;
            }
        }

        for (int j=0, p=bmerger.dotPos; j != bmerger.sum; ++j, ++p) {
            if (pyrflag[j] == 0 && sortedY[i][0] == sortedX[p][0] && sortedY[i][1] == sortedX[p][1]) {
                pyrflag[j] = 1;
                pyr.push_back(i);
                break;
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
                hehe = prev->next;
            }
            // 将两个点中的所有点全部加进去
            prev->next = pp2;
        }

        if (pp1->distance == pp3->distance) {
            hehe = prev->next;
            while (hehe != NULL) {
                prev = hehe;
                hehe = prev->next;
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
                hehe = prev->next;
            }
            // 将两个点中的所有点全部加进去
            prev->next = pp1;
        }

        if (pp2->distance == pp3->distance) {
            hehe = prev->next;
            while (hehe != NULL) {
                prev = hehe;
                hehe = prev->next;
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
                hehe = prev->next;
            }
            // 将两个点中的所有点全部加进去
            prev->next = pp1;
        }

        if (pp3->distance == pp2->distance) {
            hehe = prev->next;
            while (hehe != NULL) {
                prev = hehe;
                hehe = prev->next;
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

/*
 * 大数相乘部分
 */
void BigNum::base_add (string &a, string &b, string &out) {
    int n;
    out.empty();
    if (a.size() > b.size()) {
        n = a.size();
        b.insert(0, n-b.size(), '0');
    }
    else if (a.size() < b.size()) {
        n = b.size();
        a.insert(0, n-a.size(), '0');
    }
    else {
        n = a.size();
    }
    int c = 0;
    int sum = 0;

    for (int i=n-1; i >= 0; --i) {

        sum = a.at(i) + b.at(i) - 2*'0' + c;
        c = sum / 10;
        sum %= 10;
        out.insert(0, 1, sum+'0');
    }

    if (c != 0) {
        out.insert(0, 1, c+'0');
    }

    int zero_pos = out.find_first_not_of('0');
    if (string(out.size(), '0') == out) {
        // 全是0的情况
        zero_pos = out.size() - 1;
        out.erase(1, zero_pos);
    }
    else {
        out.erase(0, zero_pos);
    }
}

int BigNum::base_minus (string &a, string &b, string &out) {
    int n;
    int flag = 0;

    out.empty();

    if (a.size() > b.size()) {
        n = a.size();
        b.insert(0, n-b.size(), '0');
    }
    else if (a.size() < b.size()) {
        n = b.size();
        a.insert(0, n-a.size(), '0');
    }
    else {
        n = a.size();
    }

    if (a < b) {
        // 1表示结果为负数
        flag = 1;
    }

    int c = 0;
    int sum = 0;
    for (int i=n-1; i >= 0; --i) {
        if (flag == 1) {
            sum = b.at(i) - a.at(i) - c;
        }
        else {
            sum = a.at(i) - b.at(i) - c;
        }

        if (sum < 0) {
            // 顶多借一位
            c = -sum/10 + 1;
            sum += (10*c);
        }
        else {
            c = 0;
        }
        out.insert(0, 1, sum+'0');
    }
    // 对零进行特殊处理
    int zero_pos = out.find_first_not_of('0');
    if (string(out.size(), '0') == out) {
        // 全是0的情况
        zero_pos = out.size() - 1;
        out.erase(1, zero_pos);
    }
    else {
        out.erase(0, zero_pos);
    }

    return flag;
}
// 对运算进行重载
BigNum BigNum::operator+(BigNum &a) {
    //对每种情况的运算进行处理
    string tmp;
    int tmp_flag;

    if (flag == a.flag) {
        base_add(data, a.data, tmp);
        tmp_flag = flag;
    }
    else if (flag == 0) {
        // 前者为正数 后者为负数
        tmp_flag = base_minus(data, a.data, tmp);
    }
    else {
        // 后者为正数
        tmp_flag = base_minus(a.data, data, tmp);
    }
    // 计算完毕之后
    return BigNum(tmp, tmp_flag);
}
BigNum BigNum::operator-(BigNum &a) {
    string tmp;
    int tmp_flag;

    if (flag == a.flag) {
        tmp_flag = base_minus(data, a.data, tmp);

        if (tmp_flag == flag) {
            tmp_flag = 0;
        }
        else {
            tmp_flag = 1;
        }
    }
    else if (flag == 0) {
        // 如果前者为正数
        base_add(data, a.data, tmp);
        tmp_flag = 0;
    }
    else {
        // 如果前者为负数的话
        tmp_flag = 1;
        base_add(data, a.data, tmp);
    }

    return BigNum(tmp, tmp_flag);
}
BigNum BigNum::operator*(BigNum &a) {
    // 乘法运算符重载
    
}
BigNum BigNum::operator-() {
    // 单目减号运算符
    int tmp = (flag == 0)?1:0;
    return BigNum(data, tmp);
}
BigNum BigNum::operator+() {
    // 单目正号运算符
    return BigNum(data, flag);
}
BigNum::operator const char *() {
    BigNum tmp(data, flag);
    if (flag == 1) {
        tmp.data.insert(0, 1, '-');
    }
    return tmp.data.c_str();
}