// p3835
#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
using namespace std;
#define endl '\n'
template<typename T> struct FHQTreapNode {
    T val;
    int size, key, ch[2];
};
template<typename T, int N> struct FHQTreap {
    FHQTreapNode<T> t[N];
    const T INF = 0x7fffffff;
    int tot, root[500010];
    FHQTreap() {
        srand((unsigned)time(NULL));
        t[0].size = t[0].ch[0] = t[0].ch[1] = 0;      
        tot = 0;
        root[0] = 1;
        New(INF);
        insert(0, 0, -INF);
    }
    void insert(int newRoot, int oldroot, T val) {
        int x = 0, y = 0, z;
        z = New(val);
        split(root[oldroot], x, y, val);
        merge(x, x, z);
        merge(root[newRoot], x, y);
    }
    void erase(int newRoot, int oldroot, T val) {
        int x = 0, y = 0, z = 0;
        split(root[oldroot], x, y, val);
        split(x, x, z, val - 1);
        merge(z, t[z].ch[0], t[z].ch[1]);
        merge(x, x, z);
        merge(root[newRoot], x, y);
    }
    int getRankByVal(int newRoot, int oldroot, T val) {
        int x = 0, y = 0, z = 0;
        split(root[oldroot], x, y, val - 1);
        int ans = t[x].size + 1;
        merge(root[newRoot], x, y);
        return ans;
    }
    T getValByRank(int newRoot, int oldroot, int x) {
        root[newRoot] = root[oldroot];
        return findKth(root[oldroot], x);
    }
    T getPre(int newRoot, int oldroot, T val) {
        int x = 0, y = 0;
        split(root[oldroot], x, y, val - 1);
        T ans = findKth(x, t[x].size);
        merge(root[newRoot], x, y);
        return ans;
    }
    T getNex(int newRoot, int oldroot, T val) {
        int x = 0, y = 0;
        split(root[oldroot], x, y, val);
        T ans = findKth(y, 1);
        merge(root[newRoot], x, y);
        return ans;
    }
    void dfs(int now) {
        if(!now) return ;
        dfs(t[now].ch[0]);
        cout << t[now].val << ' ';
        dfs(t[now].ch[1]);
    }
    private:
    T findKth(int now, int k) { // now为树根,k为第k大数
        // 找第k大
        // if(now == 0)    return -INF;
        while(t[t[now].ch[0]].size + 1 != k)
        {
            if(t[t[now].ch[0]].size >= k) {
                now = t[now].ch[0];
            }
            else {
                k -= t[t[now].ch[0]].size + 1;
                now = t[now].ch[1];
            }
            // if(now == 0)    return -INF;
        }
        return t[now].val;
    }
    int New(T val=0) {
        t[++tot].size = 1;
        t[tot].val = val;
        t[tot].key = rand();
        t[tot].ch[0] = t[tot].ch[1] = 0;
        return tot;
    }
    void update(int x) {
        if(x) {
            t[x].size = 1;
            if(t[x].ch[0])  t[x].size += t[t[x].ch[0]].size;
            if(t[x].ch[1])  t[x].size += t[t[x].ch[1]].size;
        }
    }
    void split(int now, int &x, int &y, T val) {
        if(now == 0) {
            x = y = 0;
            return ;
        }
        if(t[now].val <= val) {
            // x = now;
            x = New();
            t[x] = t[now];            
            split(t[now].ch[1], t[x].ch[1], y, val);
            update(x);
        }
        else {
            // y = now;
            y = New();
            t[y] = t[now];
            split(t[now].ch[0], x, t[y].ch[0], val);
            update(y);
        }
        // update(now);
    }
    void merge(int &now, int x, int y)
    {
        if(x == 0 || y == 0) {
            now = x + y;
            return ;
        }
        if(t[x].key < t[y].key) {
            // now = x;
            now = New();
            t[now] = t[x];
            merge(t[now].ch[1], t[x].ch[1], y);
        }
        else {
            // now = y;
            now = New();
            t[now] = t[y];
            merge(t[now].ch[0], x, t[y].ch[0]);
        }
        update(now);
    }
};

const int N = 5e5 + 10;
FHQTreap<int, N * 50> tree;
int main()
{
    std::ios::sync_with_stdio(false);std::cin.tie(0);std::cout.tie(0);
    int n, t;
    cin >> n;
    for(int i = 1; i <= n; i++)
    {
        int v, opt, x;
        cin >> v >> opt >> x;
        switch (opt)
        {
        case 1:
            tree.insert(i, v, x);
            break;
        case 2:
            tree.erase(i, v, x);
            break;
        case 3:
            t = tree.getRankByVal(i, v, x);
            cout << t - 1 << endl;
            break;
        case 4:
            t = tree.getValByRank(i, v, x + 1);
            cout << t << endl;
            break;
        case 5:
            t = tree.getPre(i, v, x);
            cout << t << endl;
            break;
        case 6:
            t = tree.getNex(i, v, x);
            cout << t << endl;
            break;
        default:
            break;
        }
    }
    // for(int i = 1; i <= n; i++)
    //     tree.dfs(tree.root[i]), cout << '\n';
    return 0;
}