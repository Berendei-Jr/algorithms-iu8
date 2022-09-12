#include <iostream>
#include <string>
#include <vector>
#include <set>

std::ostream& operator<<(std::ostream& out, const std::vector<long long>& v) {
    for (auto& i: v) {
        std::cout << i << " ";
    }
    return out;
}

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
    std::string str;
    std::getline(std::cin, str);
    str.push_back(' ');

    std::vector<long long> initPermut;
    std::string num;
    for (auto c: str) {
        if (c == ' ' && !num.empty()) {
            initPermut.push_back(std::stoll(num));
            num.clear();
        } else {
            num.push_back(c);
        }
    }

    std::set<std::vector<long long>> setOfPermuts;
    permute(setOfPermuts, initPermut, initPermut.size());

    for (auto& i: setOfPermuts) {
        if (i < initPermut)
            continue;
        std::cout << i << std::endl;
    }
    for (auto& i: setOfPermuts) {
        if (i == initPermut)
            break;
        std::cout << i << std::endl;
    }
    return 0;
}
