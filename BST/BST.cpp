#include "BST.h"
#include <iostream>
#include <limits>

BinarySearchTree::Node::Node(Key key, Value value, Node* parent, Node* left, Node* right) 
    : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node& other) : keyValuePair(other.keyValuePair) {}

bool BinarySearchTree::Node::operator==(const Node& other) const {
    return this->keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left)
        left->output_node_tree();
    
    if (this->keyValuePair.first == std::numeric_limits<Key>::max())
        return;
    std::cout << this->keyValuePair.first << " " << this->keyValuePair.second << std::endl;
    
    if (right) 
        right->output_node_tree();
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value) {
    if (key < keyValuePair.first) {
        if (left) {
            left->insert(key, value);
            return;
        }
        this->left = new Node(key, value, this);
    } else {
        if (right) {
            right->insert(key, value);
            return;
        }
        this->right = new Node(key, value, this);
    }
}

void BinarySearchTree::Node::erase(const Key& key) {
    if (key != keyValuePair.first) {
        if (left && key < keyValuePair.first)
            left->erase(key);
        else if (right)
            right->erase(key);
        return;
    }
    
    if (left && right) {
        Node* minNode = right;
        while (minNode->left)
            minNode = minNode->left;
        
        this->keyValuePair = minNode->keyValuePair;
        minNode->erase(minNode->keyValuePair.first);
        return;
    }

    Node* child = nullptr;
    if (left)
        child = left;
    else
        child = right;

    if (parent) {
        if (parent->left == this) parent->left = child;
        else parent->right = child;
    }
    if (child) child->parent = parent;

    delete this;
}

BinarySearchTree::Node* BinarySearchTree::copySubtree(const Node* other) {
    if (other == nullptr) 
        return nullptr;

    Node* newNode = new Node(*other);
    newNode->left = copySubtree(other->left);
    if (newNode->left)
        newNode->left->parent = newNode;
    newNode->right = copySubtree(other->right);
    if (newNode->right) 
        newNode->right->parent = newNode;
    return newNode;
}

void BinarySearchTree::freeSubtree(Node* node) {
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}


BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) 
    : _root(copySubtree(other._root)), _size(other._size) {}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this == &other) 
        return *this;

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
    if (this == &other) 
        return *this;
    std::swap(this->_root, other._root);
    std::swap(this->_size, other._size);
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    freeSubtree(_root);
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (!_root) {
        _root = new Node(key, value);
        _root->right = new Node(std::numeric_limits<Key>::max(), value, _root);
        ++_size;
        return;
    }

    _root->insert(key, value);
    ++_size;
}

void BinarySearchTree::erase(const Key &key) {
    if (!_root)
        return;
    Iterator iter(_root);
    while ((iter = find(key)) != end()) {
        iter._node->erase(key);
        --_size;
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    const Node* cur = _root;
    while (cur) {
        if (cur->keyValuePair.first < key) {
            cur = cur->right;
        } else if (cur->keyValuePair.first > key) {
            cur = cur->left;
        } else {
            return ConstIterator(cur);
        }
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node* cur = _root;
    while (cur) {
        if (cur->keyValuePair.first < key) {
            cur = cur->right;
        } else if (cur->keyValuePair.first > key) {
            cur = cur->left;
        } else {
            return Iterator(cur);
        }
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> 
BinarySearchTree::equalRange(const Key &key) {
    Iterator endIt = end();
    Iterator cur = begin();
    while (cur != endIt && cur->first != key) 
        ++cur;

    Iterator first = cur;
    while (cur != endIt && cur->first == key)
        ++cur;
    return {first, cur};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> 
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator endIt = cend();
    ConstIterator cur = cbegin();
    while (cur != endIt && cur->first != key) 
        ++cur;

    ConstIterator first = cur;
    while (cur != endIt && cur->first == key) 
        ++cur;
    return {first, cur};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    return cbegin();
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    auto range = equalRange(key);
    ConstIterator cur = range.first;
    ConstIterator best = cur;
    
    for (; cur != range.second; ++cur) {
        if (cur->second < best->second) best = cur;
    }
    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    auto range = equalRange(key);
    ConstIterator cur = range.first;
    ConstIterator best = cur;
    
    for (; cur != range.second; ++cur) {
        if (cur->second > best->second) best = cur;
    }
    return best;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node* cur = _root;
    while (cur->left) cur = cur->left;
    return Iterator(cur);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Node* cur = _root;
    while (cur->right) cur = cur->right;
    return Iterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node* cur = _root;
    while (cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node* cur = _root;
    while (cur->right) cur = cur->right;
    return ConstIterator(cur);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() const {
    if (!_root) return;
    _root->output_node_tree();
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (!_node) 
        throw std::runtime_error("нулевой итератор");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    if (_node->right) {
        _node = _node->right;
        while (_node->left)
            _node = _node->left;
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
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    Iterator tmp = *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left)
            _node = _node->left;
        return tmp;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return tmp;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("end++");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    if (_node->left) {
        _node = _node->left;
        while (_node->right) 
            _node = _node->right;
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
    if (!_node) 
        throw std::runtime_error("нулевой итератор");

    Iterator tmp = *this;
    if (_node->left) {
        _node = _node->left;
        while (_node->right)
            _node = _node->right;
        return tmp;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return tmp;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("begin--");
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    if (!_node) 
        throw std::runtime_error("нулевой итератор");
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (!_node)
        throw std::runtime_error("нулевой итератор");
    if (_node->right) {
        _node = _node->right;
        while (_node->left) 
            _node = _node->left;
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
    if (!_node) 
        throw std::runtime_error("нулевой итератор");
    ConstIterator tmp = *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) 
            _node = _node->left;
        return tmp;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return tmp;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("end++");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (!_node) 
        throw std::runtime_error("нулевой итератор");
    if (_node->left) {
        _node = _node->left;
        while (_node->right) 
            _node = _node->right;
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
    if (!_node) 
        throw std::runtime_error("нулевой итератор");

    ConstIterator tmp = *this;
    if (_node->left) {
        _node = _node->left;
        while (_node->right)
            _node = _node->right;
        return tmp;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return tmp;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("begin--");
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node != other._node;
}
