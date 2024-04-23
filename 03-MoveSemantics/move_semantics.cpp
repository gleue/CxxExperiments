#include <cstdio>
#include <optional>
#include <source_location>
#include <utility>

void print(const std::source_location &location = std::source_location::current()) noexcept {
    std::puts(location.function_name());
}

struct Lifetime {
    Lifetime() noexcept { print(); }
    explicit Lifetime(int) noexcept { print(); }
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

// See: C++ Weekly - Ep 421 - You're Using optional, variant, pair, tuple, any, and expected Wrong!
//      https://www.youtube.com/watch?v=0yJk5yfdih0

Lifetime returnValue() {
    return Lifetime{1985};
}

std::optional<Lifetime> returnOptional() {
    return std::optional<Lifetime>{1985};
}

std::optional<Lifetime> returnOptionalValue() {
    return Lifetime{1985};
}

int main() {
    {
        auto lt = Lifetime{1};

        std::puts("\nPass by value:");
        callByValue(lt);

        std::puts("\nPass by const ref:");
        callByConstRef(lt);

        std::puts("\nPass by rvalue ref:");
        callByRValueRef(std::move(lt));

        std::puts("\nPass by rvalue temp:");
        callByRValueRef(Lifetime{2});

        std::puts("");
    }
    {
        std::puts("\nReturn value:");
        returnValue();

        std::puts("\nReturn optional:");
        returnOptional();

        std::puts("\nReturn optional value:");
        returnOptionalValue();
    }
}
