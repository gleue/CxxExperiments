#include <vector>

#include "slab.h"

/// Skeleton solution class from leetcode.com
class Solution {
public:
    long long sellingWood(int m, int n, std::vector<std::vector<int>>& prices) {
        Slab::reset();
        // return Slab(n, m).findOptimalPartition(prices);
        return Slab(n, m).findOptimalCuts(prices);
    }
};
