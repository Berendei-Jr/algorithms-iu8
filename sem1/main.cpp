#include <iostream>
#include <string>
#include <list>

inline void add (std::list<std::pair<long long, int>>& l, long long value) {
    for (auto &i: l) {
        if (i.first == value) {
            ++i.second;
            return;
        }
    }
    l.emplace_back(value, 1);
}

int main() {
    std::string str;
    std::getline(std::cin, str);
    str += ' ';

    std::list<std::pair<long long, int>> list;
    std::string num_str;

    for (auto i: str) {
        if (i == ' ' && !num_str.empty()) {
            add(list, std::stoll(num_str));
            num_str.clear();
        } else {
            num_str.push_back(i);
        }
    }

    long long min = list.begin()->first;
    int max_freq = list.begin()->second;
    for (auto &i: list) {
        if (i.second > max_freq) {
            max_freq = i.second;
            min = i.first;
        } else if (i.second == max_freq && i.first < min) {
            min = i.first;
        }
    }
    std::cout << min;
}
