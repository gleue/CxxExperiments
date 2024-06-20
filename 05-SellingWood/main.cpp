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
    std::puts("Testcase 27: ");
    {
        std::vector<std::vector<int>> prices{{2,3,15},{5,2,10},{1,3,27},{1,2,23},{5,1,11},{4,7,23}};

        Solution solution;
        auto maxMoney = solution.sellingWood(5, 9, prices);

        std::cout << maxMoney << '\n';
    }
    std::puts("Testcase 30: ");
    {
        std::vector<std::vector<int>> prices{{4,12,3},{5,3,18},{1,2,10},{2,12,22},{2,6,12},{3,8,30},{2,3,30},{3,12,17},{2,4,20},{3,3,19},{5,10,6},{4,10,26},{4,1,28},{5,9,9},{5,13,2},{2,5,19},{3,4,2}};

        Solution solution;
        auto maxMoney = solution.sellingWood(5, 13, prices);

        std::cout << maxMoney << '\n';
    }
}
