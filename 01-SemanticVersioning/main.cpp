#include "SemanticVersion.h"
#include "VersionRange.h"

#include <iostream>
#include <getopt.h>

void usage(const char* argv0) {
    std::cerr << "Usage\n\n  " << argv0 << " [options] <version-string> [<version-string> [<version-string>]]\n\n";
    std::cerr << "Specify one version string argument to print the version components,\n" \
                 "two version strings to compare their relation (<,>,==) and\n" \
                 "three version strings to check if the 3rd version is in range [1. version, 2.version)\n\n";
    std::cerr << "Options\n\n";
    std::cerr << "  -h | --help                     = Print this help\n" \
                 "  -M | --major                    = Output major version component\n" \
                 "  -m | --minor                    = Output minor version component\n" \
                 "  -p | --patchlevel               = Output patch version component\n" \
                 "  -r | --release                  = Output pre-release version component\n" \
                 "  -b | --buildnumber              = Output build number version component\n" \
                 "  -B | --bump (major|minor|patch) = Bump given component\n" \
                 "  -s | --silent                   = Do not print output just return result\n\n";
    std::cerr << "Returns\n\n";
    std::cerr << "  0       = OK or versions are equal or 3rd version is in range\n" \
                 "  1       = 1st version is greater than 2nd or 3rd version is not in range\n" \
                 " -1 (255) = 1st version is smaller than 2nd\n" \
                 "  2       = ERROR: invalid version string\n" \
                 "  3       = ERROR: invalid bump component\n";
    std::cerr << std::endl;
}

bool silent = false;
bool printMajor = false;
bool printMinor = false;
bool printPatch = false;
bool printRelease = false;
bool printBuild = false;

const char *bumpWhat = nullptr;

int parseArguments(int argc, char* argv[]) {

    static const option longOptions[] = {
        { "help", 0, nullptr, 'h'},
        { "major", 0, nullptr, 'M' },
        { "minor", 0, nullptr, 'm' },
        { "patchlevel", 0, nullptr, 'p' },
        { "release", 0, nullptr, 'r' },
        { "buildnumber", 0, nullptr, 'b' },
        { "bump", 1, nullptr, 'B' },
        { "silent", 0, nullptr, 's' },
        { nullptr, 0, nullptr, 0 },
    };

    int opt = -1;

    while ((opt = getopt_long(argc, argv, "hMmprbB:s", longOptions, nullptr)) > 0) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                exit(0);
            case 'M':
                printMajor = true;
                break;
            case 'm':
                printMinor = true;
                break;
            case 'p':
                printPatch = true;
                break;
            case 'r':
                printRelease = true;
                break;
            case 'b':
                printBuild = true;
                break;
            case 'B':
                bumpWhat = optarg;
                break;
            case 's':
                silent = true;
                break;
            case '?':
                usage(argv[0]);
                exit(2);
        }
    }

    return optind;
}

void printVersion(const SemanticVersion& version) {
    if (silent) return;

    bool printed = false;
    bool fullversion = true;

    if (printMajor) {
        std::cout << version.major();
        printed = true;
    }
    if (printMinor) {
        if (printed) std::cout << '.';
        std::cout << version.minor();
        printed = true;
    }
    if (printPatch) {
        if (printed) std::cout << '.';
        std::cout << version.patch();
        printed = true;
    }
    if (printRelease) {
        if (version.hasPreRelease()) {
            if (printed) std::cout << '-';
            std::cout << version.preRelease().value();
            printed = true;
        }
        else
            fullversion = false;
    }
    if (printBuild) {
        if (version.hasBuildMetadata()) {
            if (printed) std::cout << '+';
            std::cout << version.buildMetadata().value();
            printed = true;
        }
        else
            fullversion = false;
    }

    if (!printed && fullversion) {
        std::cout << version;
        printed = true;
    }

    if (printed) std::cout << std::endl;
}

bool bumpVersion(SemanticVersion& version) {
    if (strcmp(bumpWhat,"major") == 0) {
        version.bump(SemanticVersion::VersionComponent::major);
    }
    else if (strcmp(bumpWhat,"minor") == 0) {
        version.bump(SemanticVersion::VersionComponent::minor);
    }
    else if (strcmp(bumpWhat,"patch") == 0) {
        version.bump(SemanticVersion::VersionComponent::patch);
    }
    else {
        return false;
    }

    return true;
}

int compareVersions(const SemanticVersion& v1, const SemanticVersion& v2) {
    std::string op = "==";
    auto rel = (v1 <=> v2);
    int res = 0;

    if (rel < 0) {
        op = "<";
        res = -1;
    }
    else if (rel > 0) {
        op = ">";
        res = +1;
    }

    if (!silent) std::cout << v1 << ' ' << op << ' ' << v2 << std::endl;
    return res;
}

bool checkRange(const RightOpenVersionRange& range, const SemanticVersion& version) {
    auto in = range.contains(version);

    if (!silent) {
        std::string rel = in ? "in" : "not in";

        std::cout << version << ' ' << rel << ' ' << range << std::endl;
    }

    return in;
}

int main(int argc, char* argv[]) {

    auto index = parseArguments(argc, argv);

    if (argc - optind == 0) {
        usage(argv[0]);
        return 1;
    }

    try {
        SemanticVersion v1(argv[optind]);

        if (argc - optind == 1) {
            if (bumpWhat && !bumpVersion(v1)) {
                std::cerr << "Error: Invalid bump component " << bumpWhat << std::endl;
                usage(argv[0]);
                return 3;
            }

            printVersion(v1);
            return 0;
        }

        SemanticVersion v2(argv[optind + 1]);

        if (argc - optind == 2) return compareVersions(v1, v2);

        SemanticVersion v3(argv[optind + 2]);

        return checkRange(RightOpenVersionRange(v1, v2), v3) ? 0 : 1;
    }
    catch (SemanticVersion::MalformedVersionException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }
}
