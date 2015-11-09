#include <iostream>
#include <cstring>
int n; // 湖的数目 2～25
int h; // 时间 1~16
int ti[25]; // 湖间消耗的时间
int fi[25]; // 每个湖的前5分钟钓到鱼的个数
int di[25]; // 每分钟递减的个数
int resultArr[25][25]; // 最多有25个湖这里就不多管了
int total[25];
// 思路：决定采用枚举法和贪心策略

using namespace std;

int main () {

    cin >> n;
    while(n > 0) {
        cin >> h;
        for (int i=0; i < n; ++i) {
            cin >> fi[i];
        }
        for (int i=0; i < n; ++i) {
            cin >> di[i];
        }
        for (int i=0; i < n-1; ++i) {
            cin >> ti[i];
        }
        h *= 12; // 换算为单位时间 1h为12单位时间
        // 获取完数据之后就可以继续走了
        memset(resultArr, 0, sizeof(resultArr));
        memset(total, 0, sizeof(total));

        int tmpFi[n];
        // 枚举最后停留的湖
        for (int i=0; i < n; ++i) {
            //计算路途中的时间消耗
            memcpy(tmpFi, fi, sizeof(int)*n);
            int tmpH = h;
            int travelTime = 0;
            for (int j=0; j < i; ++j) {
                travelTime += ti[j];
            }
            // 如果超出总时间的话
            if (travelTime >= tmpH) {
                break;
            }
            tmpH -= travelTime;
            // 计算总时间
            while ((tmpH--) > 0) {
                // 经过一个时间段

                // 选择鱼量最多的
                int maxfish = -1;
                int maxpos = n-1;
                // 选择最佳的钓鱼位置
                for (int j=i; j >= 0; --j) {
                    if (maxfish <= tmpFi[j]) {
                        maxfish = tmpFi[j];
                        maxpos = j;
                    }
                }

                // 钓鱼
                total[i] += tmpFi[maxpos];
                tmpFi[maxpos] -= di[maxpos];
                if (tmpFi[maxpos] < 0) {
                    tmpFi[maxpos] = 0;
                }
                resultArr[i][maxpos] += 5;
            }
        }
        // 选取最大的位置
        int maxpos = n-1;
        int maxtotal = -1;
        for (int i=n-1; i >= 0; --i) {
            if (maxtotal <= total[i]) {
                maxtotal = total[i];
                maxpos = i;
            }
        }

        // 对结果进行输出
        for (int i=0; i < n; ++i) {
            cout << resultArr[maxpos][i];
            if (i != n-1) {
                cout << ", ";
            }
            else {
                cout << endl;
            }
        }
        cout << "Number of fish expected: " << maxtotal<< endl;

        cin >> n;
        if (n != 0) {
            cout << endl;
        }
    }

    return 0;
}