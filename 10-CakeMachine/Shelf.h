#pragma once

#include <mutex>

class Shelf {

    std::size_t count = 0;

    mutable std::mutex mutex;
    std::condition_variable condition;

public:

    void push(std::size_t amount = 1);
    void pop(std::size_t amount = 1);

    std::size_t getCount() const;
};
