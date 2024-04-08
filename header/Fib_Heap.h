#ifndef _FIBONACCI_TREE_HPP_
#define _FIBONACCI_TREE_HPP_

#include "default.h"

using namespace std;

class FibNode {
    public:
        int key;                // 关键字(键值)
        int vertex;             // 节点信息
        int degree;            // 度数
        FibNode *left;    // 左兄弟
        FibNode *right;    // 右兄弟
        FibNode *child;    // 第一个孩子节点
        FibNode *parent;    // 父节点
        bool marked;        // 是否被删除第一个孩子

        FibNode(int value, int vertex):key(value), vertex(vertex), degree(0), marked(false),
            left(NULL),right(NULL),child(NULL),parent(NULL) 
        {
            left = this;
            right = this;
        }
};

class FibHeap{
    private:
        int keyNum;         // 堆中节点的总数
        int maxDegree;      // 最大度
        FibNode *min;    // 最小节点(某个最小堆的根节点)
        FibNode **cons;    // 最大度的内存区域

    public:
        FibHeap();
        ~FibHeap();

        bool isEmpty();
        void insert(int key, int vertex);
        void removeMin();
        void combine(FibHeap *other);
        bool minimum(int *pkey);
        void update(int vertex, int newkey);
        void remove(int key);
        bool contains(int vertex);
        void destroy();

    private:
        void removeNode(FibNode *node);
        void addNode(FibNode *node, FibNode *root);
        void catList(FibNode *a, FibNode *b);
        void insert(FibNode *node);
        FibNode* extractMin();
        void link(FibNode* node, FibNode* root);
        void makeCons();
        void consolidate();
        void renewDegree(FibNode *parent, int degree);
        void cut(FibNode *node, FibNode *parent);
        void cascadingCut(FibNode *node) ;
        void decrease(FibNode *node, int key);
        void increase(FibNode *node, int key);
        void update(FibNode *node, int key);
        FibNode* search(FibNode *root, int vertex);
        FibNode* search(int vertex);
        void remove(FibNode *node);
        void destroyNode(FibNode *node);
};

#endif