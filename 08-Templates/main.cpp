// Comment to get link error
#define INLINE_FUNCTION

#include "template.h"
#include <iostream>

// Uncomment to call `printMe` function below
// instead of template specialization
void printMe(int value);

int main() {

    MyArray<int, 10> tenInts(99);

    tenInts.print(); std::cout << std::endl;

    int anInt{ 255 };
    float aFloat{ 255. };

    printMe(anInt); std::cout << std::endl;
    printMe(aFloat); std::cout << std::endl;

    return 0;
}

void printMe(int value) {
    std::cout << value;
}
