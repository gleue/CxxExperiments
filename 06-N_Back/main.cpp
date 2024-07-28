#include "NBackTest.h"

#include <cstring>
#include <iostream>
#include <getopt.h>

std::string name{};
unsigned iterations = 10;
unsigned milliseconds = 500;

void usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " [options] <N>\n\n";
    std::cerr << "Runs an n-back test with given N\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help                     = Print this help\n" \
                 "  -l | --length <number of chars> = Length of character sequence for test (default = 10)\n" \
                 "  -d | --duration <milliseconds>  = Number of milliseconds to show stimuli (default = 500)\n" \
                 "  -n | --name <subject name>      = Name of test subject name\n";
    std::cerr << std::endl;
}

int parseArguments(int argc, char* const argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "length", 1, nullptr, 'l' },
        { "duration", 1, nullptr, 'd' },
        { "name", 1, nullptr, 'n' },
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "hl:d:n:", longOptions, nullptr)) > 0) {
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

    NBackTest test(atoi(argv[optind]), milliseconds);

    test.run(name, iterations);
    return 0;
}
