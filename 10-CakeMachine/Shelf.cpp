#include "Shelf.h"

bool Shelf::push() {
    if (closed.load()) return false;

    std::lock_guard<std::mutex> lock(mutex);
    ++count;
    condition.notify_all();
    return !closed.load();
}

bool Shelf::pop(std::stop_token token) {
    if (closed.load()) return false;

    std::unique_lock<std::mutex> lock(mutex);

    if (count < 1) condition.wait(lock, token, [&]{ return closed.load() || count >= 1; });
    --count;
    return !closed.load();
}

std::size_t Shelf::getCount() const {
    if (closed.load()) return 0;

    std::lock_guard<std::mutex> lock(mutex);
    return count;
}

void Shelf::close() {
    if (closed.load()) return;

    closed = true;

    std::lock_guard<std::mutex> lock(mutex);
    condition.notify_all();
}
