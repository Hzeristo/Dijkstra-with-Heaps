#include "Fib_Heap.h"
#include "default.h"

using namespace std;

FibHeap::FibHeap()
{
    keyNum = 0;
    maxDegree = 0;
    min = NULL;
    cons = NULL;
}

FibHeap::~FibHeap()
{
    destroy();
}

/**
 * @brief 从双链表中移除节点
 * 
 * @param node 要移除的节点
 */
void FibHeap::removeNode(FibNode *node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}

/**
 * @brief 将节点添加到双链表的root节点之前
 * 
 * @param node 要添加的节点
 * @param root 根节点
 */
void FibHeap::addNode(FibNode *node, FibNode *root)
{
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
}

/**
 * @brief 检查堆是否为空
 * 
 * @return 如果堆为空则返回true，否则返回false
 */
bool FibHeap::isEmpty()
{
    if(min == nullptr)
        return true;
    return false;
}

/**
 * @brief 将节点插入到堆中
 * 
 * @param node 要插入的节点
 */
void FibHeap::insert(FibNode *node)
{
    if (keyNum == 0)
        min = node;
    else
    {
        addNode(node, min);
        if (node->key < min->key)
            min = node;
    }
    keyNum++;
}

/**
 * @brief 根据键值和顶点创建新节点，并插入到堆中
 * 
 * @param key 节点的键值
 * @param vertex 节点的顶点
 */
void FibHeap::insert(int key, int vertex)
{
    FibNode *node;

    node = new FibNode(key, vertex);
    if (node == NULL)
        return;

    insert(node);
}

/**
 * @brief 将双向链表b连接到双向链表a的后面
 * 
 * @param a 列表a
 * @param b 列表b
 */
void FibHeap::catList(FibNode *a, FibNode *b)
{
    FibNode *tmp;

    tmp = a->right;
    a->right = b->right;
    b->right->left = a;
    b->right = tmp;
    tmp->left = b;
}

/**
 * @brief 合并另一个斐波那契堆到当前堆中
 * 
 * @param other 要合并的另一个斐波那契堆
 */
void FibHeap::combine(FibHeap *other)
{
    if (other == NULL)
        return;
    if (other->maxDegree > this->maxDegree)
        swap(*this, *other);
    if ((this->min) == NULL) // this无"最小节点"
    {
        this->min = other->min;
        this->keyNum = other->keyNum;
        free(other->cons);
        delete other;
    }
    else if ((other->min) == NULL) // this有"最小节点" && other无"最小节点"
    {
        free(other->cons);
        delete other;
    } // this有"最小节点" && other有"最小节点"
    else
    {
        // 将"other中根链表"添加到"this"中
        catList(this->min, other->min);

        if (this->min->key > other->min->key)
            this->min = other->min;
        this->keyNum += other->keyNum;
        free(other->cons);
        delete other;
    }
}

/**
 * @brief 从堆中提取最小节点
 * 
 * @return 最小节点
 */
FibNode *FibHeap::extractMin()
{
    FibNode *p = min;

    if (p == p->right)
        min = NULL;
    else
    {
        removeNode(p);
        min = p->right;
    }
    p->left = p->right = p;

    return p;
}

/**
 * @brief 将node链接到root根结点
 * 
 * @param node 新节点
 * @param root 根节点
 */
void FibHeap::link(FibNode *node, FibNode *root)
{
    // 将node从双链表中移除
    removeNode(node);
    // 将node设为root的孩子
    if (root->child == NULL)
        root->child = node;
    else
        addNode(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = false;
}

/**
 * @brief 创建consolidate所需空间
 */
void FibHeap::makeCons()
{
    int old = maxDegree;

    // 计算log2(keyNum)，"+1"意味着向上取整！
    // ex. log2(13) = 3，向上取整为3+1=4。
    maxDegree = (log(keyNum) / log(2.0)) + 1;
    if (old >= maxDegree)
        return;

    // 因为度为maxDegree可能被合并，所以要maxDegree+1
    cons = (FibNode **)realloc(cons,
                                sizeof(FibHeap *) * (maxDegree + 1));
}

/**
 * @brief 合并斐波那契堆的根链表中左右相同度数的树
 */
void FibHeap::consolidate()
{
    int i, d, D;
    FibNode *x, *y, *tmp;

    makeCons(); //开辟哈希所用空间
    D = maxDegree + 1;

    for (i = 0; i < D; i++)
        cons[i] = NULL;

    // 合并相同度的根节点，使每个度数的树唯一
    while (min != NULL)
    {
        x = extractMin(); // 取出堆中的最小树(最小节点所在的树)
        d = x->degree;    // 获取最小树的度数
        // cons[d] != NULL，意味着有两棵树(x和y)的"度数"相同。
        while (cons[d] != NULL)
        {
            y = cons[d]; // y是"与x的度数相同的树"
            if (x->key > y->key) // 保证x的键值比y小
                swap(x, y);

            link(y, x); // 将y链接到x中
            cons[d] = NULL;
            d++;
        }
        cons[d] = x;
    }
    min = NULL;
    // 将cons中的结点重新加到根表中
    for (i = 0; i < D; i++)
    {
        if (cons[i] != NULL)
        {
            if (min == NULL)
                min = cons[i];
            else
            {
                addNode(cons[i], min);
                if ((cons[i])->key < min->key)
                    min = cons[i];
            }
        }
    }
}

/**
 * @brief 移除最小节点
 * 
 */
void FibHeap::removeMin()
{
    if (min == NULL)
        return;

    FibNode *child = NULL;
    FibNode *m = min;
    // 将min每一个儿子(儿子和儿子的兄弟)都添加到"斐波那契堆的根链表"中
    while (m->child != NULL)
    {
        child = m->child;
        removeNode(child);
        if (child->right == child)
            m->child = NULL;
        else
            m->child = child->right;

        addNode(child, min);
        child->parent = NULL;
    }
    // 将m从根链表中移除
    removeNode(m);
    // 若m是堆中唯一节点，则设置堆的最小节点为NULL；
    // 否则，设置堆的最小节点为一个非空节点(m->right)，然后再进行调节。
    if (m->right == m)
        min = NULL;
    else
    {
        min = m->right;
        consolidate();
    }
    keyNum--;

    delete m;
}

/**
 * @brief 获取斐波那契堆中最小键值对应节点名，并保存到pkey中
 * 
 * @param pkey 保存最小键值对应节点名的变量
 * @return 成功返回true，否则返回false
 */
bool FibHeap::minimum(int *pkey)
{
    if (min == NULL || pkey == NULL)
        return false;

    *pkey = min->vertex;
    return true;
}

/**
 * @brief 修改节点的度数
 * 
 * @param parent 父节点
 * @param degree 度数
 */
void FibHeap::renewDegree(FibNode *parent, int degree)
{
    parent->degree -= degree;
    if (parent->parent != NULL)
        renewDegree(parent->parent, degree);
}

/**
 * @brief 将节点从父节点parent的子链接中剥离出来，并使其成为堆的根链表中的一员
 * 
 * @param node 要剥离的节点
 * @param parent 父节点
 */
void FibHeap::cut(FibNode *node, FibNode *parent)
{
    removeNode(node);
    renewDegree(parent, node->degree);
    // node没有兄弟
    if (node == node->right)
        parent->child = NULL;
    else
        parent->child = node->right;

    node->parent = NULL;
    node->left = node->right = node;
    node->marked = false;
    // 将"node所在树"添加到"根链表"中
    addNode(node, min);
}

/**
 * @brief 对节点进行级联剪切
 * 
 * @param node 要剪切的节点
 */
void FibHeap::cascadingCut(FibNode *node)
{
    FibNode *parent = node->parent;
    if (parent != NULL)
    {
        if (node->marked == false)
            node->marked = true;
        else
        {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

/**
 * @brief 将节点的键值减小为key
 * 
 * @param node 要减小键值的节点
 * @param key 新键值
*/
void FibHeap::decrease(FibNode *node, int key)
{
    FibNode *parent;

    if (min == NULL || node == NULL)
        return;

    if (key >= node->key)
    {
        cout << "decrease failed: the new key(" << key << ") "
             << "is no smaller than current key(" << node->key << ")" << endl;
        return;
    }

    node->key = key;
    parent = node->parent;
    if (parent != NULL && node->key < parent->key)
    {
        // 将node从父节点parent中剥离出来，并将node添加到根链表中
        cut(node, parent);
        cascadingCut(parent);
    }

    // 更新最小节点
    if (node->key < min->key)
        min = node;
}

/**
 * @brief 将节点的键值增加为key
 * 
 * @param node 要增加键值的节点
 * @param key 新键值
 */
void FibHeap::increase(FibNode *node, int key)
{
    FibNode *child, *parent, *right;

    if (min == NULL || node == NULL)
        return;

    if (key <= node->key)
    {
        cout << "increase failed: the new key(" << key << ") "
             << "is no greater than current key(" << node->key << ")" << endl;
        return;
    }
    // 将node每一个儿子(不包括孙子,重孙,...)都添加到"斐波那契堆的根链表"中
    while (node->child != NULL)
    {
        child = node->child;
        removeNode(child); // 将child从node的子链表中删除
        if (child->right == child)
            node->child = NULL;
        else
            node->child = child->right;

        addNode(child, min); // 将child添加到根链表中
        child->parent = NULL;
    }
    node->degree = 0;
    node->key = key;
    // 如果node不在根链表中，
    // 则将node从父节点parent的子链接中剥离出来，
    // 并使node成为"堆的根链表"中的一员，
    // 然后进行"级联剪切"
    // 否则，则判断是否需要更新堆的最小节点
    parent = node->parent;
    if (parent != NULL)
    {
        cut(node, parent);
        cascadingCut(parent);
    }
    else if (min == node)
    {
        right = node->right;
        while (right != node)
        {
            if (node->key > right->key)
                min = right;
            right = right->right;
        }
    }
}

/**
 * @brief 更新节点的键值为key
 * 
 * @param node 要更新键值的节点
 * @param key 新键值
 */
void FibHeap::update(FibNode *node, int key)
{
    if (key < node->key)
        decrease(node, key);
    else if (key > node->key)
        increase(node, key);
    else
        cout << "No need to update" << endl;
}

/**
 * @brief 更新节点的键值为key
 * 
 * @param vertex 要更新的节点的顶点
 * @param newkey 新键值
 */
void FibHeap::update(int vertex, int newkey)
{
    FibNode *node;

    node = search(vertex);
    if (node != NULL)
        update(node, newkey);
    else
        insert(newkey, vertex);
}

/**
 * @brief 在最小堆中查找节点
 * 
 * @param root 根节点
 * @param vertex 要查找的节点的顶点
 * @return 节点指针，如果找到则返回指向该节点的指针，否则返回NULL
 */
FibNode *FibHeap::search(FibNode *root, int vertex)
{
    FibNode *t = root; // 临时节点
    FibNode *p = NULL; // 要查找的节点
    if (root == NULL)
        return root;
    do
    {
        if (t->vertex == vertex)
        {
            p = t;
            break;
        }
        else
        {
            if ((p = search(t->child, vertex)) != NULL)
                break;
        }
        t = t->right;
    } while (t != root);
    return p;
}

/**
 * @brief 在斐波那契堆中查找节点
 * 
 * @param vertex 要查找的节点的顶点
 * @return 节点指针，如果找到则返回指向该节点的指针，否则返回NULL
 */
FibNode *FibHeap::search(int vertex)
{
    if (min == NULL)
        return NULL;

    return search(min, vertex);
}

/**
 * @brief 在斐波那契堆中是否存在节点
 * 
 * @param vertex 节点的顶点
 * @return 存在返回true，否则返回false
 */
bool FibHeap::contains(int vertex)
{
    return search(vertex) != NULL ? true : false;
}

/**
 * @brief 删除节点
 * 
 * @param node 要删除的节点
 */
void FibHeap::remove(FibNode *node)
{
    int m = min->key - 1;
    decrease(node, m - 1);
    removeMin();
}

/**
 * @brief 删除具有特定键值的节点
 * 
 * @param key 节点的键值
 */
void FibHeap::remove(int key)
{
    FibNode *node;

    if (min == NULL)
        return;

    node = search(key);
    if (node == NULL)
        return;

    remove(node);
}

/**
 * @brief 销毁斐波那契节点
 */
void FibHeap::destroyNode(FibNode *node)
{
    FibNode *start = node;
    if (node == NULL)
        return;
    do
    {
        destroyNode(node->child);
        // 销毁node，并将node指向下一个
        node = node->right;
        delete node->left;
    } while (node != start);
}

/**
 * @brief 销毁斐波那契节点
 */
void FibHeap::destroy()
{
    destroyNode(min);
    free(cons);
}


