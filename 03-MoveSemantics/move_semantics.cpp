#include <cstdio>
#include <source_location>
#include <utility>

void print(const std::source_location &location = std::source_location::current()) noexcept {
    std::puts(location.function_name());
}

struct Lifetime {
    Lifetime() noexcept { print(); }
    Lifetime(int) noexcept { print(); }
    Lifetime(const Lifetime &) noexcept { print(); }
    Lifetime(Lifetime &&) noexcept { print(); }
    
    ~Lifetime() noexcept { print(); }

    Lifetime& operator=(const Lifetime &) noexcept { print(); return *this; }
    Lifetime& operator=(Lifetime &&) noexcept { print(); return *this; }
};

void callByValue(Lifetime lt) {
    auto lifetime = std::move(lt);
}

void callByConstRef(const Lifetime &lt) {
    auto lifetime = lt;
}

void callByRValueRef(Lifetime &&lt) {
    auto lifetime = std::move(lt);
}

int main() {
    auto lt = Lifetime(1);

    std::puts("\nValue:");
    callByValue(lt);

    std::puts("\nConst Ref:");
    callByConstRef(lt);

    std::puts("\nRvalue Ref:");
    callByRValueRef(std::move(lt));

    std::puts("\nRvalue Temp:");
    callByRValueRef(Lifetime(2));

    std::puts("");
}
