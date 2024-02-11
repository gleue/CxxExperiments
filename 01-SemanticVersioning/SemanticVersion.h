/**
 * Semantic Versioning
 * 
 * see https://semver.org
 * 
 */

#pragma once

#include <compare>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

class SemanticVersion final {

    struct Identifier {
        std::string_view text;
        bool isNumeric;

        std::strong_ordering operator<=> (const Identifier& rhs) const;
    };
    
public:

    enum class VersionComponent {
        all,
        major,
        minor,
        patch,
        preRelease,
        buildMetadata
    };

    struct MalformedVersionException: std::invalid_argument {

        MalformedVersionException(VersionComponent versionComponent, const std::string& versionString) : std::invalid_argument(createWhatString(versionComponent, versionString)) {}

    private:

        static std::string createWhatString(VersionComponent versionComponent, const std::string& versionString);
    };

    struct VersionCore {
        unsigned major = 0;
        unsigned minor = 0;
        unsigned patch = 0;

        bool operator== (const VersionCore& rhs) const = default;
        auto operator<=> (const VersionCore& rhs) const = default;

        void bump(VersionComponent component);

        friend std::ostream& operator<<(std::ostream& ostr, const VersionCore& ver);
    };

    constexpr SemanticVersion(unsigned major, unsigned minor = 0, unsigned patch = 0, const std::optional<std::string>& preRelease = std::nullopt, const std::optional<std::string>& buildMetadata = std::nullopt) :
    _versionCore({major, minor, patch}),
    _preRelease(preRelease),
    _buildMetadata(buildMetadata) {
    }

    SemanticVersion(const std::string& versionString);

    inline bool hasPreRelease() const { return _preRelease.has_value(); }
    inline bool hasBuildMetadata() const { return _buildMetadata.has_value(); }

    inline const unsigned major() const { return _versionCore.major; }
    inline const unsigned minor() const { return _versionCore.minor; }
    inline const unsigned patch() const { return _versionCore.patch; }

    inline const VersionCore& versionCore() const { return _versionCore; }
    inline const std::optional<std::string>& preRelease() const { return _preRelease; }
    inline const std::optional<std::string>& buildMetadata() const { return _buildMetadata; }

    bool operator== (const SemanticVersion& rhs) const = default;
    std::strong_ordering operator<=> (const SemanticVersion& rhs) const;

    inline void bump(VersionComponent component) { _versionCore.bump(component); }

    friend std::ostream& operator<<(std::ostream& ostr, const SemanticVersion& ver);

private:

    VersionCore _versionCore;
    std::optional<std::string> _preRelease;
    std::vector<Identifier> _preReleaseIdentifiers;
    std::optional<std::string> _buildMetadata;
    std::vector<Identifier> _buildMetadataIdentifiers;

    static std::vector<Identifier> parseDotSeparatedIdentifierList(const std::optional<std::string>& dotSeparatedIdentifiers);
};

std::ostream& operator<<(std::ostream& ostr, const SemanticVersion::VersionComponent& comp);
