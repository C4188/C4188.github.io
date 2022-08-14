#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int SIZE = 1e5 + 10;
const int INF = 0x7fffffff;
struct Treap {
    int l, r;   // 左右子节点在数组中的下标
    int val, dat;   // 节点关键码、权值
    int cnt, size;  // 副本数、子树大小
} a[SIZE];  // 数组模拟链表
int tot, root, n;

int New(int val)
{
    a[++tot].val = val;
    a[tot].dat = rand();
    a[tot].cnt = a[tot].size = 1;
    return tot;
}

void Update(int p)  // 更新的是 下标为 p 的节点为根的子树大小(包括副本数) O(1)
{
    a[p].size = a[a[p].l].size + a[a[p].r].size + a[p].cnt;
}

void Build()    // 初始化平衡树 O(1)
{
    srand((unsigned int)time(0));
    New(-INF);
    New(INF);
    root = 1;
    a[1].r = 2;
    Update(root);
}

int GetRankByVal(int p, int val)    // 根据关键码val 返回对应最小排名 O(logN)
{
    if(p == 0) return 0;
    if(val == a[p].val) return a[a[p].l].size + 1;
    if(val < a[p].val)  return GetRankByVal(a[p].l, val);
    return GetRankByVal(a[p].r, val) + a[a[p].l].size + a[p].cnt;
}

int GetValByRank(int p, int rank)   // 根据排名rank 返回对应的关键码 O(logN)
{
    if(p == 0)  return INF;
    if(a[a[p].l].size >= rank)  return GetValByRank(a[p].l, rank);
    if(a[a[p].l].size + a[p].cnt >= rank)   return a[p].val;
    return GetValByRank(a[p].r, rank - a[a[p].l].size - a[p].cnt);
}

void zig(int &p)    // 右旋
{
    int q = a[p].l;
    a[p].l = a[q].r;
    a[q].r = p;
    p = q;
    Update(a[p].r);
    Update(p);
}

void zag(int &p)    // 左旋
{
    int q = a[p].r;
    a[p].r = a[q].l;
    a[q].l = p;
    p = q;
    Update(a[p].l);
    Update(p);
}

void Insert(int &p, int val)    // 向平衡树中插入节点, p 的返回值是存入节点的下标 O(logN)
{
    if(p == 0) {
        p = New(val);
        return ;
    }
    if(val == a[p].val) {
        a[p].cnt++;
        Update(p);
        return ;
    }
    if(val < a[p].val) {
        Insert(a[p].l, val);
        if(a[p].dat < a[a[p].l].dat)    zig(p); // 不满足堆性质, 右旋
    }
    else {
        Insert(a[p].r, val);
        if(a[p].dat < a[a[p].r].dat)    zag(p); // 不满足堆性质, 左旋
    }
    Update(p);
}

int GetPre(int val) // 找到关键码为val 的节点的前驱 O(logN)
{
    int ans = 1;
    int p = root;
    while(p)
    {
        if(val == a[p].val) {
            if(a[p].l > 0) {
                p = a[p].l;
                while(a[p].r > 0)   p = a[p].r; // 左子树上一直向右走
                ans = p;
            }
            break;
        }
        if(a[p].val < val && a[p].val > a[ans].val) ans = p;
        p = val < a[p].val? a[p].l: a[p].r;
    }
    return a[ans].val;
}

int GetNext(int val)    // 找到关键码为val 的节点的后驱 O(logN)
{
    int ans = 2;
    int p = root;
    while(p)
    {
        if(val == a[p].val) {
            if(a[p].r > 0) {
                p = a[p].r;
                while(a[p].l > 0)   p = a[p].l; // 右子树上一直向左走
                ans = p;
            }
            break;
        }
        if(a[p].val > val && a[p].val < a[ans].val) ans = p;
        p = val < a[p].val? a[p].l: a[p].r;
    }
    return a[ans].val;
}

void Remove(int &p, int val)    // 删除关键码为 val的节点， 如果关键码有重复, 则不删除节点, 只减少副本数 O(logN)
{
    if(p == 0)  return ;
    if(val == a[p].val) {   // 检索到了val
        if(a[p].cnt > 1) {  // 有重复,减少副本数即可
            a[p].cnt--;
            Update(p);
            return ;
        }
    
        if(a[p].l || a[p].r) {  // 不是叶子节点, 向下旋转
            if(a[p].r == 0 || a[a[p].l].dat > a[a[p].r].dat) {
                zig(p);
                Remove(a[p].r, val);
            }
            else {
                zag(p);
                Remove(a[p].l, val);
            }
            Update(p);
        }
        else p = 0; // 叶子节点, 删除
        return ;
    }
    val < a[p].val? Remove(a[p].l, val): Remove(a[p].r, val);
    Update(p);
}

int main()
{

    Build();
    scanf("%d", &n);
    while(n--)
    {
        int opt, x;
        scanf("%d%d", &opt, &x);
        switch (opt)
        {
        case 1:
            Insert(root, x);
            break;
        case 2:
            Remove(root, x);
            break;
        case 3:
            printf("%d\n", GetRankByVal(root, x) - 1);
            break;
        case 4:
            printf("%d\n", GetValByRank(root, x + 1));
            break;
        case 5:
            printf("%d\n", GetPre(x));
            break;
        case 6:
            printf("%d\n", GetNext(x));
            break;
        default:
            break;
        }
    }
    return 0;
}