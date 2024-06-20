#include "slab.h"

#include <algorithm>
#include <ranges>

long long Slab::findOptimalPartition(std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (const auto& price: std::ranges::views::filter(prices, [&](auto price){ return price[0] <=height && price[1] <= width; })) {
        money = std::max({money, cut(price, prices)});
    }

    return money;
}

long long Slab::cut(const std::vector<int>& price, std::vector<std::vector<int>>& prices) const {
    auto priceHeight{price[0]};
    auto priceWidth{price[1]};
    auto pricePrice{price[2]};

    // Slab too small -> skip
    if (width < priceWidth || height < priceHeight) return 0;

    // Slab exact width -> check height
    if (width == priceWidth) {
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
        long long moneyS{pricePrice};

        // Slab too tall -> split horizontally and examine rest
        if (height > priceHeight) {
            if (auto bottomRest = Slab(priceWidth, height).splitHorizontally(priceHeight); bottomRest) moneyS += bottomRest.value().findOptimalPartition(prices);
        }

        const auto& rightRest = splitVertically(priceWidth);
        long long moneyR = rightRest ? rightRest.value().findOptimalPartition(prices) : 0;

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
