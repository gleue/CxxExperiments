#include "slab.h"

#include <algorithm>

long long Slab::findOptimalPartition(std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (const auto& price: prices) {
        money = std::max({money, partitionWidth(price, prices), partitionHeight(price, prices)});
    }

    return money;
}

long long Slab::partitionWidth(const std::vector<int>& price, std::vector<std::vector<int>>& prices) const {
    auto priceHeight{price[0]};
    auto priceWidth{price[1]};
    auto pricePrice{price[2]};

    // Slab too narrow -> skip
    if (width < priceWidth) return 0;

    // Slab exact width -> check height
    if (width == priceWidth) {
        // Slab not tall enough -> skip
        if (height < priceHeight) return 0;

        // Slab exact height -> update price
        if (height == priceHeight) {
            return pricePrice;
        }
        // Slab too tall -> split and examine rest
        else {
            long long moneyR{0};

            if (auto rest = splitHorizontally(priceHeight); rest) moneyR = rest.value().findOptimalPartition(prices);

            return moneyR + pricePrice;
        }
    }
    // Slab is too wide -> split vertically
    else {
        const auto& rest = splitVertically(priceWidth);

        long long moneyS = Slab(priceWidth, height).partitionHeight(price, prices);
        long long moneyR = rest ? rest.value().findOptimalPartition(prices) : 0;

        return moneyS + moneyR;
    }
}

long long Slab::partitionHeight(const std::vector<int>& price, std::vector<std::vector<int>>& prices) const {
    auto priceHeight{price[0]};
    auto priceWidth{price[1]};
    auto pricePrice{price[2]};

    // Slab too short -> skip
    if (height < priceHeight) return 0;

    // Slab exact height -> check width
    if (height == priceHeight) {
        // Slab not wide enough -> skip
        if (width < priceWidth) return 0;

        // Slab wide enough -> update price
        if (width == priceWidth) {
            return pricePrice;
        }
        // Slab too wide -> split and examine rest
        else {
            long long moneyR{0};

            if (auto rest = splitVertically(priceWidth); rest) moneyR = rest.value().findOptimalPartition(prices);

            return moneyR + pricePrice;
        }
    }
    // Slab is too tall -> split horizontally
    else {
        const auto& rest = splitHorizontally(priceHeight);

        auto moneyS = Slab(width, priceHeight).partitionWidth(price, prices);
        auto moneyR = rest ? rest.value().findOptimalPartition(prices) : 0;

        return moneyS + moneyR;
    }
}

std::optional<Slab> Slab::splitVertically(int x) const {
    if (x <= 0) return Slab(*this);
    if (x >= width) return std::nullopt;

    return Slab(width-x, height);
}

std::optional<Slab> Slab::splitHorizontally(int y) const {
    if (y <= 0) return Slab(*this); 
    if (y >= height) return std::nullopt;

    return Slab(width, height - y);
}
