#pragma once

#include <array>
#include <iostream>

template<typename T>
void printMe(T value) {
    std::cout << value;
}

template<typename T, int n>
class MyArray {

    std::array<T, n> _values{};

public:
    MyArray(const T& val) {
        _values.fill(val);
    }

#ifdef INLINE_FUNCTION
    void print() {
        for (auto val: _values) {
            printMe(val);
            std::cout << ' ';
        }
    }
#else
    void print();
#endif
};
