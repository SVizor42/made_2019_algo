/*
7_1. Солдаты ДД

В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.

Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится. Требуемая скорость выполнения команды - O(log n) в среднем.

В реализации используйте декартовы деревья.
*/

#include <random>
#include <iostream>

// Узел декартова дерева.
struct TreapNode {
    explicit TreapNode(int _key, int _priority) :
        key(_key), priority(_priority) {}

    int key = 0;
    int priority = 0;
    int size = 1;
    TreapNode* left = nullptr;
    TreapNode* right = nullptr;
};

int get_size(TreapNode* node) {
    return (node ? node->size : 0);
}

void update_size(TreapNode* node) {
    if (node)
        node->size = get_size(node->left) + get_size(node->right) + 1;
}

void split(TreapNode* node, int key, TreapNode*& left, TreapNode*& right) {
    if (!node)
        left = right = nullptr;
    
    else if (node->key >= key) {
        split(node->right, key, node->right, right);
        left = node;
        update_size(left);
    } 
    else {
        split(node->left, key, left, node->left);
        right = node;
        update_size(right);
    }
}

TreapNode* merge(TreapNode* left, TreapNode* right) {
    if (!left || !right)
        return (left == 0 ? right : left);
    
    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        update_size(left);
        return left;
    }
    else {
        right->left = merge(left, right->left);
        update_size(right);
        return right;
    }
}

class Treap {
public:
    int insert(int key);
    void erase(int value);
    ~Treap();
private:
    TreapNode* root = nullptr;

    void delete_subtree(TreapNode* node);
};

int Treap::insert(int key) {
    
    int priority = rand();
    if (!root) {
        root = new TreapNode(key, priority);
        return 0;
    }
    
    TreapNode* node = root;
    TreapNode** parent = &root;
    int index = 0;
    if (node->left)
        index = get_size(node->left);
    
    while (node) {
        if (node->priority <= priority)
            break;
        
        node->size++;
        if (node->key >= key) {
            parent = &node->right;
            node = *parent;
            index++;
            if (node && node->left)
                index += get_size(node->left);
        }
        else {
            parent = &node->left;
            node = *parent;
            if (node) index--;
            if (node && node->right)
                index -= get_size(node->right);
        }
    }
    
    if (node && node->left)
        index -= get_size(node->left);
    
    *parent = new TreapNode(key, priority);
    split(node, key, (*parent)->left, (*parent)->right);
    update_size(*parent);
    
    if ((*parent)->left)
        index += get_size((*parent)->left);
        
    return index;
}

void Treap::erase(int value) {
    TreapNode* node = root;
    TreapNode** parent = &root;
    int index = 0;
    if (node->left)
        index = get_size(node->left);
    
    while (index - value) {
        node->size--;
        if (index < value) {
            parent = &node->right;
            node = *parent;
            index++;
            if (node && node->left)
                index += get_size(node->left);
        }
        else {
            parent = &node->left;
            node = *parent;
            if (node) index--;
            if (node && node->right)
                index -= get_size(node->right);
        }
    }

    *parent = merge(node->left, node->right);
    delete node;
}

Treap::~Treap() {
    delete_subtree(root);
}

void Treap::delete_subtree(TreapNode* node) {
    if (!node) 
        return;
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}

int main() {
    int commands_count = 0;
    std::cin >> commands_count;
    
    int command = 0, value = 0;
    Treap treap;
    for (int i = 0; i < commands_count; i++) {
        std::cin >> command >> value;
        if (command == 1) {
            std::cout << treap.insert(value) << std::endl;
        }
        else {
            treap.erase(value);
        }
    }    
    
    return 0;
}


