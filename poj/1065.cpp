#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

pair<int, int> arr[5000];
bool flag[5000];

bool compare(const pair<int, int>a, const pair<int, int>b) {

    if (a.first == b.first) {
        return a.second <= b.second;
    }
    return a.first < b.first;

}
int main() {
    // 想法：先按照长度排序，然后查找单调递增子序列
    
    int n; // 组数
    cin >> n;

    while (n > 0) {
        int num; // 该组数据的点的数目
        cin >> num;

        memset(flag, 0, sizeof(flag));

        for (int i=0; i < num; ++i) {
            cin >> arr[i].first >> arr[i].second;
        }

        // 然后进行排序
        sort(arr, arr+num, compare);
        
        int max;
        int result = 0;

        for (int i=0; i < num; ++i) {
            max = arr[i].second;
            if (!flag[i]) {
                // i没有检测过
                for (int j=i+1; j < num; ++j) {
                    if (!flag[j] && max <= arr[j].second) {
                        // 加入序列
                        flag[j] = true;
                        max = arr[j].second;
                    }
                }
                // 说明又添加了一个序列
                ++result;
            }
        }
        cout << result << endl;
        --n;
    }
    return 0;
}
