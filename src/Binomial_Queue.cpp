#include "Binomial_Queue.h"

/**
 * @brief 判断二项队列是否为空
 * 
 * @return 如果为空，则返回true；如果非空，则返回false
 */
bool Binomial_Queue::isEmpty()
{
    return mRoot == nullptr;
}

/**
 * @brief 获取二项队列中的最小键值，公共方法。
 * 
 * @param pkey 用于存储最小键值的指针。
 * @return 如果找到最小键值，则返回true；如果二项队列为空，则返回false。
 */
bool Binomial_Queue::minimum(int *pkey)
{
    if(mRoot == nullptr)
        return false;
    BiNode *x, *y;
    x = mRoot->next;
    y = mRoot;
    while(x != nullptr)
    {
        if (x->key < y->key)
        {
            y = x;
        }
        x = x->next;
    }
    *pkey = y->vertex;
    return true;
}

/**
 * @brief 将同度数的两个二项树合并为一个二项树。
 * 
 * @param child 将被链接的子节点。
 * @param root 将要链接子节点的根节点。
 */
void Binomial_Queue::link(BiNode *child, BiNode *root)
{
    child->parent = root;
    child->next = root->child;
    root->child = child;
    root->degree++;
}

/**
 * @brief 将两个二项队列重组根节点顺序并合并为一个队列，不改变各个二项树结构。
 * 
 * @param h1 第一个待合并的二项堆。
 * @param h2 第二个待合并的二项堆。
 * @return BiNode* 合并后的二项堆的根节点。
 */
BiNode *Binomial_Queue::merge(BiNode *h1, BiNode *h2)
{
    if(h1 == nullptr) return h2;
    if(h2 == nullptr) return h1;
    BiNode *root = nullptr, *tail = nullptr;
    while(h1 && h2)
    {
        BiNode* temp = nullptr;
        if(h1->degree <= h2->degree)
        {
            temp = h1;
            h1 = h1->next;
        }
        else
        {
            temp = h2;
            h2 = h2->next;
        }
        if(tail == nullptr)
            root = tail = temp;
        else
        {
            tail->next = temp;
            tail = temp;
        }
    }
    tail->next = (h1 != nullptr ? h1 : h2);
    return root;
}

/**
 * @brief 将两个二项队列合并成一个二项队列，调用link。
 * 
 * @param h1 第一个待合并的二项堆。
 * @param h2 第二个待合并的二项堆。
 * @return BiNode* 合并后的二项堆的根节点。
 */
BiNode *Binomial_Queue::unionHeaps(BiNode *h1, BiNode *h2)
{
    BiNode* root = merge(h1, h2);
    if (root == nullptr) return nullptr;
    BiNode *prev = nullptr, *curr = root, *next = curr->next;
    while (next)
    {
        if((curr->degree != next->degree) || (next->next && next->next->degree == curr->degree))
        {
            prev = curr;
            curr = next;
        }
        else if(curr->key <= next->key)
        {
            curr->next = next->next;
            link(next, curr);
        }
        else
        {
            if(prev == nullptr)
                root = next; 
            else 
                prev->next = next;
            link(curr, next);
            curr = next;
        }
        next = curr->next;
    }
    return root;
}

/**
 * @brief 反转二项队列的头链表。
 * 
 * @param root 要被反转的二项队列的根节点。
 * @return BiNode* 反转后链表的新头节点。
 */
BiNode *Binomial_Queue::reverse(BiNode *root)
{
    BiNode* next = nullptr;
    BiNode* tail = nullptr;
    if(root == nullptr)
        return root;
    root->parent = nullptr; 
    while(root->next != nullptr)
    {
        next = root->next;  
        root->next = tail;  
        tail = root;        
        root = next;        
        root->parent = nullptr; 
    }
    root->next = tail;
    return root;
}

/**
 * @brief 从二项堆中提取最小节点，调用reverse和unionHeaps。
 * 
 * @param root 二项堆的根节点。
 * @return BiNode* 提取操作后的二项堆的新根节点。
 */
BiNode *Binomial_Queue::removeMin(BiNode *root)
{
    if(root == nullptr)
        return nullptr;
    BiNode *x = root->next, *prev_x = root;
    BiNode *y = root, *prev_y = nullptr;
    while (x != nullptr) {
        if (x->key < y->key)
        {
            y = x;
            prev_y = prev_x;
        }
        prev_x = x;
        x = x->next;
    }
    if (prev_y == nullptr)
        root = root->next;  
    else
        prev_y->next = y->next; 
    //reverse拆成独立的二项堆是为了将度数递减变为递增
    BiNode* child = reverse(y->child);
    root = unionHeaps(root, child);
    delete y;  
    return root;
}

/**
 * @brief 降低二项堆中某节点的键值。
 * 
 * @param node 要降低键值的节点。
 * @param key 新的键值。
 */
void Binomial_Queue::decreaseKey(BiNode *node, int key)
{
    if(key >= node->key || contains(key))
    {
        throw "decrease failed";
        return;
    }
    node->key = key;
    BiNode* child = node;
    BiNode* parent = node->parent;
    while (parent != nullptr && child->key < parent->key)
    {
        // 交换parent和child的键值
        int tmp = parent->key;
        parent->key = child->key;
        child->key = tmp;
        // 向上移动
        child = parent;
        parent = child->parent;
    }
    return;
}

/**
 * @brief 提高二项堆中某节点的键值。
 * 
 * @param node 要提高键值的节点。
 * @param key 新的键值。
 */
void Binomial_Queue::increaseKey(BiNode *node, int key)
{
    if(key <= node->key || contains(key))
    {
        throw "increase failed";
        return;
    }
    node->key = key;
    BiNode* current = node;
    BiNode* child = current->child;
    while(child != nullptr)
    {
        if (current->key > child->key)
        {
            // 找出current的所有孩子中键值最小的节点
            BiNode* least = child; 
            while (child->next != nullptr)
            {
                if (child->next->key < least->key)
                    least = child->next;
                child = child->next;
            }
            // 交换current节点和最小孩子节点的键值
            int temp = least->key;
            least->key = current->key;
            current->key = temp;
            // 继续调整交换后的最小孩子节点，保持最小堆的性质
            current = least;
            child = current->child;
        }else
            child = child->next;
    }
}

/**
 * @brief 更新二项堆中某节点的键值，调用increase和decrease。
 * 
 * @param node 要更新键值的节点。
 * @param key 新的键值。
 */
void Binomial_Queue::updateKey(BiNode *node, int key)
{
    if(node == nullptr)
        return;
    if(key < node->key)
        decreaseKey(node, key);
    else if(key > node->key)
        increaseKey(node, key);
    else
        throw "No need to update";
    return;
}

/**
 * @brief 在二项堆中搜索具有给定顶点的节点。
 * 
 * @param root 二项堆的根节点。
 * @param vertex 要搜索的顶点。
 * @return BiNode* 如果找到具有给定顶点的节点，则返回该节点；否则，返回 nullptr。
 */
BiNode *Binomial_Queue::search(BiNode *root, int vertex)
{
    if (root == nullptr)
        return nullptr;
    BiNode* node = root;
    while (node != nullptr)
    {
        if (node->vertex == vertex)
            return node;
        else
        {
            BiNode* found = search(node->child, vertex);
            if(found != nullptr)
                return found;
            node = node->next;
        }
    }

    return nullptr;
}

/**
 * @brief 将另一个二项队列合并到当前二项队列中，公共方法。
 * 
 * @param other 要合并的另一个二项队列。
 */
void Binomial_Queue::combine(Binomial_Queue *other)
{
    if (other != nullptr && other->mRoot != nullptr)
        mRoot = merge(mRoot, other->mRoot);
}

/**
 * @brief 向二项队列中插入节点
 * 
 * @param key 节点键值
 * @param vertex 节点名称
 */
void Binomial_Queue::insert(int key, int vertex)
{
    if (contains(key))
    {
        throw "alreay exists";
        return;
    }
    BiNode* node = new BiNode(key, vertex);
    mRoot = unionHeaps(mRoot, node);
}

/**
 * @brief 移除二项队列中最小值对应节点
 * 
 */
void Binomial_Queue::removeMin()
{
    mRoot = removeMin(mRoot);
}

/**
 * @brief 在二项队列中查询是否包含顶点值为key的节点
 * 
 * @param key 实际上是顶点名称
 * @return 如果包含返回true，否则返回false
 */
bool Binomial_Queue::contains(int key)
{
    return search(mRoot, key) != nullptr ? true : false;
}

/**
 * @brief 更新节点值
 * 
 * @param vertex 要更新的节点的顶点名
 * @param newkey 要更新节点的新键值
 */
void Binomial_Queue::update(int vertex, int newkey)
{
    BiNode* node;
    node = search(mRoot, vertex);
    if(node != nullptr)
        updateKey(node, newkey);
}
