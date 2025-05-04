#include "Workstation.h"

#include <chrono>
#include <iostream>
#include <random>

using namespace std::chrono_literals;

std::mutex mutex;
std::random_device seed;
std::mt19937 gen(seed());

std::chrono::duration<long double> prepTimerFunc() {
    std::lock_guard lock(mutex);
    static std::uniform_int_distribution<std::size_t> dist(5000.0, 8000.0);
    auto millis = std::chrono::milliseconds(dist(gen));
    return millis;
}

int reportInterval = 10;

int main(int argc, char* argv[]) {

    if (argc > 1) reportInterval = std::max(std::atoi(argv[1]), 1);

    puts("Initializing...");
    auto shelf1{ std::make_shared<Shelf>()};
    auto shelf2{ std::make_shared<Shelf>()};
    auto shelf3{ std::make_shared<Shelf>()};

    auto prepTimer = []{ return prepTimerFunc(); };
    Workstation<3, decltype(prepTimer)> prep("Prep", nullptr, shelf1, prepTimer);

    auto bakeTimer = []{ return 10s; };
    Workstation<5, decltype(bakeTimer)> bake("Bake", shelf1, shelf2, bakeTimer);

    auto packTimer = []{ return 2s; };
    Workstation<2, decltype(packTimer)> pack("Pack", shelf2, shelf3, packTimer);

    auto outTimer = []{ return 0s; };
    Workstation<1, decltype(outTimer)> total("Total", shelf3, nullptr, outTimer);

    puts("Working...");
    auto startTime = std::chrono::high_resolution_clock::now();
    std::size_t curTotal = 0;
    while (true) {
        for (std::size_t i = 0; i < reportInterval; std::this_thread::sleep_for(1s), i++) {
            std::cout << ((i % 10) ? "." : ":") << std::flush;
        }

        auto newTotal = total.getCount();
        auto delta = newTotal - curTotal;
        auto reportTime = std::chrono::high_resolution_clock::now();
        std::cout << " " << prep << " > " << bake << " > " << pack << " = Total: " << newTotal << " (+" << delta << ")";
        curTotal = newTotal;
        
        auto durationSinceStart = std::chrono::duration<double>(reportTime - startTime);
        auto factor = 60s / durationSinceStart;
        auto throughput = double(curTotal) * factor;
        std::cout << " [" << throughput << "/m]\n";
    }

    return 0;
}
