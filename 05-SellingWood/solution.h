#include <vector>

#include "slab.h"

/// Skeleton solution class from leetcode.com
class Solution {
public:
    long long sellingWood(int m, int n, const std::vector<std::vector<int>>& prices) {
        // return Slab(n, m).findOptimalPartition(prices);
        Slab::reset(prices);
        return Slab(n, m).findOptimalCuts(prices);
    }
};
