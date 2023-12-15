#pragma once

#include "SemanticVersion.h"

#include <ostream>

struct VersionRange {
    SemanticVersion minVersion;
    SemanticVersion maxVersion;

    VersionRange(const SemanticVersion& min, const SemanticVersion& max);

    virtual bool contains(const SemanticVersion& version) const = 0;
};

struct RightOpenVersionRange: public VersionRange {

    RightOpenVersionRange(const SemanticVersion& min, const SemanticVersion& max) : VersionRange(min, max) {}

    bool contains(const SemanticVersion& version) const override;

    friend std::ostream& operator<<(std::ostream& ostr, const RightOpenVersionRange& rng);
};
