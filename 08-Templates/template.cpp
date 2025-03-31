#include "template.h"
#include <iostream>

#ifndef INLINE_FUNCTION
template<typename T, int n>
void MyArray<T,n>::print() {
    for (auto val: _values) std::cout << val << ' ';
}
#endif

template<>
void printMe(int intValue) {
    std::cout << std::hex << "0x" << intValue << std::dec;
}
