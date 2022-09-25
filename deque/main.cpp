#include <iostream>
#include <memory>
#include <string>
#include <sstream>

template<class T>
class Deque {
public:
    explicit Deque(size_t max_size): _max_size(max_size) {
        _data.reset(new T[max_size]);
    }
    void push_back(const T& data) {
        if (_size == _max_size) {
            std::cout << "overflow\n";
            return;
        }
        if ((_last == _max_size - 1) || !_size) {
            _data[0] = data;
            ++_size;
            _last = 0;
        } else {
            ++_last;
            _data[_last] = data;
            ++_size;
        }
    }
    void push_front(const T& data) {
        if (_size == _max_size) {
            std::cout << "overflow\n";
            return;
        }
        if (!_size) {
            _data[0] = data;
            ++_size;
        } else if (_first == 0) {
            _data[_max_size - 1] = data;
            ++_size;
            _first = _max_size - 1;
        } else {
            --_first;
            _data[_first] = data;
            ++_size;
        }
    }
    T pop_back() {
        if (!_size) {
            std::cout << "underflow\n";
            throw std::out_of_range("");
        } else if (_size == 1) {
            size_t tmp = _last;
            _last = _first = 0;
            --_size;
            return _data[tmp];
        } else if (_last == 0) {
            _last = _size - 1;
            --_size;
            return _data[0];
        } else {
            --_last;
            --_size;
            return _data[_last + 1];
        }
    }
    T pop_front() {
        if (!_size) {
            std::cout << "underflow\n";
            throw std::out_of_range("");
        } else if (_size == 1) {
            size_t tmp = _last;
            _last = _first = 0;
            --_size;
            return _data[tmp];
        } else if (_first == _max_size - 1) {
            _first = 0;
            --_size;
            return _data[_max_size - 1];
        } else {
            ++_first;
            --_size;
            return _data[_first - 1];
        }
    }
    void print() {
        if (!_size) {
            std::cout << "empty\n";
            return;
        }
        if (_first <= _last) {
            for (size_t i = _first; i < _last; ++i) {
                std::cout << _data[i] << " ";
            }
            std::cout << _data[_last] << std::endl;
        } else {
            for (size_t i = _first; i < _max_size; ++i) {
                std::cout << _data[i] << " ";
            }
            for (size_t i = 0; i < _last; ++i) {
                std::cout << _data[i] << " ";
            }
            std::cout << _data[_last] << std::endl;
        }
    }

private:
    std::unique_ptr<T[]> _data;
    size_t _first = 0;
    size_t _last = 0;
    size_t _size = 0;
    size_t _max_size;
};

int main() {
    int max_size;
    std::string command;
    bool size_set = false;
    while (std::getline(std::cin, command)) {
        if (command.empty()) {
            continue;
        }
        std::stringstream sstr(command);
        std::string fragment;
        sstr >> fragment;
        if (fragment == "set_size") {
            sstr >> max_size;
            if (max_size < 0 || !sstr.eof()) {
                std::cout << "error\n";
                continue;
            }
            size_set = true;
            break;
        } else {
            std::cout << "error\n";
            continue;
        }
    }
    if (!size_set) {
        return 0;
    }
    Deque<long long> deq(max_size);
    while (std::getline(std::cin, command)) {
        if (command.empty())
            continue;
        long long value;
        std::stringstream sstr(command);
        std::string fragment;
        sstr >> fragment;
        if (fragment == "pushb") {
            sstr >> value;
            if (!sstr.eof()) {
                std::cout << "error\n";
                continue;
            }
            deq.push_back(value);
            continue;
        } else if (fragment == "pushf") {
            sstr >> value;
            if (!sstr.eof()) {
                std::cout << "error\n";
                continue;
            }
            deq.push_front(value);
            continue;
        } else if (fragment == "popf") {
            if (!sstr.eof()) {
                std::cout << "error\n";
                continue;
            }
            try {
                std::cout << deq.pop_front() << std::endl;
            } catch (std::out_of_range&) {
                continue;
            }
        } else if (fragment == "popb") {
            if (!sstr.eof()) {
                std::cout << "error\n";
                continue;
            }
            try {
                std::cout << deq.pop_back() << std::endl;
            } catch (std::out_of_range &) {
                continue;
            }
        } else if (fragment == "print") {
            if (!sstr.eof()) {
                std::cout << "error\n";
                continue;
            }
            deq.print();
            continue;
        } else {
            std::cout << "error\n";
            continue;
        }
    }

    return 0;
}
