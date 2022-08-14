#include <iostream>
#include <algorithm>
#include <cstdio>
using namespace std;
template<typename T> struct SplayTreeNode {
    int ff, cnt, ch[2], size, mark;
    T val;
    void init(T val, int ff, int cnt=1, int size=1) {
        this->val = val;
        this->ff = ff;
        this->cnt = cnt;
        this->size = size;
        ch[0] = ch[1] = mark = 0;
    }
};

template<typename T> struct SplayTree {
    static const int N = 1e5 + 10;  // 数组大小，改
    static const T INF = 2147483647;    // 值域范围，改
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
            // 初始化平衡树节点
            t[u].init(v, ff);
        }
        splay(u, 0);
    }

    // 执行reverse，下传标记
    void pushdown(int x)
    {
        if(x && t[x].mark) {
            t[t[x].ch[0]].mark ^= 1;
            t[t[x].ch[1]].mark ^= 1;
            t[x].mark = 0;
            swap(t[x].ch[0], t[x].ch[1]);
        }
    }

    // 返回中序遍历中的第x个数的节点编号
    int getx(int x)
    {
        int u = root;
        while(1)
        {
            pushdown(u);
            if(x <= t[t[u].ch[0]].size) {
                u = t[u].ch[0];
            }
            else {
                x -= t[t[u].ch[0]].size + 1;
                if(!x)  return u;
                u = t[u].ch[1];
            }
        }
    }
    // 翻转区间l, r
    void reverse(int l, int r)
    {
        int u = getx(l - 1);
        int v = getx(r + 1);
        splay(u, 0);
        splay(v, u);
        int x = t[t[root].ch[1]].ch[0];
        t[x].mark ^= 1;
    }
    // 中序遍历的到最终序列
    void dfs(int x)
    {
        if(!x)   return ;
        pushdown(x);
        dfs(t[x].ch[0]);
        if(t[x].val != -INF && t[x].val != INF) {
            // cout << t[x].val << ' ';
            printf("%d ", t[x].val);
        }
        dfs(t[x].ch[1]);
    }
    private:
    // 更新节点 x 作为子树的子树大小
    inline void update(int x)
    {
        if(x) {
            t[x].size = t[x].cnt;
            if(t[x].ch[0])  t[x].size += t[t[x].ch[0]].size;
            if(t[x].ch[1])  t[x].size += t[t[x].ch[1]].size;
        }
    }
    // 将节点 x，向上移动一层
    inline void rotate(int x)
    {
        int y = t[x].ff;
        pushdown(x), pushdown(y);
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
    int n, m;
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; i++)
        tree.insert(i);
    while(m--)
    {
        int l, r;
        scanf("%d%d", &l, &r);
        tree.reverse(l + 1, r + 1);
    }
    tree.dfs(tree.root);
    return 0;
}