#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

// 用于储存读取的数据
typedef struct {
    int a;
    int b;
    int c;
    int P;
    int R;
} data; // 城市的数目是小于等于10的
int n, m; // n表示城市数量, m表示路径的数目
data path[10];

int visited[11]; // 记录是否访问过,因为每个节点可能被访问多次,所以说不能以1为判断

void dfs_search(int, int);
int minExp = 2147483647; // 设置为最大值
// 决定使用类似于DFS的算法相当于枚举吧,但是可能会出现环,那就对搜索做一个限制
int main () {

    // 读取数据

    // 刚开始似乎忘了处理不可能的情况
    cin >> n >> m;

    for (int i=0; i != m; ++i) {
        cin >> path[i].a >> path[i].b >> path[i].c >> path[i].P >> path[i].R;
    }

    // 然后可以从起始节点开始计算了
    memset(visited, 0, sizeof(visited));
    visited[1] = 1;
    dfs_search(1, 0);

    if (minExp == 2147483647) {
        // 此种情况表明到达不了目的地
        cout << "impossible" << endl;
    }
    else {
        cout << minExp << endl;
    }

    return 0;
}

void dfs_search(int currPos, int currExp) {
    // currPos 为当前的节点位置 currExp 为当前消耗的钱数
    if (currPos == n) {
        // 若是最后的城市
        if (minExp > currExp) {
            minExp = currExp;
        }
        return;
    }
    //如果不是的话就遍历邻接点
    for (int i=0; i <= m; ++i) {
        if (visited[path[i].a] <= 3 && path[i].a == currPos) {
            // 如果之前没有访问过C点的话
            ++visited[path[i].b]; //单线程运行,这样能够保证不会出错
            if (visited[path[i].c] == 0) {
                dfs_search(path[i].b, currExp+path[i].R);
            }
            else {
                dfs_search(path[i].b, currExp+path[i].P);
            }
            --visited[path[i].b];
        }
    }
    return;

}