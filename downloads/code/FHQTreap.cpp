#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
using namespace std;
template<typename T> struct FHQTreapNode {
    T val;
    int size, key, ch[2];
};
template<typename T, int N> struct FHQTreap {
    FHQTreapNode<T> t[N];
    const T INF = 0x7fffffff;
    int tot, root;
    FHQTreap() {
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
    int getRankByVal(T val) {
        int x = 0, y = 0, z = 0;
        split(root, x, y, val - 1);
        int ans = t[x].size + 1;
        merge(root, x, y);
        return ans;
    }
    T getValByRank(int x) {
        return findKth(root, x);
    }
    T getPre(T val) {
        int x = 0, y = 0;
        split(root, x, y, val - 1);
        T ans = findKth(x, t[x].size);
        merge(root, x, y);
        return ans;
    }
    T getNex(T val) {
        int x = 0, y = 0;
        split(root, x, y, val);
        T ans = findKth(y, 1);
        merge(root, x, y);
        return ans;
    }
    private:
    T findKth(int now, int k) { // now为树根,k为第k大数
        // 找第k大
        while(t[t[now].ch[0]].size + 1 != k)
        {
            if(t[t[now].ch[0]].size >= k) {
                now = t[now].ch[0];
            }
            else {
                k -= t[t[now].ch[0]].size + 1;
                now = t[now].ch[1];
            }
        }
        return t[now].val;
    }
    int New(T val) {
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
            x = now;
            split(t[now].ch[1], t[x].ch[1], y, val);
        }
        else {
            y = now;
            split(t[now].ch[0], x, t[y].ch[0], val);
        }
        update(now);
    }
    void merge(int &now, int x, int y)
    {
        if(x == 0 || y == 0) {
            now = x + y;
            return ;
        }
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
    std::ios::sync_with_stdio(false);std::cin.tie(0);std::cout.tie(0);
    int n, m;
    cin >> n >> m;
    for(int i = 1; i <= n; i++)
    {
        int a;
        cin >> a;
        tree.insert(a);
    }
    int last = 0, ans = 0;
    while(m--)
    {
        int opt, x;
        cin >> opt >> x;
        if(opt == 1) {
            tree.insert(last ^ x);
        }
        else if(opt == 2) {
            tree.erase(last ^ x);
        }
        else if(opt == 3) {
            int t = tree.getRankByVal(last ^ x) - 1;
            last = t;
            ans ^= t;
        }
        else if(opt == 4) {
            int t = tree.getValByRank((last ^ x) + 1);
            last = t;
            ans ^= t;
        }
        else if(opt == 5) {
            int t = tree.getPre(last ^ x);
            last = t;
            ans ^= t;
        }
        else if(opt == 6) {
            int t = tree.getNex(last ^ x);
            last = t;
            ans ^= t;
        }
    }
    cout << ans << endl;
    return 0;
}