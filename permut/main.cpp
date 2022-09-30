#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>

class A {
public:
    A() {
        Foo();
    }
    virtual void Foo() {
        std::cout << "a";
    }
};

class B: public A {
public:
    void Foo() override {
        std::cout << "b";
    }
};

void permute(std::set<std::vector<long long>>& set, std::vector<long long>& vec, size_t n) {
    if (n < 2) {
        set.insert(vec);
    } else {
        for (int i = n - 1; i >= 0; --i) {
            std::swap(vec[i], vec[n-1]);
            permute(set, vec, n - 1);
            std::swap(vec[i], vec[n-1]);
        }
    }
}

int main() {
    B b;
}
