#include "slab.h"

#include <algorithm>
#include <ranges>

namespace {
    inline int sizeToIndex(int w, int h) { return (h << 8) + w; }
}

std::unordered_map<int, long long> Slab::moneyCache{};
std::unordered_map<int, long long> Slab::priceCache{};

void Slab::reset(const std::vector<std::vector<int>>& prices) {
    moneyCache.clear();
    priceCache.clear();

    for (const auto& price: prices) {
        priceCache[sizeToIndex(price[1], price[0])] = price[2];
    }
}

long long Slab::findOptimalCuts(const std::vector<std::vector<int>>& prices) const {
    if (auto item = moneyCache.find(sizeToIndex(width, height)); item != moneyCache.end()) {
        return item->second;
    }

    long long money{findPrice(prices)};
    
    moneyCache[sizeToIndex(width, height)] = std::max({money, width > 1 ? cutVertically(prices) : 0, height > 1 ? cutHorizontally(prices) : 0});
    return moneyCache[sizeToIndex(width, height)];
}

long long Slab::cutVertically(const std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (int w = 1; w <= width/2 + width%2; w++) {
        Slab left(w, height);
        auto right{splitVertically(w)};

        money = std::max(money, left.findOptimalCuts(prices) + (right ? right.value().findOptimalCuts(prices) : 0));
    }
    return money;
}

long long Slab::cutHorizontally(const std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (int h = 1; h <= height/2 + height%2; h++) {
        Slab top(width, h);
        auto bottom{splitHorizontally(h)};

        money = std::max(money, top.findOptimalCuts(prices) + (bottom ? bottom.value().findOptimalCuts(prices) : 0));
    }
    return money;
}

long long Slab::findOptimalPartition(std::vector<std::vector<int>>& prices) const {
    long long money{0};

    for (const auto& price: std::ranges::views::filter(prices, [&](auto price){ return price[0] <=height && price[1] <= width; })) {
        money = std::max({money, partition(price, prices)});
    }

    return money;
}

long long Slab::partition(const std::vector<int>& price, std::vector<std::vector<int>>& prices) const {
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

long long Slab::findPrice(const std::vector<std::vector<int>>& prices) const {
    if (auto match = priceCache.find(sizeToIndex(width, height)); match != priceCache.end()) {
        return match->second;
    }
    // if (auto match = std::find_if(prices.begin(), prices.end(), [&](const auto& price){ return price[0] == height && price[1] == width; }); match != prices.end()) {
    //     return (*match)[2];
    // }
    return 0;
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
