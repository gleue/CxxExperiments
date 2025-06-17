#include "City.h"

#include "../shared/mygetopt.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <sstream>
#include <string_view>
#include <vector>

void usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " csv-file\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help   = Print this help\n" \
                 "  -H | --header = input file has no header row\n";
    std::cerr << "Returns\n\n";
    std::cerr << "  0       = OK\n" \
                 "  1       = ERROR\n";
    std::cerr << std::endl;
}

bool hasHeaderRow{true};
char delimiterChar{';'};

int parseArguments(int argc, char* argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "header", 0, nullptr, 'H'},
        { "delim", 0, nullptr, 'D'},
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "hHD:", longOptions, nullptr)) > 0) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                exit(0);
            case 'H':
                hasHeaderRow = false;
                break;
            case 'D':
                delimiterChar = optarg[0];
                break;
            case '?':
                usage(argv[0]);
                exit(1);
        }
    }

    return optind;
}

std::optional<std::vector<City>> readCitiesFromCSV(const std::string& filepath, bool hasHeader = false, char delim = ';') {

    std::ifstream csvstream(filepath);

    if (!csvstream.good()) return std::nullopt;

    std::vector<City> cities;

    bool headerSeen = !hasHeader;
    while (!csvstream.eof()) {
        std::string csvline;

        if (!std::getline(csvstream, csvline)) break;

        // Skip header (if necessary)
        if (!headerSeen) {
            headerSeen = true;
            continue;
        }

        // Parse data lines
        auto fields = csvline | std::views::split(delimiterChar) | std::views::transform([](auto const f){ return std::string_view(f); });
        auto name = fields.front();
        auto state = std::next(std::ranges::begin(fields), 16);

        unsigned population = 0;
        {
            auto populationString = std::string(*std::next(std::ranges::begin(fields), 11));
            std::istringstream istr(populationString);
            istr >> population;
        }

        float area = .0;
        {
            auto areaString = std::string(*std::next(std::ranges::begin(fields), 12));
            std::istringstream istr(areaString);
            istr >> area;
        }

        unsigned since = 0;
        {
            auto sinceString = std::string(*std::next(std::ranges::begin(fields), 15));
            std::istringstream istr(sinceString);
            istr >> since;
        }

        cities.push_back(City{ std::string(name), population, area, since, std::string(*state)});
    }

    return cities;
}

int main(int argc, char* argv[]) {

    auto index = parseArguments(argc, argv);

    if (argc - optind == 0) {
        usage(argv[0]);
        return 1;
    }

    auto citiesOpt = readCitiesFromCSV(argv[optind], hasHeaderRow, delimiterChar);
    if (!citiesOpt) return 1;

    auto cities = citiesOpt.value();
    std::cout << "All cities:\n";
    for (auto const city: cities) std::cout << city << std::endl;

    // Check if all have >= 100.000 inhabitants:
    //
    if (std::ranges::all_of(cities, [](auto const c){ return c.population >= 100'000; })) {
        std::cout << "\nAll cities have at least 100k inhabitants\n";
    }

    // Sum total population
    //
    auto total = std::ranges::fold_left(cities, 0, [](unsigned total, const City& city)-> unsigned { return total += city.population; });
    std::cout << "\nTotal population: " << total << std::endl;

    // Cities by population
    //
    std::ranges::sort(cities, std::ranges::greater(), &City::population);

    auto largeCities = cities | std::views::filter([](auto const c){ return c.population >= 1'000'000; });
    std::cout << "\nLarge cities (>= 1M inhabitants):\n";
    for (auto const city: largeCities) std::cout << city.name << ": " << city.population << std::endl;

    auto mediumCities = cities | std::views::drop_while([](auto const c){ return c.population >= 1'000'000; }) | std::views::take_while([](auto const c){ return c.population >= 200'000; });
    std::cout << "\nMedium cities (< 1M and >= 200k inhabitants):\n";
    for (auto const city: mediumCities) std::cout << city.name << ": " << city.population << std::endl;

    auto densities = cities | std::views::transform([](auto const c)->float{ return float(c.population)/float(c.area); });
    std::cout << "\nPopulation density per city:\n";
    for (auto const [city, density]: std::views::zip(cities, densities)) std::cout << city.name << ": " << density << " inh/sqmkm" << std::endl;

    // 5 oldest cities:
    //
    std::ranges::sort(cities, std::ranges::less(), &City::since);

    auto topFiveOldest = std::views::counted(cities.begin(), 5);
    std::cout << "\nTop 5 oldest cities:\n";
    for (auto const city: topFiveOldest) std::cout << city.name << ": " << city.since << std::endl;

    // State with cities in them
    //
    std::vector<std::string> statesWithCities;
    auto states = cities | std::views::transform([](auto const city)->std::string { return city.state; });
    std::ranges::copy_if(states, std::back_inserter(statesWithCities), [](auto const s){ return !s.empty(); });
    std::ranges::sort(statesWithCities);
    auto toErase = std::ranges::unique(statesWithCities);
    statesWithCities.erase(toErase.begin(), toErase.end());

    std::cout << "\nStates with cities:\n";
    // for (auto const state: statesWithCities) std::cout << state << std::endl;
    std::ranges::copy(statesWithCities, std::ostream_iterator<std::string>(std::cout, "\n"));
}