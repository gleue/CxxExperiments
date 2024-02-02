// Example from https://en.cppreference.com/w/cpp/ranges

#define _LIBCPP_ENABLE_EXPERIMENTAL

#include <algorithm>
#include <cctype>
#include <iostream>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

template<std::ranges::input_range V>
static void printRange(V range, const std::optional<std::string>& heading = std::nullopt) {
    if (heading) std::cout << heading.value() << ":\n";
    for (int idx = 0; auto const num: range) std::cout << idx++ << ": " << num << '\n';
    std::cout << std::endl;
}

template<std::ranges::input_range V>
static void printTuples(V range, const std::optional<std::string>& heading = std::nullopt) {
    if (heading) std::cout << heading.value() << ":\n";
    for (int idx = 0; auto const tpl: range) std::cout << idx++ << ": [" << std::get<0>(tpl) << ',' << std::get<1>(tpl) << "]\n";
    std::cout << std::endl;
}

static void integerTests() {
    // range from container
    std::vector<int> odds{1,3,5,7,9,11};
    printRange(odds, "My odd numbers container");

    // range from factory
    auto factoryOdds = std::views::iota(1,12) | std::views::filter([](auto const num){ return num % 2; });
    printRange(factoryOdds, "My odd numbers view");
    printRange(factoryOdds | std::views::reverse, "The reversed odd numbers view");

    // range from adaptor
    auto threeOdds = std::views::counted(odds.begin(), 3);
    printRange(threeOdds | std::views::transform([](auto const num){ return num + 1; }), "The first three odd numbers incremented");
}

static void stringTests() {
    using std::operator""sv;

    auto csv{"a,b,c,,d,e;f,g"sv};
    auto fieldViews = csv | std::views::split(',') | std::views::transform([](auto const val){ return std::string_view(val); });
    printRange(fieldViews, "String view split at commas");

    std::string csvString{"a,b,c,,d,e;f,g"};
    auto fields = csvString | std::views::split(',') | std::views::transform([](auto const f){ auto sv = std::string_view(f); return sv.empty() ? "-" : sv; });
    printRange(fields, "String split at commas and replaced when empty");

    auto str{"a1b2d3e4f5g6"};
    std::string_view strView = str;
    auto digits = strView | std::views::filter([](auto const c){ return std::isdigit(c); });
    printRange(digits, "Digits filtered from a string");

    std::string digraphs[]{"ab", "bc", "cd"};
    printRange(digraphs | std::views::all, "An array of digraphs");
    auto joined = std::ranges::join_view(digraphs);
    printRange(joined, "The same digraphs joined into a flat range");
}   

static void tupleTests() {

    std::tuple<std::string, int> languageVersions[]
    {
        {"c", 95}, {"c", 99}, {"c", 11}, {"c++", 98}, {"c++", 11}, {"c++", 14}, {"c++", 17}, {"c++", 20}, {"c++", 23}
    };
    printTuples(languageVersions | std::views::all, "The original array of tuples");

    // sort tuples w/ projections
    std::ranges::sort(languageVersions, std::greater(), [](auto const lang){ return std::get<1>(lang); });
    printTuples(languageVersions | std::views::all, "Tuples sorted by descending version");

    // sort extracted tuple elements
    auto versions = languageVersions | std::views::elements<1>;
    // default is less
    std::ranges::sort(versions);
    printRange(versions, "Tuple versions sorted ascending");
    // w/ greater comparison
    // std::ranges::sort(versions, std::greater());
    // printRange(versions, "Tuple versions sorted descending");

    // sort tuples w/ projections
    std::ranges::sort(languageVersions, std::greater(), [](auto const lang){ return std::get<1>(lang); });
    printTuples(languageVersions | std::views::all, "Tuples sorted by descending version w/ closure");

    // sort tuples w/ projections
    std::ranges::sort(languageVersions, {}, [](auto const lang){ return std::get<0>(lang); });
    printTuples(languageVersions | std::views::all, "Tuples sorted by ascending name w/ closure");
}

struct Language {
    std::string name;
    int version;

    friend std::ostream& operator<<(std::ostream& ostr, const Language& lang);
};

std::ostream& operator<<(std::ostream& ostr, const Language& lang) {
    ostr << lang.name << " @ " << lang.version;
    return ostr;
}

static void structTests() {
    std::vector<Language> languageVersions
    {
        {"c", 95}, {"c", 99}, {"c", 11}, {"c++", 98}, {"c++", 11}, {"c++", 14}, {"c++", 17}, {"c++", 20}, {"c++", 23}
    };
    printRange(languageVersions | std::views::all, "The original languages");

    // sort structs w/ projection
    std::ranges::sort(languageVersions, {}, &Language::version );
    printRange(languageVersions | std::views::all, "Languages sorted by ascending version w/ projection");

    // sort structs w/ projections
    std::ranges::sort(languageVersions, std::greater(), [](auto const lang){ return lang.version; } );
    printRange(languageVersions | std::views::all, "Languages sorted by descending version w/ closure projection");

    auto modernLanguages = languageVersions | std::views::filter([](auto const lang){ return lang.version <= 23; });
    printRange(modernLanguages, "Filtered languages");
    languageVersions.push_back({"ada", 05});
    languageVersions.push_back({"fortran", 77});
    printRange(modernLanguages, "Filtered languages w/ extra element(s) in underlying container");
    printRange(languageVersions | std::views::all, "All languages");
}

int main() {
    integerTests();
    stringTests();
    tupleTests();
    structTests();
}
