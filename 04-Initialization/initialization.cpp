#include <cstdio>
#include <source_location>
#include <vector>

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

int main() {
    puts("Hello, initialization!");

    std::puts("\nDefault initialization:");
    {
        Lifetime lt;
    }

    std::puts("\nValue initialization:");
    {
        Lifetime lt{};
    }

    std::puts("\nDirect initialization:");
    {
        Lifetime lt1(42);
        Lifetime lt2{42};
    }

    std::puts("\nCopy initialization:");
    {
        Lifetime lt = Lifetime(42);
    }

    std::puts("\nAssignment:");
    {
        Lifetime lt;
        lt = Lifetime(42);
        lt = Lifetime{42};
    }

    std::puts("\nArray:");
    {
        //Lifetime lt1[] = { {1}, {2}, {3} };
        Lifetime lt2[] = { Lifetime(1), Lifetime(2), Lifetime(3) };
    }

    std::puts("\nVector (copy):");
    {
        std::vector<Lifetime> lt = { Lifetime(1), Lifetime(2), Lifetime(3) };
    }

    std::puts("\nVector (move):");
    {
        std::vector<Lifetime> lt = { std::move(Lifetime(1)), std::move(Lifetime(2)), std::move(Lifetime(3)) };
    }
}
