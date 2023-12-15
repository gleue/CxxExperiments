#include "VersionRange.h"

#include <stdexcept>

VersionRange::VersionRange(const SemanticVersion& min, const SemanticVersion& max) :
    minVersion(min),
    maxVersion(max) {

    if (minVersion > maxVersion) throw std::invalid_argument("Invalid version range bounds");
}

bool RightOpenVersionRange::contains(const SemanticVersion& version) const {
    if (minVersion.hasPreRelease() || maxVersion.hasPreRelease()) {
        return version >= minVersion && version < maxVersion;
    } else {
        // Ignore pre-release if bounds don't have them
        return version.versionCore() >= minVersion.versionCore() && version.versionCore() < maxVersion.versionCore();
    }
}

std::ostream& operator<<(std::ostream& ostr, const RightOpenVersionRange& rng) {
    return ostr << '[' << rng.minVersion << ',' << rng.maxVersion << ')';
}
