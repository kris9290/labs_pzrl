#pragma once
#include <cstddef>

template<typename Key, typename Value>
class SplayTree {
public:
    SplayTree() : root(nullptr), treeSize(0) {}
    
    ~SplayTree() {
        clear(root);
    }
    
    void insert(const Key& key, const Value& value) {
        if (!root) {
            root = new Node(key, value);
            treeSize++;
            return;
        }
        
        Node* current = root;
        Node* parent = nullptr;
        
        while (current) {
            parent = current;
            if (key < current->key)
                current = current->left;
            else if (current->key < key)
                current = current->right;
            else {
                current->value = value;
                splay(current);
                return;
            }
        }
        
        Node* newNode = new Node(key, value);
        newNode->parent = parent;
        
        if (key < parent->key) 
            parent->left = newNode;
        else 
            parent->right = newNode;
        
        treeSize++;
        splay(newNode);
    }
    
    bool remove(const Key& key) {
        Node* node = findNode(key);
        if (!node)
            return false;
        
        splay(node);
        
        Node* leftSubtree = node->left;
        Node* rightSubtree = node->right;
        
        if (leftSubtree) 
            leftSubtree->parent = nullptr;
        if (rightSubtree) 
            rightSubtree->parent = nullptr;
        
        delete node;
        treeSize--;
        
        if (!leftSubtree) {
            root = rightSubtree;
        } 
        else {
            root = leftSubtree;
            Node* maxNode = leftSubtree;
            while (maxNode->right) 
                maxNode = maxNode->right;
            splay(maxNode);
            root->right = rightSubtree;
            if (rightSubtree) 
                rightSubtree->parent = root;
        }
        
        return true;
    }
    
    Value* search(const Key& key) {
        Node* node = findNode(key);
        return node ? &node->value : nullptr;
    }
    
    const Value* search(const Key& key) const {
        Node* node = findNodeConst(key);
        return node ? &node->value : nullptr;
    }
    
    bool isValidBST() const {
        return validateBST(root, nullptr, nullptr);
    }
    
    size_t size() const
        { return treeSize; }
    bool empty() const
        { return treeSize == 0; }
    
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    
    Node* root;
    size_t treeSize;
    
    void rotateRight(Node* node) {
        Node* leftChild = node->left;
        if (!leftChild) 
            return;
        
        node->left = leftChild->right;
        if (leftChild->right) 
            leftChild->right->parent = node;
        
        leftChild->parent = node->parent;
        if (!node->parent) 
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else 
            node->parent->right = leftChild;
        
        leftChild->right = node;
        node->parent = leftChild;
    }
    
    void rotateLeft(Node* node) {
        Node* rightChild = node->right;
        if (!rightChild)
            return;
        
        node->right = rightChild->left;
        if (rightChild->left)
            rightChild->left->parent = node;
        
        rightChild->parent = node->parent;
        if (!node->parent) 
            root = rightChild;
        else if (node == node->parent->left) 
            node->parent->left = rightChild;
        else 
            node->parent->right = rightChild;
        
        rightChild->left = node;
        node->parent = rightChild;
    }
    
    void splay(Node* node) {
        while (node && node->parent) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;
            
            if (!grandparent) {
                if (node == parent->left)
                 rotateRight(parent);
                else 
                 rotateLeft(parent);
            } else if (node == parent->left && parent == grandparent->left) {
                rotateRight(grandparent);
                rotateRight(parent);
            } else if (node == parent->right && parent == grandparent->right) {
                rotateLeft(grandparent);
                rotateLeft(parent);
            } else if (node == parent->left && parent == grandparent->right) {
                rotateRight(parent);
                rotateLeft(grandparent);
            } else {
                rotateLeft(parent);
                rotateRight(grandparent);
            }
        }
    }
    
    Node* findNode(const Key& key) {
        Node* current = root;
        Node* last = nullptr;
        
        while (current) {
            last = current;
            if (key < current->key) 
                current = current->left;
            else if (current->key < key) 
                current = current->right;
            else {
                splay(current);
                return current;
            }
        }
        
        if (last) 
            splay(last);
        return nullptr;
    }
    
    Node* findNodeConst(const Key& key) const {
        Node* current = root;
        while (current) {
            if (key < current->key) 
                current = current->left;
            else if (current->key < key) 
                current = current->right;
            else return current;
        }
        return nullptr;
    }
    
    void clear(Node* node) {
        if (!node) 
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
    bool validateBST(const Node* node, const Key* minKey, const Key* maxKey) const {
        if (!node) 
            return true;
        if (minKey && node->key <= *minKey) 
            return false;
        if (maxKey && node->key >= *maxKey)
            return false;
        return validateBST(node->left, minKey, &node->key) && validateBST(node->right, &node->key, maxKey);
    }
};
