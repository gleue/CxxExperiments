#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

class Slab final {

    int width{0};
    int height{0};

    static std::unordered_map<int, long long> cache;

public:

    Slab(int w, int h): width{w}, height{h} {}

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

    long long findOptimalCuts(const std::vector<std::vector<int>>& prices) const;
    long long findOptimalPartition(std::vector<std::vector<int>>& prices) const;

    static inline void reset() { cache.clear(); }

private:

    long long cutVertically(const std::vector<std::vector<int>>& prices) const;
    long long cutHorizontally(const std::vector<std::vector<int>>& prices) const;

    long long partition(const std::vector<int>& price, std::vector<std::vector<int>>& prices) const;

    /// Splits this slab vertically at given x position (0 = left) returning the remaining rest-slab if any.
    std::optional<Slab> splitVertically(int x) const;
    /// Splits this slab horizontally at given y position (0 = top) returning the remaining rest-slab if any.
    std::optional<Slab> splitHorizontally(int y) const;
};
