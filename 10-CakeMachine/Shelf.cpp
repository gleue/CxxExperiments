#include "Shelf.h"

void Shelf::push(std::size_t amount) {
    if (amount == 0) return;

    std::lock_guard<std::mutex> lock(mutex);

    count += amount;
    if (amount > 1)
        condition.notify_all();
    else
        condition.notify_one();
}

void Shelf::pop(std::size_t amount) {
    if (amount == 0) return;

    std::unique_lock<std::mutex> lock(mutex);

    if (count < amount) condition.wait(lock, [&]{ return count >= amount; });
    count -= amount;
}

std::size_t Shelf::getCount() const {
    std::lock_guard<std::mutex> lock(mutex);
    return count;
}
