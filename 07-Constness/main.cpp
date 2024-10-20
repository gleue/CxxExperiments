#include <array>
#include <utility>

//#define CONSTEVAL

constexpr unsigned fac_switch(unsigned n) {
    switch (n) {
        case 0:
        case 1:
            return 1;
        default:
            return n * fac_switch(n-1);
    }
}

constexpr unsigned fac_loop(unsigned n){
    unsigned result = 1;
    for(unsigned i = 2; i <= n; i++){
        result *= i;
    }
    return result;
}

constexpr unsigned fac_recursive_constexpr(unsigned n){
    return (n > 1) ? n * fac_recursive_constexpr(n - 1) : 1;
}

consteval unsigned fac_recursive_consteval(unsigned n){
    return (n > 1) ? n * fac_recursive_consteval(n - 1) : 1;
}

#ifdef CONSTEVAL
#define factorial fac_recursive_consteval
#else
#define factorial fac_recursive_constexpr
#endif

int main() {
    
    //
    // Variables/Constants
    //

    constexpr int v[]{1, 2, 3, 4, 5, 6};
    constexpr auto a1{std::to_array(v)};
    constexpr std::array<int, 7> a2{20, 1, -5, 6, -1, 8, 31};   
    constexpr auto s = std::integer_sequence<int, 7, 31, 8, -1, 6, -5, 1, 20>{};

    //
    // Functions & Static variables
    //

    // Literal argument/initialization
    auto flvar1 = factorial(6); // run-time function call
    const auto flconst1 = factorial(6); // compile-time result 720
    constexpr auto flconstexpr1 = factorial(6); // compile-time result 720

    constinit static unsigned stat1 = 6;
    constinit static unsigned stat1a = factorial(6);

    // Variable argument/initialization
    unsigned var = 7;
#ifndef CONSTEVAL
    auto flvar2 = factorial(var); // run-time function call
    const auto flconst2 = factorial(var); // run-time function call
#endif
    // ERROR: constexpr auto flconstexpr2 = factorial(var);

    // ERROR: constinit static unsigned stat2 = var;
    // ERROR: constinit static unsigned stat2a = factorial(var);

    // Constant argument/initialization
    const unsigned cnst = 8;
    auto flvar3 = factorial(cnst); // run-time function call (constexpr) / compile-time result 40.320 (consteval)
    const auto flconst3 = factorial(cnst); // compile-time result 40.320
    constexpr auto flconstexpr3 = factorial(cnst); // compile-time result 40.320

    constinit static unsigned stat3 = cnst;
    constinit static unsigned stat3a = factorial(cnst);

    // Constexpr argument/initialization
    constexpr unsigned cxpr = 9;
    auto flvar4 = factorial(cxpr); // run-time function call (constexpr) / compile-time result 362.880 (consteval)
    const auto flconst4 = factorial(cxpr); // compile-time result 362.880
    constexpr auto flconstexpr4 = factorial(cxpr); // compile-time result 362.880

    constinit static unsigned stat4 = cxpr;
    constinit static unsigned stat4a = factorial(cxpr);

    // Constinit argument/initialization
    constinit static unsigned cnit = 10;
    // ERROR: auto flvar5 = factorial(cnit); // run-time function call (constexpr) / compile-time result 362.880 (consteval)
    // ERROR: const auto flconst5 = factorial(cnit);
    // ERROR: constexpr auto flconstexpr5 = factorial(cnit);

    // ERROR: constinit static unsigned stat5 = cnit;
    // ERROR: constinit static unsigned stat5a = factorial(cnit);

    return 0;
}
