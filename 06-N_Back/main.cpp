#include "NBackTest.h"

#include <cstring>
#include <iostream>
#include <getopt.h>

#define DEFAULT_LENGTH 20
#define DEFAULT_MILLIS 500
#define DEFAULT_PROBAB 0.1

std::string name{};
unsigned iterations = DEFAULT_LENGTH;
unsigned milliseconds = DEFAULT_MILLIS;
float probability = DEFAULT_PROBAB;

void usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " [options] <N>\n\n";
    std::cerr << "Runs an n-back test with given N\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help                      = Print this help\n" \
                 "  -l | --length <number of chars>  = Length of character sequence for test (default = " << DEFAULT_LENGTH << ")\n" \
                 "  -d | --duration <milliseconds>   = Number of milliseconds to show stimuli (default = " << DEFAULT_MILLIS << ")\n" \
                 "  -p | --probability <real number> = Probability of repetition (default = " << DEFAULT_PROBAB << ")\n" \
                 "  -n | --name <subject name>       = Name of test subject name\n";
    std::cerr << std::endl;
}

int parseArguments(int argc, char* const argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "length", 1, nullptr, 'l' },
        { "duration", 1, nullptr, 'd' },
        { "probability", 1, nullptr, 'p' },
        { "name", 1, nullptr, 'n' },
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "hl:d:p:n:", longOptions, nullptr)) > 0) {
        switch (opt) {
        case 'h':
            usage(argv[0]);
            exit(0);
        case 'l':
            iterations = atoi(optarg);
            break;
        case 'd':
            milliseconds = atoi(optarg);
            break;
        case 'p':
            probability = atof(optarg);
            if (probability < .0) probability = 0.0; else if (probability > 1.0) probability = 1.;
            break;
        case 'n':
            name = optarg;
            break;
        case '?':
            usage(argv[0]);
            exit(1);
        }
    }

    return optind;
}

int main(int argc, char* const argv[]) {

    auto index = parseArguments(argc, argv);

    if (argc - optind == 0) {
        usage(argv[0]);
        return 1;
    }

    NBackTest test(atoi(argv[optind]), milliseconds, probability);
    test.run(name, iterations);
    return 0;
}
