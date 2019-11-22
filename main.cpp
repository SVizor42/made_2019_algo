/*
6_1. Обход дерева в порядке pre-order
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root. Выведите элементы в порядке pre-order (сверху вниз).

Рекурсия запрещена.

*/

#include <iostream>
#include <stack>

// Узел бинарного дерева
struct treeNode {
    explicit treeNode(int value) : value_(value) {}
    ~treeNode();
    
    int value_ = 0;
    treeNode* left = nullptr;
    treeNode* right = nullptr;
};

class Tree {
public:
    ~Tree();
    void print() const;
    void add(int value);

private:
    treeNode* root = nullptr;
};

treeNode::~treeNode() {
    if (this->left) {
        delete this->left;
        this->left = nullptr;
    }
    if (this->right) {
        delete this->right;
        this->right = nullptr;
    }
}

Tree::~Tree() {
    if (!root) {
        return;
    }
    
    delete root;
}

void Tree::add(int value) {
    if (!root) {
        root = new treeNode(value);
        return;
    }
    
    treeNode* node = root;
    while (node) {
        if (node->value_ > value) {
            if (node->left) {
                node = node->left;
            }
            else {
                node->left = new treeNode(value);
                return;
            }
        }
        else {
            if (node->right) {
                node = node->right;
            }
            else {
                node->right = new treeNode(value);
                return;
            }
        }
    }
}

void Tree::print() const {
    if (!root) {
        return;
    }
    
    std::stack<treeNode*> nodes;
    nodes.push(root);
    
    while (!nodes.empty()) {
        treeNode* node = nodes.top();
        nodes.pop();
        std::cout << node->value_ << " ";
        
        if (node->right) {
            nodes.push(node->right);
        }
        if (node->left) {
            nodes.push(node->left);
        }
    }
}

int main() {
    int nodes_count = 0;
    std::cin >> nodes_count;
    
    Tree tree;
    int node = 0;
    for (int i = 0; i < nodes_count; i++) {
        std::cin >> node;
        tree.add(node);
    }    
    
    tree.print();
    
    return 0;
}

