#include "solution.h"

#include <iostream>

int main() {
    std::puts("Example 1: ");
    {
        std::vector<std::vector<int>> prices{{1,4,2}, {2,2,7}, {2,1,3}};

        Solution solution;
        auto maxMoney = solution.sellingWood(3, 5, prices);

        std::cout << maxMoney << '\n';
    }
    std::puts("Example 2: ");
    {
        std::vector<std::vector<int>> prices{{3,2,10}, {1,4,2}, {4,1,3}};

        Solution solution;
        auto maxMoney = solution.sellingWood(4, 6, prices);

        std::cout << maxMoney << '\n';
    }
}
