#include "slab.h"

#include <algorithm>

long long Slab::findOptimalPartition(std::vector<std::vector<int>>& prices) const {
    return std::max(partitionHorizontally(prices), partitionVertically(prices));
}

long long Slab::partitionHorizontally(std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (const auto& price: prices) {
        auto priceHeight{price[0]};
        auto priceWidth{price[1]};
        auto pricePrice{price[2]};

        // Slab too narrow -> skip
        if (width < priceWidth) continue;

        // Slab exact width -> check height
        if (width == priceWidth) {
            // Slab not tall enough -> skip
            if (height < priceHeight) continue;

            // Slab exact height -> update price
            if (height == priceHeight) {
                money = std::max(money, static_cast<long long>(pricePrice));
            }
            // Slab too tall -> split and examine rest
            else {
                decltype(money) moneyR{0};

                if (auto rest = splitHorizontally(priceHeight); rest) moneyR = rest.value().findOptimalPartition(prices);

                money = std::max({money, moneyR + pricePrice, moneyR + pricePrice});
            }
        }
        // Slab is too wide -> split vertically
        else {
            const auto& rest = splitVertically(priceWidth);

            decltype(money) moneyS = Slab(priceWidth, height).partitionVertically(prices);
            decltype(money) moneyR = rest ? rest.value().findOptimalPartition(prices) : 0;

            money = std::max(money, moneyS + moneyR);
        }
    }

    return money;
}

long long Slab::partitionVertically(std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (const auto& price: prices) {
        auto priceHeight{price[0]};
        auto priceWidth{price[1]};
        auto pricePrice{price[2]};

        // Slab too short -> skip
        if (height < priceHeight) continue;

        // Slab exact height -> check width
        if (height == priceHeight) {
            // Slab not wide enough -> skip
            if (width < priceWidth) continue;

            // Slab wide enough -> update price
            if (width == priceWidth) {
                money = std::max(money, static_cast<long long>(pricePrice));
            }
            // Slab too wide -> split and examine rest
            else {
                decltype(money) moneyR{0};

                if (auto rest = splitVertically(priceWidth); rest) moneyR = rest.value().findOptimalPartition(prices);

                money = std::max({money, moneyR + pricePrice, moneyR + pricePrice});
            }
        }
        // Slab is too tall -> split horizontally
        else {
            const auto& rest = splitHorizontally(priceHeight);

            decltype(money) moneyS = Slab(width, priceHeight).partitionHorizontally(prices);
            decltype(money) moneyR = rest ? rest.value().findOptimalPartition(prices) : 0;

            money = std::max(money, moneyS + moneyR);
        }
    }

    return money;
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
