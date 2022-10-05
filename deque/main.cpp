#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

int getSize(const std::string& s) {
    if (std::find_if(s.begin(), s.end(), [](char c){ return c < '0' || c > '9'; }) != s.end()) {
        throw std::invalid_argument("");
    } else {
        int size = std::stoi(s);
        if (size > 0)
            return size;
        else
            throw std::invalid_argument("");
    }
}

template<class T>
class Deque {

public:
    Deque() = delete;
    explicit Deque(size_t size): _capacity(size), _size(0), _first(0), _last(0) {
        _data.reset(new T[size]);
    }

    void push_front(const T &element) {
        if (_size == _capacity) {
            std::cout << "overflow" << std::endl;
            return;
        }
        if (_size != 0) {
            if (_first == 0) {
                _first = _capacity;
            }
            --_first;
        }
        _data[_first] = element;
        ++_size;
    }

    void push_back(const T &element) {
        if (_size == _capacity) {
            std::cout << "overflow" << std::endl;
            return;
        }
        if (_size != 0) {
            ++_last;
            if (_last == _capacity) {
                _last = 0;
            }
        }
        _data[_last] = element;
        ++_size;
    }

    T pop_front() {
        if (!_size) {
            std::cout << "underflow\n";
            throw std::out_of_range("");
        }
        T element = _data[_first];
        ++_first;
        if (_first == _capacity) {
            _first = 0;
        }
        --_size;
        if (!_size) {
            _first = 0;
            _last = 0;
        }
        return element;
    }

    T pop_back() {
        if (!_size) {
            std::cout << "underflow\n";
            throw std::out_of_range("");
        }
        T element = _data[_last];
        if (!_last) {
            _last = _capacity;
        }
        --_last;
        --_size;
        if (!_size) {
            _first = 0;
            _last = 0;
        }
        return element;
    }

    void print() {

        if (!_size) {
            std::cout <<"empty"<<std::endl;
            return;
        }
        for (int i = _first; i != _last; ++i) {
            if (i == _capacity) {
                i = -1;
                continue;
            }
            std::cout << _data[i] << " "; // ?
        }
        std::cout << _data[_last] << std::endl;
    }

private:
    std::unique_ptr<T[]> _data;
    size_t _first;
    size_t _last;
    size_t _size;
    size_t _capacity;
};

int main() {
    int max_size;
    std::string command;
    bool size_set = false;

    while (std::getline(std::cin, command) && !size_set) {
        if (command.empty()) {
            continue;
        }
        auto it = std::find_if(command.begin(), command.end(), isspace);
        if (it == command.end()) { std::cout << "error\n"; continue; }
        if (std::find_if(it + 1, command.end(), isspace) != command.end()) {  std::cout << "error\n"; continue; }
        if (std::string(command.begin(), it) != "set_size") { std::cout << "error\n"; continue; }
        std::string value = std::string(it + 1, command.end());
        try {
            max_size = getSize(value);
            size_set = true;
        } catch (const std::invalid_argument &) {
            std::cout << "error\n";
            continue;
        }
    }
    if (!size_set) {
        return 0;
    }
    Deque<std::string> deq(max_size);

    do {
        if (command.empty()) { continue; }

        auto it = std::find_if(command.begin(), command.end(), isspace);
        std::string fragment(command.begin(), it);
        if ((fragment == "pushb" || fragment == "pushf") && it != command.end()) {
            if (std::find_if(it + 1, command.end(), isspace) != command.end()) { std::cout << "error\n"; continue; }
        } else if (fragment == "popf" || fragment == "popb" || fragment == "print") {
            if (it != command.end()) { std::cout << "error\n"; continue; }
        } else { std::cout << "error\n"; continue; }

        if (fragment == "pushb") {
            deq.push_back(std::string(it + 1, command.end()));
        } else if (fragment == "pushf") {
            deq.push_front(std::string(it + 1, command.end()));
        } else if (fragment == "popf") {
            try {
                std::cout << deq.pop_front() << std::endl;
            } catch (std::out_of_range&) {}
        } else if (fragment == "popb") {
            try {
                std::cout << deq.pop_back() << std::endl;
            } catch (std::out_of_range &) {}
        } else if (fragment == "print") {
            deq.print();
        } else { std::cout << "error\n"; }
    } while (std::getline(std::cin, command));

    return 0;
}
