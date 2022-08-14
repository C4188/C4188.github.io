#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
using namespace std;
template<typename T> struct FHQTreapNode {
    T val;
    int size, key, ch[2], tag;
};
template<typename T, int N> struct FHQTreap {
    FHQTreapNode<T> t[N];
    const T INF = 0x7fffffff;
    int tot, root;
    FHQTreap() {
        t[0].size = 0;
        srand((unsigned)time(NULL));        
        tot = 0;
        root = 1;
        New(INF);
        insert(-INF);
    }
    void insert(T val) {
        int x = 0, y = 0, z;
        z = New(val);
        split(root, x, y, val);
        merge(x, x, z);
        merge(root, x, y);
    }
    void erase(T val) {
        int x = 0, y = 0, z = 0;
        split(root, x, y, val);
        split(x, x, z, val - 1);
        merge(z, t[z].ch[0], t[z].ch[1]);
        merge(x, x, z);
        merge(root, x, y);
    }
    T getValByRank(int x) {
        return findKth(root, x);
    }
    void reverse(int l, int r)
    {
        int x = 0, y = 0, z = 0;
        splitByKth(root, x, y, l - 1);
        splitByKth(y, y, z, r - l + 1);
        t[y].tag ^= 1;
        merge(y, y, z);
        merge(root, x, y);
    }
    void dfs(int x) {
        if(!x)  return ;
        pushdown(x);
        dfs(t[x].ch[0]);
        if(t[x].val != INF && t[x].val != -INF) {
            cout << t[x].val << ' ';
        }
        dfs(t[x].ch[1]);
    }
    private:
    T findKth(int now, int k) { // now为树根,k为第k大数
        // 找第k大
        pushdown(now);
        while(t[t[now].ch[0]].size + 1 != k)
        {
            if(t[t[now].ch[0]].size >= k) {
                now = t[now].ch[0];
            }
            else {
                k -= t[t[now].ch[0]].size + 1;
                now = t[now].ch[1];
            }
            pushdown(now);
        }
        return t[now].val;
    }
    int New(T val) {
        t[++tot].size = 1;
        t[tot].val = val;
        t[tot].key = rand();
        t[tot].ch[0] = t[tot].ch[1] = t[tot].tag = 0;
        return tot;
    }
    void pushdown(int x) {
        if(x && t[x].tag) {
            if(t[x].ch[0])  t[t[x].ch[0]].tag ^= 1;
            if(t[x].ch[1])  t[t[x].ch[1]].tag ^= 1;
            swap(t[x].ch[0], t[x].ch[1]);
            t[x].tag = 0;
        }
    }
    void update(int x) {
        if(x) {
            t[x].size = 1;
            if(t[x].ch[0])  t[x].size += t[t[x].ch[0]].size;
            if(t[x].ch[1])  t[x].size += t[t[x].ch[1]].size;
        }
    }
    // void split(int now, int &x, int &y, T val) {
    //     if(now == 0) {
    //         x = y = 0;
    //         return ;
    //     }
    //     pushdown(now);
    //     if(t[now].val <= val) {
    //         x = now;
    //         split(t[now].ch[1], t[x].ch[1], y, val);
    //     }
    //     else {
    //         y = now;
    //         split(t[now].ch[0], x, t[y].ch[0], val);
    //     }
    //     update(now);
    // }
    void splitByKth(int now, int &x, int &y, int k) {
        if(now == 0) {
            x = y = 0;
            return ;
        }
        pushdown(now);
        if(t[t[now].ch[0]].size + 1 <= k) {
            x = now;
            k -= t[t[now].ch[0]].size + 1;
            splitByKth(t[now].ch[1], t[x].ch[1], y, k);
        }
        else {
            y = now;
            splitByKth(t[now].ch[0], x, t[y].ch[0], k);
        }
        update(now);
    }
    void merge(int &now, int x, int y)
    {
        if(x == 0 || y == 0) {
            now = x + y;
            return ;
        }
        pushdown(x), pushdown(y);
        if(t[x].key < t[y].key) {
            now = x;
            merge(t[now].ch[1], t[x].ch[1], y);
        }
        else {
            now = y;
            merge(t[now].ch[0], x, t[y].ch[0]);
        }
        update(now);
    }
};

const int N = 2e6 + 10;
FHQTreap<int, N> tree;
int main()
{
    int n, m;
    cin >> n >> m;
    for(int i = 1; i <= n; i++)
        tree.insert(i);
    while(m--)
    {
        int l, r;
        cin >> l >> r;
        tree.reverse(l + 1, r + 1);
    }
    tree.dfs(tree.root);
    return 0;
}
/*
5 3
2 4
1 2
3 5
*/