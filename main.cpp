/*
6_1. Обход дерева в порядке pre-order
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root. Выведите элементы в порядке pre-order (сверху вниз).

Рекурсия запрещена.

*/

#include <iostream>
#include <stack>
#include <functional>

class Tree {
public:
// Узел бинарного дерева
    struct treeNode {
        explicit treeNode(int value) : value_(value) {}
    
        int value_ = 0;
        treeNode* left = nullptr;
        treeNode* right = nullptr;
    };
    
    using function = std::function<void(treeNode*)>;

    Tree() : root(nullptr) {};
    ~Tree();
    Tree(const Tree&) = delete;
    Tree(Tree&&) = delete;
    Tree& operator=(const Tree&) = delete;
    Tree& operator=(Tree&&) = delete;
    void PreOrder(const function& func) const;
    void Add(int value);

private:
    treeNode* root = nullptr;
    
};

Tree::~Tree() {
    PreOrder([](treeNode* node) {
        delete node;
    });
}

void Tree::Add(int value) {
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

void Tree::PreOrder(const function& func) const {
    if (!root) {
        return;
    }
    
    std::stack<treeNode*> nodes;
    nodes.push(root);
    
    while (!nodes.empty()) {
        treeNode* node = nodes.top();
        nodes.pop();
        
        if (node->right) {
            nodes.push(node->right);
        }
        if (node->left) {
            nodes.push(node->left);
        }
        
        func(node);
    }
}

int main() {
    int nodes_count = 0;
    std::cin >> nodes_count;
    
    Tree tree;
    int node = 0;
    for (int i = 0; i < nodes_count; i++) {
        std::cin >> node;
        tree.Add(node);
    }    
    
    tree.PreOrder([](Tree::treeNode* node) {
        std::cout << node->value_ << " ";
    });
    
    return 0;
}
