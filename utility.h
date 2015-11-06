//
// Created by KaiHang Yang on 15/11/3.
//

#ifndef ALGLAB_UTILITY_H
#define ALGLAB_UTILITY_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

/* 最近点对问题辅助 */
typedef struct ppair{
    int p1[2];
    int p2[2];
    float distance;
    struct ppair * next; // 用来储存多个相同距离的点对
} pointpair;
typedef struct {
    int dotPos; // 记录边界内部点集和起始位置
    int sum; // 记录边界内部的点集的数目
    float left; // 记录边界的左边界
    float right; // 记录右边界
} bound;

int ** initArray(int m, int n);
void freeArray(int ** arr, int m);
bool sortFunc_x(const int *a, const int *b);
bool sortFunc_y(const int *a, const int *b);

// 距离没有开平方，最后再求
class NearestPoint {
private:
    // 总共的点数
    int pSum;
    // 点集按照X 或者 Y坐标排序后的
    int ** sortedX;
    int ** sortedY;

    vector<int **> initialDotArr;
    int arrNum;
    int *arrSize;
    unsigned long currProcess;

    ifstream inputFile;
    ofstream outputFile;

    void deepCopy(int ** in, int ** out, int sum, int start=0);

    // 取 >= start 和 < end的点数目
    int loadNext(); // 成功返回0 不成功返回-1
    void caleBound(bound &bd);
    // 返回值是是否分化成功等
    int split(const bound &bdin, bound &bdleft, bound &bdright);
    // 获取区间内部最小距离
    void getMinPart(bound &bd, pointpair *result);
    // 获取合并过程中的最小点
    void getMinMerge(const bound &bdl, const bound &bdr, const pointpair *pp1, const pointpair *pp2 ,pointpair *result);
    // 获取三个点集中最小的点
    int getMinPointPair(pointpair *pp1, pointpair *pp2, pointpair *pp3);

public:
    // 初始化函数
    NearestPoint(int ** points, int sum) {
        arrNum = 1;
        currProcess = 0;
        pSum = sum;

        sortedX = initArray(sum, 2);
        sortedY = initArray(sum, 2);

        deepCopy(points, sortedX, sum);
        deepCopy(points, sortedY, sum);

        // 先按照x进行排序 使用内置的排序算法
        sort(sortedX, sortedX+sum, sortFunc_x);
        sort(sortedY, sortedY+sum, sortFunc_y);
    };
    // 从文件中初始化数据
    NearestPoint(char * path) {
        string s;
        inputFile.open(path);

        if (inputFile.fail()) {
            cout << "Open file failed!" << endl;
            exit(-1);
        }
        getline(inputFile, s);
        arrNum = atoi(s.c_str()); // 如果读取失败结果是0 所以循环没问题

        if (arrNum == 0) {
            cout << "File format is wrong!" << endl;
            exit(-1);
        }

        arrSize = new int[arrNum];

        for (int i=0; i != arrNum; ++i) {
            // 获取该组的点数
            getline(inputFile, s);
            arrSize[i] = atoi(s.c_str());
            // 对这么多点进行操作
            int ** tmpDots = initArray(arrSize[i], 2);
            for (int j=0; j != arrSize[i]; ++j) {
                getline(inputFile, s);
                sscanf(s.c_str(), "%d%*[^0-9]%d", &tmpDots[j][0], &tmpDots[j][1]);
            }
            initialDotArr.push_back(tmpDots);
        }
        currProcess = 0;
        int sum = arrSize[currProcess];

        pSum = sum;

        sortedX = initArray(sum, 2);
        sortedY = initArray(sum, 2);

        deepCopy(initialDotArr.at(currProcess), sortedX, sum);
        deepCopy(initialDotArr.at(currProcess), sortedY, sum);

        // 先按照x进行排序 使用内置的排序算法
        sort(sortedX, sortedX+sum, sortFunc_x);
        sort(sortedY, sortedY+sum, sortFunc_y);
        inputFile.close();
    }
    // 析构
/*    ~NearestPoint() {
        freeArray(sortedX, pSum);
        freeArray(sortedY, pSum);
    }*/
    // 获取最近点
    void getNearestPoint(pointpair *pp);
    void output(char * path);
    void printSortedX();
    void printSortedY();
};

#endif //ALGLAB_UTILITY_H