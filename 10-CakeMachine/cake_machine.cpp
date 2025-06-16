#include "Workstation.h"

#include "../shared/mygetopt.h"

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

constexpr int defaultNumberOfCakes = 0;
constexpr int defaultReportInterval = 10;

int numberOfCakes = defaultNumberOfCakes;
int reportInterval = defaultReportInterval;

void usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " [options]\n\n";
    std::cerr << "Creates an infinite number of cakes and reports progress every 10 seconds.\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help                     = Print this help\n" \
                 "  -c | --cakes number             = Number of cakes to produce (0 == infinite)\n" \
                 "  -r | --report seconds           = Seconds between progress reports\n\n";
    std::cerr << std::endl;
}

int parseArguments(int argc, char* argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "cakes", 1, nullptr, 'c' },
        { "report", 1, nullptr, 'r' },
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "hc:r:", longOptions, nullptr)) > 0) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                exit(0);
            case 'c':
                numberOfCakes = atoi(optarg);
                if (numberOfCakes < 0) numberOfCakes = defaultNumberOfCakes;
                break;
            case 'r':
                reportInterval = atoi(optarg);
                if (reportInterval < 1) reportInterval = defaultReportInterval;
                break;
            case '?':
                usage(argv[0]);
                exit(1);
        }
    }

    return optind;
}

int main(int argc, char* argv[]) {

    auto index = parseArguments(argc, argv);

    if (argc - optind != 0) {
        usage(argv[0]);
        return 1;
    }

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
    auto startTime = std::chrono::steady_clock::now();
    std::size_t curTotal = 0;
    while (numberOfCakes == 0 || curTotal < numberOfCakes) {
        for (std::size_t i = 0; i < reportInterval; std::this_thread::sleep_for(1s), i++) {
            std::cout << ((i % 10) ? "." : ":") << std::flush;
        }

        auto newTotal = total.getCount();
        auto delta = newTotal - curTotal;
        auto reportTime = std::chrono::steady_clock::now();
        std::cout << " " << prep << " > " << bake << " > " << pack << " = Total: " << newTotal << " (+" << delta << ")";
        curTotal = newTotal;
        
        auto durationSinceStart = std::chrono::duration<double>(reportTime - startTime);
        auto factor = 60s / durationSinceStart;
        auto throughput = double(curTotal) * factor;
        std::cout << " [" << throughput << "/m]\n";
    }

    puts("Finishing...");
    shelf1->close();
    shelf2->close();
    shelf3->close();

    prep.stop();
    bake.stop();
    pack.stop();
    total.stop();

    return 0;
}
