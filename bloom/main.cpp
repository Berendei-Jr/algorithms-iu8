#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
typedef unsigned long long num;

class bloomFilter {
public:
    bloomFilter() = default;
    std::pair<num, size_t> set(const long long expectedElements, const double probability) {
        if (expectedElements <= 0 || probability <= 0 || probability >= 1)
            throw std::logic_error("Invalid parameters!\n");
        _m = round(-expectedElements*log2(probability)/log(2));
        _bitset.reserve(_m);
        for (size_t i = 0; i < _m; ++i) {
            _bitset.push_back(false);
        }
        _hashFunctionsNumber = round(-log2(probability));
        if (!_hashFunctionsNumber)
            throw std::logic_error("Invalid parameters!\n");
        else if (_hashFunctionsNumber == 1) {
            _primeNumbers.push_back(2);
        } else {
            primesGenerator();
        }
        return std::make_pair(_m, _hashFunctionsNumber);
    }
    void add(num key) {
        for (size_t i = 0; i < _hashFunctionsNumber; ++i) {
            _bitset[hash(key, i)] = true;
        }
    }
    bool contains(const num key) const {
        for (size_t i = 0; i < _hashFunctionsNumber; ++i) {
            if (!_bitset[hash(key, i)])
                return false;
        }
        return true;
    }
    void print(std::ostream& stream) const {
        for (auto i: _bitset) {
            stream << i;
        }
        stream << std::endl;
    }

private:
    size_t hash(const num key, size_t i) const {
        return ((((i+1)%Mersenne31*key%Mersenne31 + _primeNumbers[i]%Mersenne31))%Mersenne31)%_m;
    }
    void primesGenerator() {
        _primeNumbers.reserve(_hashFunctionsNumber);
        _primeNumbers.push_back(2);
        num tmp = 3;
        while (_primeNumbers.size() != _primeNumbers.capacity()) {
            if (!(tmp % 2))
                continue;
            bool prime = true;
            for (num i = 3; i <= sqrt(tmp); i += 2) {
                if (!(tmp % i)) {
                    prime = false;
                    break;
                }
            }
            if (prime) { _primeNumbers.push_back(tmp); }
            tmp += 2;
        }
    }
    std::vector<bool> _bitset;
    long long _m = 0;
    size_t _hashFunctionsNumber = 0;
    std::vector<num> _primeNumbers;
    long long Mersenne31 = 2147483647;
};

int main() {
    bloomFilter bf;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        //line.pop_back();
        auto it = std::find_if(line.begin(), line.end(), isspace);
        std::string command = std::string(line.begin(), it);
        if (command != "set" || it == line.end()) { std::cout << "error\n"; continue; }
        auto it1 = std::find_if(it + 1, line.end(), isspace);
        std::string nStr = std::string(it + 1, it1);
        long long n = std::stoll(nStr);
        if (it1 == line.end()) { std::cout << "error\n"; continue; }
        auto it2 = std::find_if(it1 + 1, line.end(), isspace);
        std::string pStr = std::string(it1 + 1, it2);
        if (it2 != line.end()) { std::cout << "error\n"; continue; }
        double p = std::stod(pStr);
        try {
            auto pair = bf.set(n, p);
            std::cout << pair.first << " " << pair.second << std::endl;
            break;
        }
        catch (std::logic_error&) {
            std::cout << "error\n"; continue;
        }
    }

    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        //line.pop_back();
        auto it = std::find_if(line.begin(), line.end(), isspace);
        std::string command = std::string(line.begin(), it);
        if (command == "print") {
            if (it != line.end()) {
                std::cout << "error\n";
                continue;
            }
            bf.print(std::cout);
        } else if (command == "add") {
            if (it == line.end()) {
                std::cout << "error\n";
                continue;
            }
            auto second_it = std::find_if(it + 1, line.end(), isspace);
            if (second_it != line.end()) {
                std::cout << "error\n";
                continue;
            }
            std::string key_str = std::string(it + 1, second_it);
            num key = std::stoull(key_str);
            bf.add(key);
        } else if (command == "search") {
            if (it == line.end()) {
                std::cout << "error\n";
                continue;
            }
            auto second_it = std::find_if(it + 1, line.end(), isspace);
            if (second_it != line.end()) {
                std::cout << "error\n";
                continue;
            }
            std::string key_str = std::string(it + 1, second_it);
            num key = std::stoull(key_str);
            bf.contains(key) ? std::cout << 1 << std::endl : std::cout << 0 << std::endl;
        } else {
            std::cout << "error\n";
        }
    }
    return 0;
}
