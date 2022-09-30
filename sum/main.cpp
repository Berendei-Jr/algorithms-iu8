#include <iostream>
#include <string>

int main() {
    long long sum = 0;
    std::string str;
    bool minus = false;
    std::string num;

    while (std::getline(std::cin, str)) {
        if (str.empty() || str == "\n")
            continue;
        if (str == "\n")
            continue;

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
            } else if (!num.empty()) {
                sum += std::stol(num);
                num.clear();
            } else if (i != '-') {
                minus = false;
            }
        }
        if (!num.empty()) {
            sum += std::stol(num);
            num.clear();
        }
    }

    if (!num.empty())
        sum += std::stol(num);
    std::cout << sum;
}
