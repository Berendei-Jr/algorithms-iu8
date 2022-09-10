#include <iostream>
#include <string>
#include <list>

int main() {
    std::string str;
    bool double_blank = false;
    while (true) {
        std::string s;
        std::getline(std::cin, s);
        if (s == "\n")
            continue;
        if (s.empty()) {
            if (double_blank)
                break;
            double_blank = true;
        } else {
            str += " " + s;
            double_blank = false;
        }
    }
    str += " ";

    std::list<long> list;
    std::string num;
    bool minus = false;
    for (auto i: str) {
        if (num.empty() && i == '-') {
            minus = true;
        }
        if (i >= '0' && i <= '9') {
            if (minus) {
                num += '-';
                minus = false;
            }
            num += i;
        }
        else if (!num.empty()) {
            list.push_back(std::stol(num));
            num.clear();
        } else if (i != '-') {
            minus = false;
        }
    }

    long long sum = 0;
    for (auto i: list) {
        sum += i;
    }
    std::cout << sum;
}
