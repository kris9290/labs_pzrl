#include "RBT.h"
#include <iostream>
#include <limits>

BinarySearchTree::Node::Node(Key key, Value value, bool color, Node* parent, Node* left, Node* right) 
    : keyValuePair(key, value), color(color), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node& other) : keyValuePair(other.keyValuePair) {}

bool BinarySearchTree::Node::operator==(const Node& other) const {
    return this->keyValuePair == other.keyValuePair;
}

//Копирование и освобождение поддеревьев
BinarySearchTree::Node* BinarySearchTree::copySubtree(const Node* other) {
    if (other == nullptr) return nullptr;
    Node* newNode = new Node(*other);
    newNode->left = copySubtree(other->left);
    newNode->right = copySubtree(other->right);
    return newNode;
}

void BinarySearchTree::freeSubtree(Node* node) {
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}

//Конструкторы и деструктор дерева
BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) 
    : _root(copySubtree(other._root)), _size(other._size) {}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this == &other) return *this;
    freeSubtree(_root);
    _root = copySubtree(other._root);
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept 
    : _root(other._root), _size(other._size) {
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this == &other) return *this;
    std::swap(this->_root, other._root);
    std::swap(this->_size, other._size);
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    freeSubtree(_root);
}

//Вспомогательные методы узла
void BinarySearchTree::Node::rotateLeft() {
    Node* newParent = this->right;

    newParent->parent = this->parent;
    if (this->parent) {
        if (this->parent->left == this) {
            this->parent->left = newParent;
        } else {
            this->parent->right = newParent;
        }
    }

    this->right = newParent->left;
    if (newParent->left) newParent->left->parent = this;

    this->parent = newParent;
    newParent->left = this;
}

void BinarySearchTree::Node::rotateRight() {
    Node* newParent = this->left;

    newParent->parent = this->parent;
    if (this->parent) {
        if (this->parent->left == this) {
            this->parent->left = newParent;
        } else {
            this->parent->right = newParent;
        }
    }

    this->left = newParent->right;
    if (newParent->right) newParent->right->parent = this;

    this->parent = newParent;
    newParent->right = this;
}

void BinarySearchTree::Node::insertRebalance(Node** rootPtr) {
    if (!parent) {
        color = 0;
        *rootPtr = this;
        return;
    }

    if (parent->color == false) 
        return;

    Node* aunt = (parent->parent->left == parent) ? parent->parent->right : parent->parent->left;
    if (aunt && aunt->color) {
        parent->color = false;
        aunt->color = false;
        parent->parent->color = true;
        parent->parent->insertRebalance(rootPtr);
        return;
    } 

    Node* nodePtr = this;
    if (nodePtr == parent->right && parent == parent->parent->left) {
        parent->rotateLeft();
        nodePtr = left;
    } else if (nodePtr == parent->left && parent == parent->parent->right) {
        parent->rotateRight();
        nodePtr = right;
    }

    nodePtr->parent->color = false;
    nodePtr->parent->parent->color = true;
    if ((nodePtr == nodePtr->parent->left) && (nodePtr->parent == nodePtr->parent->parent->left)) {
        if (nodePtr->parent->parent == *rootPtr) *rootPtr = nodePtr->parent->parent->left;
        nodePtr->parent->parent->rotateRight();
    } else {
        if (nodePtr->parent->parent == *rootPtr) *rootPtr = nodePtr->parent->parent->right;
        nodePtr->parent->parent->rotateLeft();
    }
}

void BinarySearchTree::Node::eraseRebalance(Node** rootPtr) {
    if (!parent) return;

    Node* brother;
    if (parent->left == this) brother = parent->right;
    else brother = parent->left;
    
    if (brother->color) {
        parent->color = 1;
        brother->color = 0;
        if (parent->left == this) {
            if (parent == *rootPtr) *rootPtr = parent->right;
            parent->rotateLeft();
        } else {
            if (parent == *rootPtr) *rootPtr = parent->left;
            parent->rotateRight();
        }
    }

    if (!parent->color && !brother->color 
            && (!brother->left || !brother->left->color)
            && (!brother->right || !brother->right->color)) {
        brother->color = 1;
        parent->eraseRebalance(rootPtr);
        return;
    }
    
    if (parent->color && !brother->color 
            && (!brother->left || !brother->left->color)
            && (!brother->right || !brother->right->color)) {
        brother->color = 1;
        parent->color = 0;
        return;
    }

    if (!brother->color) {
        if (this == parent->left 
                && (!brother->right || !brother->right->color) 
                && (brother->left && brother->left->color)) {
            brother->color = 1;
            brother->left->color = 0;
            brother->rotateRight();
        } else if (this == parent->right
                && (brother->right && brother->right->color)
                && (!brother->left || !brother->left->color)) {
            brother->color = 1;
            brother->right->color = 0;
            brother->rotateLeft();
        }
        return;
    }

    brother->color = parent->color;
    parent->color = 0;

    if (this == parent->left) {
        if (brother->right) brother->right->color = 0;
        if (parent == *rootPtr) *rootPtr = parent->right;
        parent->rotateLeft();
    } else {
        if (brother->left) brother->left->color = 0;
        if (parent == *rootPtr) *rootPtr = parent->left;
        parent->rotateRight();
    }
}

size_t BinarySearchTree::Node::getMaxHeight() const {
    if (this->keyValuePair.first == std::numeric_limits<Key>::max()) return 0;
    size_t leftHeight = 0, rightHeight = 0;
    if (left) leftHeight = left->getMaxHeight();
    if (right) rightHeight = right->getMaxHeight();
    return 1 + std::max(leftHeight, rightHeight);
}

void BinarySearchTree::Node::output_node_tree(int level) const {
    if (left) left->output_node_tree(level+1);
    
    if (this->keyValuePair.first == std::numeric_limits<Key>::max()) return;
    std::cout << "(" << this->keyValuePair.first << "," << this->keyValuePair.second << ")" << std::endl;
    
    if (right) right->output_node_tree(level+1);
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value, Node** rootPtr) {
    if (!rootPtr) return;
    if (key <= keyValuePair.first) {
        if (left) {
            left->insert(key, value, rootPtr);
            return;
        }
        left = new Node(key, value, true, this);
        left->insertRebalance(rootPtr);
    } else {
        if (right) {
            right->insert(key, value, rootPtr);
            return;
        }
        right = new Node(key, value, true, this);
        right->insertRebalance(rootPtr);
    }
}

void BinarySearchTree::Node::erase(const Key& key, Node** rootPtr) {
    if (!rootPtr) return;
    if (key != keyValuePair.first) {
        if (left && key <= keyValuePair.first) left->erase(key, rootPtr);
        else if (right) right->erase(key, rootPtr);
        return;
    }
    
    if (left && right) {
        Node* walker = right;
        while (walker->left) walker = walker->left;
        
        this->keyValuePair = walker->keyValuePair;
        walker->erase(walker->keyValuePair.first, rootPtr);
        return;
    }

    if (color) {
        if (parent->left == this) parent->left = nullptr;
        else parent->right = nullptr;
        delete this;
        return;
    }

    Node* childPtr = nullptr;
    if (left) childPtr = left;
    else childPtr = right;
    
    if (!childPtr) {
        this->color = 0;
        this->eraseRebalance(rootPtr);
        if (parent) {
            if (parent->left == this) parent->left = childPtr;
            else parent->right = childPtr;
        }
        if (this == *rootPtr) *rootPtr = childPtr;
        delete this;
        return;
    }

    if (parent) {
        if (parent->left == this) parent->left = childPtr;
        else parent->right = childPtr;
    }
    childPtr->parent = parent;

    if (childPtr->color) {
        childPtr->color = false;
    } 
    else childPtr->eraseRebalance(rootPtr);
    
    if (this == *rootPtr) *rootPtr = childPtr;
    delete this;
    return;
}

//Публичные методы дерева
void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (!_root) {
        _root = new Node(key, value);
        _root->right = new Node(std::numeric_limits<Key>::max(), value, false, _root);
        ++_size;
        return;
    }
    Node* endNode = end()._node;
    if (endNode->parent) endNode->parent->right = nullptr;

    _root->insert(key, value, &_root);
    ++_size;

    Node* walker = _root;
    while (walker->right) walker = walker->right;
    walker->right = endNode;
    endNode->parent = walker;
}

void BinarySearchTree::erase(const Key &key) {
    if (!_root) return;
    Iterator it(_root);
    while ((it = find(key)) != end()) {
        Node* endNode = end()._node;
        endNode->parent->right = nullptr;
        
        it._node->erase(key, &_root);
        --_size;
        if (!_root) {
            delete endNode;
            return;
        }

        Node* walker = _root;
        while (walker->right) walker = walker->right;
        walker->right = endNode;
        endNode->parent = walker;
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    const Node* walker = _root;
    while (walker) {
        if (walker->keyValuePair.first < key) {
            walker = walker->right;
        } else if (walker->keyValuePair.first > key) {
            walker = walker->left;
        } else {
            return ConstIterator(walker);
        }
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node* walker = _root;
    while (walker) {
        if (walker->keyValuePair.first < key) {
            walker = walker->right;
        } else if (walker->keyValuePair.first > key) {
            walker = walker->left;
        } else {
            return Iterator(walker);
        }
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> 
BinarySearchTree::equalRange(const Key &key) {
    Iterator endIt = end();
    Iterator current = begin();
    while (current != endIt && current->first != key) ++current;

    Iterator first = current;
    while (current != endIt && current->first == key) ++current;
    return {first, current};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> 
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator endIt = cend();
    ConstIterator current = cbegin();
    while (current != endIt && current->first != key) ++current;

    ConstIterator first = current;
    while (current != endIt && current->first == key) ++current;
    return {first, current};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    return cbegin();
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    auto range = equalRange(key);
    ConstIterator current = range.first;
    ConstIterator best = current;
    
    for (; current != range.second; ++current) {
        if (current->second < best->second) best = current;
    }
    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    auto range = equalRange(key);
    ConstIterator current = range.first;
    ConstIterator best = current;
    
    for (; current != range.second; ++current) {
        if (current->second > best->second) best = current;
    }
    return best;
}BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node* walker = _root;
    while (walker->left) walker = walker->left;
    return Iterator(walker);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Node* walker = _root;
    while (walker->right) walker = walker->right;
    return Iterator(walker);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node* walker = _root;
    while (walker->left) walker = walker->left;
    return ConstIterator(walker);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node* walker = _root;
    while (walker->right) walker = walker->right;
    return ConstIterator(walker);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() const {
    if (!_root) return;
    _root->output_node_tree();
}

size_t BinarySearchTree::max_height() const {
    return _root->getMaxHeight();
}

//Итераторы
BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (!_node) throw std::runtime_error("Null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (!_node) throw std::runtime_error("Null iterator");
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    if (!_node) throw std::runtime_error("Null iterator");
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    if (!_node) throw std::runtime_error("Null iterator");
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) throw std::runtime_error("Null iterator");
    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("++end");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    if (!_node) throw std::runtime_error("Null iterator");
    Iterator it = *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return it;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return it;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("end++");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (!_node) throw std::runtime_error("Null iterator");
    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("--begin");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    if (!_node) throw std::runtime_error("Null iterator");
    Iterator it = *this;

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return it;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return it;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("begin--");
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node->keyValuePair.first == other._node->keyValuePair.first;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return _node->keyValuePair.first != other._node->keyValuePair.first;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (!_node) throw std::runtime_error("Null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    if (!_node) throw std::runtime_error("Null iterator");
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (!_node) throw std::runtime_error("Null iterator");
    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("++end");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    if (!_node) throw std::runtime_error("Null iterator");
    ConstIterator it = *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return it;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return it;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("end++");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node) throw std::runtime_error("Null iterator");
    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("--begin");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    if (!_node) throw std::runtime_error("Null iterator");
    ConstIterator it = *this;

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return it;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return it;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("begin--");
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node->keyValuePair.first == other._node->keyValuePair.first;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node->keyValuePair.first != other._node->keyValuePair.first;
}
