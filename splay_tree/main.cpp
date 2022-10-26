#include <iostream>
#include <list>
#include <algorithm>
#include <tuple>

template<class T>
class Node;

template<class T>
using node = Node<T>*;
using key_type = long long;

template<class T>
class Node {
public:
    Node(const node<T> father, const T& value, const key_type key, const bool is_left_child = true) :_father(father), _value(value), _key(key), _is_left_child(is_left_child) {}
    bool hasLeftChild() const { return _left_child != nullptr; }
    auto leftChild() { return _left_child; }
    bool hasRightChild() const { return _right_child != nullptr; }
    auto rightChild() { return _right_child; }
    bool isRoot() { return _father == nullptr; }
    auto getFather() { return _father; }
    key_type getKey() const { return _key; }
    const T& getValue() const { return _value; }
    void setLeftChild(node<T> lc) { _left_child = lc; }
    void setRightChild(node<T> rc) { _right_child = rc; }
    void setFather(node<T> f) { _father = f; }
    bool isLeftChild() const { return _is_left_child; }
    void changeIsLeft() { _is_left_child = !_is_left_child; }
    void setValue(const T& val) { _value = val; }

private:
    node<T> _father;
    node<T> _left_child = nullptr;
    node<T> _right_child = nullptr;
    T _value;
    key_type _key;
    bool _is_left_child;
};

template<class T>
class splay_tree {

public:
    splay_tree() = default;
    ~splay_tree() {
        if (!empty()) {
            std::list<node<T>> second_level;
            second_level.push_back(_root->leftChild());
            second_level.push_back(_root->rightChild());
            delete _root;
            clear(second_level);
        }
    }

    void add(const key_type key, const T& value) {
        if (empty()) {
            _root = new Node<T>( nullptr, value, key );
        } else {
            auto new_node = insert(_root, 1, key, value);
            if (new_node.first) {
                splay(new_node.first, new_node.second);
            }
        }
    }
    void print() const {
        if (empty()) { std::cout << "_\n"; }
        else {
            std::list<node<T>> second_level;
            std::cout << '[' << _root->getKey() << ' ' << _root->getValue() << "]\n";
            second_level.push_back(_root->leftChild());
            second_level.push_back(_root->rightChild());
            print_level(second_level);
        }
    }
    void set(const key_type key, const T& value) {
        if (empty()) {
            throw std::logic_error("error\n");
        } else {
                auto res = find(_root, 1, key);
                if (!std::get<2>(res)) {
                    splay(std::get<0>(res), std::get<1>(res));
                    return;
                }
                std::get<0>(res)->setValue(value);
                splay(std::get<0>(res), std::get<1>(res));
        }
    }
    const T& search(const key_type key) {
        if (empty()) {
            throw std::logic_error("error\n");
        } else {
            auto res = find(_root, 1, key, false);
            if (!std::get<2>(res)) {
                splay(std::get<0>(res), std::get<1>(res));
                throw std::logic_error("error\n");
            }
            splay(std::get<0>(res), std::get<1>(res));
            return std::get<0>(res)->getValue();
        }
    }
    void del(const key_type key) {
        if (empty()) {
            throw std::logic_error("error\n");
        } else {
            try {
                auto res = find(_root, 1, key);
                if (!std::get<2>(res)) {
                    splay(std::get<0>(res), std::get<1>(res));
                    return;
                }
                if (std::get<0>(res) == _root) {
                    auto tmp = _root;
                    _root = merge(_root->leftChild(), _root->rightChild());
                    delete tmp;
                    return;
                }
                print();
                splay(std::get<0>(res), std::get<1>(res));
                auto l = std::get<0>(res)->leftChild();
                auto r = std::get<0>(res)->rightChild();

                auto tmp = _root;
                _root = merge(l, r);
                print();
                delete tmp;
            } catch (const std::logic_error& e) {
                throw e;
            }
        }
    }
    std::pair<key_type, const T> min() {
        if (empty())
            throw std::logic_error("error\n");
        auto tmp = find_min(_root, 1);
        splay(tmp.first, tmp.second);
        return std::make_pair(tmp.first->getKey(), tmp.first->getValue());
    }
    std::pair<key_type, const T> max() {
        if (empty())
            throw std::logic_error("error\n");
        auto tmp = find_max(_root, 1);
        splay(tmp.first, tmp.second);
        return std::make_pair(tmp.first->getKey(), tmp.first->getValue());
    }

private:
    bool empty() const { return !_root; }
    void clear(const std::list<node<T>> list) {
        std::list<node<T>> new_level;
        for (auto &i: list) {
            if (i != nullptr) {
                if (i->hasLeftChild()) { new_level.push_back(i->leftChild()); }
                if (i->hasRightChild()) { new_level.push_back(i->rightChild()); }
                delete i;
            }
        }
        if (!new_level.empty()) {
            clear(new_level);
        }
    }
    void print_level(const std::list<node<T>> list) const {
        if (std::find_if(list.begin(), list.end(), [](const node<T>& n){ return n != nullptr; }) == list.end()) { return; }
        std::list<node<T>> new_level;
        auto i = list.begin();
        for (int s = 1; s < list.size(); ++s, ++i) {
            if (*i == nullptr) {
                std::cout << "_ ";
                new_level.push_back(nullptr);
                new_level.push_back(nullptr);
            } else {
                std::cout << '[' << (*i)->getKey() << ' ' << (*i)->getValue() << ' ' << (*i)->getFather()->getKey() << "] ";
                new_level.push_back((*i)->leftChild());
                new_level.push_back((*i)->rightChild());
            }
        }
        if (*i == nullptr) {
            std::cout << "_" << std::endl;
            new_level.push_back(nullptr);
            new_level.push_back(nullptr);
        } else {
            std::cout << '[' << (*i)->getKey() << ' ' << (*i)->getValue() << ' ' << (*i)->getFather()->getKey() << ']' << std::endl;
            new_level.push_back((*i)->leftChild());
            new_level.push_back((*i)->rightChild());
        }
        print_level(new_level);
    }
    std::pair<node<T>, size_t> insert(node<T> cur_node, size_t depth, const key_type key, const T& value) {
        if (cur_node->getKey() == key) {
            std::cout << "error" << std::endl;
            return std::make_pair(cur_node, depth); //TODO
        }
        if (key < cur_node->getKey()) {
            if (cur_node->hasLeftChild()) {
                return insert(cur_node->leftChild(), depth + 1, key, value);
            } else {
                auto new_node = new Node<T>(cur_node, value, key);
                cur_node->setLeftChild(new_node);
                new_node->setFather(cur_node);
                return std::make_pair(new_node, depth + 1);
            }
        } else {
            if (cur_node->hasRightChild()) {
                return insert(cur_node->rightChild(), depth + 1, key, value);
            } else {
                auto new_node = new Node<T>(cur_node, value, key, false);
                cur_node->setRightChild(new_node);
                new_node->setFather(cur_node);
                return std::make_pair(new_node, depth + 1);
            }
        }
    }
    void splay(node<T> n, size_t depth) {
        if (depth == 2) {
            n->isLeftChild() ? zig(n, false) : zig(n);
            _root = n;
        } else if (depth == 1) {
            _root = n;
        } else {
            if (n->isLeftChild()) {
                n->getFather()->isLeftChild() ? zig_zig(n) : zig_zag(n, false);
                splay(n, depth - 2);
            } else {
                n->getFather()->isLeftChild() ? zig_zag(n) : zig_zig(n, false);
                splay(n, depth - 2);
            }
        }
    }
    void zig (node<T> n, bool left = true) {
        auto father = n->getFather();
        if (!father->isRoot()) {
            auto grand_father = father->getFather();
            n->setFather(grand_father);
            father->isLeftChild() ? grand_father->setLeftChild(n) : grand_father->setRightChild(n);
        } else {
            n->setFather(nullptr);
        }
        if (left) { // левый разворот
            if (n->hasLeftChild()) {
                father->setRightChild(n->leftChild());
                father->rightChild()->changeIsLeft();
                father->rightChild()->setFather(father);
            } else {
                father->setRightChild(nullptr);
            }
            if (!father->isLeftChild()) {
                father->changeIsLeft();
            } else {
                n->changeIsLeft();
            }
            n->setLeftChild(father);
            father->setFather(n);
        } else { // правый разворот
            if (n->hasRightChild()) {
                father->setLeftChild(n->rightChild());
                father->leftChild()->changeIsLeft();
                father->leftChild()->setFather(father);
            } else {
                father->setLeftChild(nullptr);
            }
            if (father->isLeftChild()) {
                father->changeIsLeft();
            } else {
              n->changeIsLeft();
            }
            n->setRightChild(father);
            father->setFather(n);
        }
    }
    void zig_zig(node<T> n, bool left = true) {
        zig(n->getFather(), !left);
        zig(n, !left);
    }
    void zig_zag(node<T> n, bool left = true) {
        zig(n, left);
        zig(n, !left);
    }
    std::tuple<node<T>, size_t, bool> find(node<T> cur_node, size_t depth, const key_type key, bool print_enabled = true) {
        if (cur_node->getKey() == key) {
            return std::make_tuple(cur_node, depth, true);
        } else if (cur_node->getKey() > key && cur_node->hasLeftChild()) {
            if (print_enabled) {
                return find(cur_node->leftChild(), depth + 1, key);
            } else {
                return find(cur_node->leftChild(), depth + 1, key, false);
            }
        } else if (cur_node->getKey() < key && cur_node->hasRightChild()) {
            if (print_enabled) {
                return find(cur_node->rightChild(), depth + 1, key);
            } else {
                return find(cur_node->rightChild(), depth + 1, key, false);
            }
        } else {
            if (print_enabled)
                std::cout << "error" << std::endl;
            return std::make_tuple(cur_node, depth, false);
        }
    }
    std::pair<node<T>, size_t> find_min(node<T> cur_node, size_t depth) {
        if (!cur_node->hasLeftChild()) {
            return std::make_pair(cur_node, depth);
        } else {
            return find_min(cur_node->leftChild(), depth + 1);
        }
    }
    std::pair<node<T>, size_t> find_max(node<T> cur_node, size_t depth) {
        if (!cur_node->hasRightChild()) {
            return std::make_pair(cur_node, depth);
        } else {
            return find_max(cur_node->rightChild(), depth + 1);
        }
    }
    node<T> merge(node<T> left, node<T> right) {
        if (left) {
            auto left_max = find_max(left, 1).first;
            if (right) {
                right->setFather(left_max);
                left_max->setRightChild(right);
                if (left != left_max) {
                    if (left_max->isLeftChild()) {
                        left_max->getFather()->setLeftChild(nullptr);
                    } else {
                        left_max->getFather()->setRightChild(nullptr);
                        left_max->changeIsLeft();
                    }
                    left_max->setLeftChild(left);
                    left->setFather(left_max);
                }
                left_max->setFather(nullptr);
            } else {
               left->setFather(nullptr);
               return left;
            }
            return left_max;
        } else {
            if (right) {
                right->setFather(nullptr);
                right->changeIsLeft();
                return right;
            } else {
                return nullptr;
            }
        }
    }

    node<T> _root = nullptr;
};

int main() {
    splay_tree<std::string> st;
    std::string str;
    while (std::getline(std::cin, str)) {
        if (str.empty())
            continue;
        auto it = std::find_if(str.begin(), str.end(), isspace);
        std::string command = std::string(str.begin(), it);
        try {
            if (command == "print") {
                if (it != str.end()) { std::cout << "error\n"; continue; }
                st.print();
                continue;
            } else if (command == "min") {
                if (it != str.end()) { std::cout << "error\n"; continue; }
                auto tmp = st.min();
                std::cout << tmp.first << ' ' << tmp.second << std::endl;
                continue;
            } else if (command == "max") {
                if (it != str.end()) { std::cout << "error\n"; continue; }
                auto tmp = st.max();
                std::cout << tmp.first << ' ' << tmp.second << std::endl;
                continue;
            } else if (command == "add") {
                if (it == str.end()) { std::cout << "error\n"; continue; }
                auto second_it = std::find_if(it + 1, str.end(), isspace);
                if (second_it == str.end()) { std::cout << "error\n"; continue; }
                std::string key_str = std::string(it + 1, second_it);
                try {
                    key_type key = std::stoll(key_str);
                    auto third_it = std::find_if(second_it + 1, str.end(), isspace);
                    if (third_it != str.end()) { std::cout << "error\n"; continue; }
                    std::string value = std::string(second_it + 1, third_it);
                    st.add(key, value);
                } catch (...) { std::cout << "error\n"; continue; }
                continue;
            } else if (command == "set") {
                if (it == str.end()) { std::cout << "error\n"; continue; }
                auto second_it = std::find_if(it + 1, str.end(), isspace);
                if (second_it == str.end()) { std::cout << "error\n"; continue; }
                std::string key_str = std::string(it + 1, second_it);
                try {
                    key_type key = std::stoll(key_str);
                    auto third_it = std::find_if(second_it + 1, str.end(), isspace);
                    if (third_it != str.end()) { std::cout << "error\n"; continue; }
                    std::string value = std::string(second_it + 1, third_it);
                    st.set(key, value);
                } catch (...) { std::cout << "error\n"; continue; }
                continue;
            } else if (command == "search") {
                if (it == str.end()) { std::cout << "error\n"; continue; }
                auto second_it = std::find_if(it + 1, str.end(), isspace);
                if (second_it != str.end()) { std::cout << "error\n"; continue; }
                std::string key_str = std::string(it + 1, second_it);
                try {
                    key_type key = std::stoll(key_str);
                    auto tmp = st.search(key);
                    std::cout << 1 << ' ' << tmp << std::endl;
                } catch (const std::logic_error&) { std::cout << 0 << std::endl; }
                catch (...) { std::cout << "error\n"; continue; }
                continue;
            } else if (command == "delete") {
                if (it == str.end()) { std::cout << "error\n"; continue; }
                auto second_it = std::find_if(it + 1, str.end(), isspace);
                if (second_it != str.end()) { std::cout << "error\n"; continue; }
                std::string key_str = std::string(it + 1, second_it);
                try {
                    key_type key = std::stoll(key_str);
                    st.del(key);
                } catch (...) { std::cout << "error\n"; continue; }
            } else { std::cout << "error\n"; continue; }
            continue;
        } catch (const std::logic_error &e) {
            std::cout << e.what();
            continue;
        }
    }
}
