#ifndef __BIQ_H
#define __BIQ_H

#include "default.h"

using namespace std;

class BiNode
{
public:
    int key; // 关键字(键值)
    int vertex; // 顶点
    int degree; // 度数
    BiNode* child; // 左孩子
    BiNode* parent; // 父节点
    BiNode* next; // 兄弟节点

    BiNode():key(-1), vertex(-1), degree(0){}
    BiNode(int key, int vertex):key(key), vertex(vertex), degree(0), child(nullptr), parent(nullptr), next(nullptr){};
};

class Binomial_Queue
{
    private:
    BiNode* mRoot;

    void link(BiNode* child, BiNode* root);
    BiNode* merge(BiNode* h1, BiNode* h2);
    BiNode* unionHeaps(BiNode* h1, BiNode* h2);
    BiNode* reverse(BiNode* root);
    BiNode* extractMinimum(BiNode* root);
    void decreaseKey(BiNode* node, int key);
    void increaseKey(BiNode* node, int key);
    void updateKey(BiNode* node, int key);
    BiNode* search(BiNode* root, int key);

public:
    Binomial_Queue():mRoot(nullptr){}
    ~Binomial_Queue(){};

    bool isEmpty();
    bool minimum(int *pkey);
    void combine(Binomial_Queue* other);
    void insert(int key, int vertex);
    void removeMin();
    bool contains(int key);
    void update(int vertex, int newkey);
};

#endif