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
    std::puts("Testcase 31: ");
    {
        std::vector<std::vector<int>> prices{{10,1,4},{19,2,26},{3,4,27},{5,2,2},{9,4,13},{16,6,1},{4,5,30},{18,7,22},{19,3,4},{10,5,18},{19,1,15},{7,5,23},{10,6,30},{11,1,1},{9,2,12},{13,2,8},{7,3,4},{1,5,18},{7,7,14}};

        Solution solution;
        auto maxMoney = solution.sellingWood(20, 7, prices);

        std::cout << maxMoney << '\n';
    }
    std::puts("Testcase 33: ");
    {
        std::vector<std::vector<int>> prices{{11,1,11},{12,2,18},{13,2,7},{5,1,2},{18,1,8},{1,1,15},{19,1,26},{3,2,30},{3,1,14},{17,1,1},{1,2,7},{12,1,1},{7,2,23},{7,1,18},{8,2,14},{10,2,7}};

        Solution solution;
        auto maxMoney = solution.sellingWood(19, 2, prices);

        std::cout << maxMoney << '\n';
    }
}
