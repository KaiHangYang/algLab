//
// Created by KaiHang Yang on 15/11/3.
//

#ifndef ALGLAB_UTILITY_H
#define ALGLAB_UTILITY_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/* 最近点对问题辅助 */
typedef struct {
    int p1[2];
    int p2[2];
    float distance;
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

class NearestPoint {
private:
    // 总共的点数
    int pSum;
    // 点集按照X 或者 Y坐标排序后的
    int ** sortedX;
    int ** sortedY;

    void deepCopy(int ** in, int ** out, int sum, int start=0);

    // 取 >= start 和 < end的点数目
    void getSum(bound &bd);
    // 返回值是是否分化成功等
    int split(const bound &bdin, bound &bdleft, bound &bdright);
    // 获取区间内部最小距离
    pointpair getMinPart(bound &bd);
    // 获取合并过程中的最小点
    pointpair getMinMerge(bound &bd);
    // 获取三个点集中最小的点
    pointpair &getMinPointPair(const pointpair &pp1, const pointpair &pp2, const pointpair &pp3);

public:
    // 初始化函数
    NearestPoint(int ** points, int sum) {
        pSum = sum;

        sortedX = initArray(sum, 2);
        sortedY = initArray(sum, 2);

        deepCopy(points, sortedX, sum);
        deepCopy(points, sortedY, sum);

        // 先按照x进行排序
        sort(sortedX, sortedX+sum, sortFunc_x);
        sort(sortedY, sortedY+sum, sortFunc_y);
    };
    int ** getNearestPoint(float *dis);
};

#endif //ALGLAB_UTILITY_H