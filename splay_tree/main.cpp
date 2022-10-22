#include <iostream>
#include <memory>

template<class T>
class Node;

template<class T>
using node = std::shared_ptr<Node<T>>;

template<class T>
class Node {
public:
    bool hasLeftChild() { return _left_child; }
    auto leftChild() { return _left_child; }
    bool hasRightChild() { return _right_child; }
    auto rightChild() { return _right_child; }
    bool isRoot() { return !_father; }
    auto getFather() { return _father; }
    size_t getDepth() { return _depth; }
    int getKey() { return _key; }
    void setLeftChild(node<T> lc) { _left_child = lc; }
    void setRightChild(node<T> rc) { _right_child = rc; }
    void setFather(node<T> f) { _father = f; }
    void incrementDepth() { ++_depth; }
    void decrementDepth() { --_depth; }
    bool isLeftChild() { return _is_left_child; }
    void changeIsLeft() { _is_left_child = !_is_left_child; }

private:
    node<T> _father;
    node<T> _left_child;
    node<T> _right_child;
    node<T> _depth;
    T _value;
    int _key;
    bool _is_left_child = true;
};

template<class T>
class splay_tree {
    bool empty() { return !_root; }

private:
    void splay(int key) {


    }
    void zig (node<T> n, bool left = true) {
        auto father = n->getFather();
        if (left) { // левый разворот
            if (n->hasLeftChild()) {
                father->setRightChild(n->leftChild());
                father->rightChild()->changeIsLeft();
            } else {
                father->setRightChild(nullptr);
            }
            if (n->hasRightChild())
                n->rightChild()->decrementDepth();
            if (father->hasLeftChild())
                father->leftChild()->incrementDepth();
        } else { // правый разворот
            if (n->hasRightChild()) {
                father->setLeftChild(n->rightChild());
                father->leftChild()->changeIsLeft();
            } else {
                father->setLeftChild(nullptr);
            }
            father->changeIsLeft();
            if (n->hasLeftChild())
                n->leftChild()->decrementDepth();
            if (father->hasRightChild())
                father->rightChild()->incrementDepth();
        }
        father->incrementDepth();
        n->decrementDepth();
        _root = n;
    }

    std::shared_ptr<node<T>> find(int key) {
        if (_root) {
            auto cur = _root;
            while (true) {
                if (cur->getKey())
                    return cur;
                if (cur->getKey() < key) {
                    if (cur->hasRightChild()) { cur = cur->rightChild(); continue; }
                    else return nullptr;
                }
                if (cur->getKey() > key) {
                    if (cur->hasLeftChild()) { cur = cur->leftChild(); continue; }
                    else return nullptr;
                }
            }
        }
        else return nullptr;
    }
    node<T> _root;
    size_t _height;
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
