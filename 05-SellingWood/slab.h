#pragma once

#include <optional>
#include <vector>

class Slab final {

    int width{0};
    int height{0};

public:

    Slab(int w, int h): width{w}, height{h} {}

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

    long long findOptimalPartition(std::vector<std::vector<int>>& prices) const;

private:

    long long partitionHorizontally(std::vector<std::vector<int>>& prices) const;
    long long partitionVertically(std::vector<std::vector<int>>& prices) const;

    /// Splits this slab vertically at given x position (0 = left) returning the remaining rest-slab if any.
    std::optional<Slab> splitVertically(int x) const;
    /// Splits this slab horizontally at given y position (0 = top) returning the remaining rest-slab if any.
    std::optional<Slab> splitHorizontally(int y) const;
};
