#include <iostream>
#include <algorithm>
#include <cstdio>
using namespace std;
template<typename T> struct SplayTreeNode {
    int ff, cnt, ch[2], size;
    T val;
};

template<typename T> struct SplayTree {
    static const int N = 2e6 + 10;  // 数组大小，改
    static const T INF = 1073741824;    // 值域范围，改
    SplayTreeNode<T> t[N];
    int root, tot;
    SplayTree() {clear();}
    void clear() {
        root = 0;
        tot = 0;
        // 因为加了一个最大值，一个最小值，故用排名求值时要+1，用值求排名时不用+1
        insert(INF);
        insert(-INF);
    }
    // 插入一个整数 v
    void insert(T v) {
        int u = root, ff = 0;
        while(u && t[u].val != v)
        {
            ff = u;
            u = t[u].ch[v > t[u].val];
        }
        if(u)   t[u].cnt++;
        else {
            u = ++tot;
            if(ff)  t[ff].ch[v > t[ff].val] = u;
            t[u].ch[0] = t[u].ch[1] = 0;
            t[u].ff = ff;
            t[u].val = v;
            t[u].cnt = 1;
            t[u].size = 1;
        }
        splay(u, 0);
    }
    // 求 v 的后继（后继定义为大于 v，且最小的数）。
    int Next(T v) {
        find(v);
        int u = root;
        if(t[u].val > v)   return u;
        u = t[u].ch[1];
        while(t[u].ch[0])
            u = t[u].ch[0];
        return u;
    }
    // 求 v 的前驱（前驱定义为小于 v，且最大的数）。
    int Last(T v) {
        find(v);
        int u = root;
        if(t[u].val < v)    return u;
        u = t[u].ch[0];
        while(t[u].ch[1])   u = t[u].ch[1];
        return u;
    }
    // 删除一个整数 v（若有多个相同的数，只删除一个）
    void erase(T v)
    {
        int last = Last(v);
        int Net = Next(v);
        splay(last, 0);
        splay(Net, last);
        int del = t[Net].ch[0];
        if(t[del].cnt > 1) {
            t[del].cnt--;
            splay(del, 0);
        }
        else t[Net].ch[0] = 0;
    }
    // 返回第k大值
    T rankToVal(int x)
    {
        int u = root;
        if(x <= 0 || t[u].size < x)   return INF;
        while(1)
        {
            int y = t[u].ch[0];
            if(x > t[y].size + t[u].cnt) {
                x -= t[y].size + t[u].cnt;
                u = t[u].ch[1];
            }
            else if(x <= t[y].size)
                u = y;
            else {
                splay(u, 0);
                return t[u].val;
            }
        }
    }
    // 查询整数 v 的排名（排名定义为比当前数小的数的个数 +1）
    int valToRank(T v)
    {
        find(v);
        if(v <= t[root].val) {  // 看find()的注释
            return t[t[root].ch[0]].size;
        }
        return t[root].size - t[t[root].ch[1]].size;
    }

    private:
    /*  找到值 v，并将所在值节点旋转到根节点，
        如果找不到值v，则将找到v 的前驱或者后驱，然后将v 的前驱或者后驱旋转到根节点
        如果将v 的前驱旋转到了根节点，则根节点的右子节点就是v 的后驱
        如果将v 的后驱旋转到了根节点，则根节点的左子节点就是v 的前驱 */
    inline void find(T v)
    {
        int u = root;
        if(!u)  return ;
        while(t[u].ch[v > t[u].val] && v != t[u].val)
            u = t[u].ch[v > t[u].val];
        splay(u, 0);
    }
    // 更新节点 x 作为子树的子树大小
    inline void update(int x)
    {
        t[x].size = t[t[x].ch[0]].size + t[t[x].ch[1]].size + t[x].cnt;
    }
    // 将节点 x，向上移动一层
    inline void rotate(int x)
    {
        int y = t[x].ff;
        int z = t[y].ff;
        int k = (t[y].ch[1] == x);
        t[z].ch[(t[z].ch[1] == y)] = x;
        t[x].ff = z;
        t[y].ch[k] = t[x].ch[k ^ 1];
        t[t[x].ch[k ^ 1]].ff = y;
        t[x].ch[k ^ 1] = y;
        t[y].ff = x;
        update(y);
        update(x);
    }
    // 将节点 x，向上移动到节点 s的子节点位置，如果s = 0，则将节点 x，移动到根节点位置
    inline void splay(int x, int s)
    {
        while(t[x].ff != s)
        {
            int y = t[x].ff, z = t[y].ff;
            if(z != s)  (t[z].ch[0]==y) ^ (t[y].ch[0] == x)? rotate(x):rotate(y);
            rotate(x);
        }
        if(s == 0)  root = x;
    }
};
SplayTree<int> tree;
int main()
{
    int n, last = 0, v = 0, m, ans = 0;
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; i++)
    {
        scanf("%d", &v);
        tree.insert(v);
    }
    while(m--)
    {
        int opt, x;
        scanf("%d%d", &opt, &x);
        switch (opt)
        {
        case 1:
            tree.insert(last ^ x);
            break;
        case 2:
            tree.erase(last ^ x);
            break;
        case 3:
            last = tree.valToRank(last ^ x);
            ans ^= last;
            break;
        case 4:
            last = tree.rankToVal((last ^ x) + 1);
            ans ^= last;
            break;
        case 5:
            last = tree.t[tree.Last(last ^ x)].val;
            ans ^= last;
            break;
        case 6:
            last = tree.t[tree.Next(last ^ x)].val;
            ans ^= last;
            break;
        default:
            break;
        }
    }
    printf("%d\n", ans);
    return 0;
}