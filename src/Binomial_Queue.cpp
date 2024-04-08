#include "Binomial_Queue.h"


bool Binomial_Queue::isEmpty()
{
    return mRoot == nullptr;
}

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
 * @brief 将两个二项队列合并成一个二项队列。
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
        if ((curr->degree != next->degree) || (next->next && next->next->degree == curr->degree)) {
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
            if (prev == nullptr)
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


BiNode *Binomial_Queue::reverse(BiNode *root)
{
    BiNode* next = nullptr;
    BiNode* tail = nullptr;
    if (root == nullptr)
        return root;
    root->parent = nullptr; 
    while (root->next != nullptr)
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

BiNode *Binomial_Queue::extractMinimum(BiNode *root)
{
    if (root == nullptr) {
        return nullptr;
    }
    BiNode *x = root->next, *prev_x = root;
    BiNode *y = root, *prev_y = nullptr;
    while (x != nullptr) {
        if (x->key < y->key) {
            y = x;
            prev_y = prev_x;
        }
        prev_x = x;
        x = x->next;
    }
    if (prev_y == nullptr) {
        root = root->next;  
    } else {
        prev_y->next = y->next; 
    }
    //reverse拆成独立的二项堆是为了将度数递减变为递增
    BiNode* child = reverse(y->child);
    root = unionHeaps(root, child);
    delete y;  
    return root;
}

void Binomial_Queue::decreaseKey(BiNode *node, int key)
{
    if (key >= node->key || contains(key)) {
        throw "decrease failed";
        return;
    }
    node->key = key;
    BiNode* child = node;
    BiNode* parent = node->parent;
    while (parent != nullptr && child->key < parent->key) {
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

void Binomial_Queue::increaseKey(BiNode *node, int key)
{
    if (key <= node->key || contains(key)) {
        throw "increase failed";
        return;
    }
    node->key = key;
    BiNode* current = node;
    BiNode* child = current->child;
    while (child != nullptr) {
        if (current->key > child->key) {
            // 找出current的所有孩子中键值最小的节点
            BiNode* least = child; 
            while (child->next != nullptr) {
                if (child->next->key < least->key) {
                    least = child->next;
                }
                child = child->next;
            }
            // 交换current节点和最小孩子节点的键值
            int temp = least->key;
            least->key = current->key;
            current->key = temp;
            // 继续调整交换后的最小孩子节点，保持最小堆的性质
            current = least;
            child = current->child;
        } else {
            child = child->next;
        }
    }
}

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

BiNode *Binomial_Queue::search(BiNode *root, int vertex)
{
    if (root == nullptr) {
        return nullptr;
    }

    BiNode* node = root;
    while (node != nullptr) {
        if (node->vertex == vertex) {
            return node;
        } else {
            BiNode* found = search(node->child, vertex);
            if (found != nullptr) {
                return found;
            }
            node = node->next;
        }
    }

    return nullptr;
}

void Binomial_Queue::combine(Binomial_Queue *other)
{
    if (other != nullptr && other->mRoot != nullptr)
        mRoot = merge(mRoot, other->mRoot);
}

void Binomial_Queue::insert(int key, int vertex)
{
    if (contains(key)) {
        throw "alreay exists";
        return;
    }
    BiNode* node = new BiNode(key, vertex);
    mRoot = unionHeaps(mRoot, node);
}

void Binomial_Queue::removeMin()
{
    mRoot = extractMinimum(mRoot);
}

bool Binomial_Queue::contains(int key)
{
    return search(mRoot, key) != nullptr ? true : false;
}

void Binomial_Queue::update(int vertex, int newkey)
{
    BiNode* node;
    node = search(mRoot, vertex);
    if(node != nullptr)
        updateKey(node, newkey);
}
