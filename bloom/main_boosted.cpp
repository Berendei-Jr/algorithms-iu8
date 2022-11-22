#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
typedef unsigned long long num;

class bloomFilter {
public:
    bloomFilter() = default;
    std::pair<size_t, size_t> set(const long long expectedElements, const double probability) {
        if (expectedElements <= 0 || probability <= 0 || probability >= 1)
            throw std::logic_error("Invalid parameters!\n");
        _m = round(-expectedElements*log2(probability)/log(2));
        _bitset.resize(_m);
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
    friend void print(const bloomFilter& b);

private:
    size_t hash(const num key, size_t i) const {
        return ((((i+1)*(key%Mersenne31) + _primeNumbers[i]))%Mersenne31)%_m;
    }
    void primesGenerator() {
        _primeNumbers.push_back(2);
        size_t tmp = 3;
        while (_primeNumbers.size() != _hashFunctionsNumber) {
            if (!(tmp % 2))
                continue;
            bool prime = true;
            for (size_t i = 3; i <= sqrt(tmp); i += 2) {
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
    // В векторе bool каждый элемент занимает 1 бит вместо sizeof(bool)

    size_t Mersenne31 = 2147483647;
    size_t _m = 0;
    size_t _hashFunctionsNumber = 0;
    std::vector<size_t> _primeNumbers;
};

void print(const bloomFilter& b) {
    for (auto i: b._bitset)
        std::cout << i;
    std::cout << std::endl;
}

int main() {
    std::ios::sync_with_stdio(false);
    bloomFilter bf;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        auto it = std::find_if(line.begin(), line.end(), isspace);
        std::string command = std::string(line.begin(), it);
        if (command != "set") { std::cout << "error\n"; continue; }
        auto it1 = std::find_if(it + 1, line.end(), isspace);
        std::string nStr = std::string(it + 1, it1);
        long long n = std::stoll(nStr);
        std::string pStr = std::string(it1 + 1, line.end());
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
        auto it = std::find_if(line.begin(), line.end(), isspace);
        std::string command = std::string(line.begin(), it);
        if (command == "print") {
            print(bf);
        } else if (command == "add") {
            std::string key_str = std::string(it + 1, line.end());
            bf.add(std::stoull(key_str));
        } else if (command == "search") {
            std::string key_str = std::string(it + 1, line.end());
            bf.contains(std::stoull(key_str)) ? std::cout << 1 << std::endl : std::cout << 0 << std::endl;
        } else {
            std::cout << "error\n";
        }
    }
    return 0;
}
