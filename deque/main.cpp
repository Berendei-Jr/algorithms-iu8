#include <iostream>
#include <memory>

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
        if (_first <= _last) {
            for (size_t i = _first; i <= _last; ++i) {
                std::cout << _data[i] << " ";
            }
            std::cout << std::endl;
        } else {
            for (size_t i = _first; i < _max_size; ++i) {
                std::cout << _data[i] << " ";
            }
            for (size_t i = 0; i <= _last; ++i) {
                std::cout << _data[i] << " ";
            }
            std::cout << std::endl;
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
    Deque<int> d(5);
    d.push_back(1);
    d.push_front(2);
    d.push_back(3);
    d.push_front(4);
    d.push_back(5);
    std::cout << d.pop_back() << std::endl;
    std::cout << d.pop_front() << std::endl;
    std::cout << d.pop_front() << std::endl;
    std::cout << d.pop_front() << std::endl;
    std::cout << d.pop_front() << std::endl;
    std::cout << d.pop_front() << std::endl;
    d.print();
    return 0;
}
