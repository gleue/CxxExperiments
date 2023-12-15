#include "SemanticVersion.h"

#include <iomanip>
#include <iostream>
#include <sstream>

//SemanticVersion::MalformedVersionException(SemanticVersion::VersionComponent versionComponent, const std::string& versionString) : std::invalid_argument("Malformed version string: " + versionString), component(versionComponent) {}

std::string SemanticVersion::MalformedVersionException::createWhatString(SemanticVersion::VersionComponent versionComponent, const std::string& versionString) {
    std::ostringstream str;
    str << versionComponent;

    auto comp = str.str() + (str.str().empty() ? "" : " ");

    return "Malformed " + comp + "version: " + versionString;
}

SemanticVersion::SemanticVersion(const std::string& versionString) {
    const std::string allowedVersionChars = "0123456789";
    const std::string allowedIdentifierChars = "ABCDEFGHIJ" \
                                               "KLMNOPQRST" \
                                               "UVWXYZ" \
                                               "abcdefghij" \
                                               "klmnopqrst" \
                                               "uvwxyz" \
                                               "0123456789" \
                                               "-.";
    std::string version = versionString;

    // Optional trailing build metadata
    if (auto start = version.find('+'); start != version.npos) {
        auto metadata = version.substr(start + 1, version.npos);
        if (metadata.find_first_not_of(allowedIdentifierChars) != std::string_view::npos) throw MalformedVersionException(VersionComponent::buildMetadata, metadata);

        _buildMetadata = metadata;
        _buildMetadataIdentifiers = parseDotSeparatedIdentifierList(_buildMetadata);

        version.erase(start, version.npos);
    }

    // Optional trailing pre-release
    if (auto start = version.find('-'); start != version.npos) {
        auto prerelease = version.substr(start + 1, version.npos);
        if (prerelease.find_first_not_of(allowedIdentifierChars) != std::string_view::npos) throw MalformedVersionException(VersionComponent::preRelease, prerelease);

        _preRelease = prerelease;
        _preReleaseIdentifiers = parseDotSeparatedIdentifierList(_preRelease);

        version.erase(start, version.npos);
    }

    // Mandatory major.minor.patch
    auto dot = version.find('.');
    if (dot == 0 || dot == version.npos) throw MalformedVersionException(VersionComponent::all, versionString);

    {
        auto major = version.substr(0, dot);
        if (major.find_first_not_of(allowedVersionChars) != std::string_view::npos) throw MalformedVersionException(VersionComponent::major, major);

        std::istringstream str(major);

        str >> _versionCore.major;
        if (_versionCore.major > 0 && major.starts_with("0")) throw MalformedVersionException(VersionComponent::major, major);
    }

    version.erase(0, dot + 1);
    dot = version.find('.');
    if (dot == 0 || dot == version.npos) throw MalformedVersionException(VersionComponent::all, versionString);

    {
        auto minor = version.substr(0, dot);
        if (minor.find_first_not_of(allowedVersionChars) != std::string_view::npos) throw MalformedVersionException(VersionComponent::minor, minor);

        std::istringstream str(minor);

        str >> _versionCore.minor;
        if (_versionCore.minor > 0 && minor.starts_with("0")) throw MalformedVersionException(VersionComponent::minor, minor);
    }

    version.erase(0, dot + 1);
    dot = version.find('.');
    if (dot == 0) throw MalformedVersionException(VersionComponent::all, versionString);

    {
        auto patch = version.substr(0, dot);
        if (patch.find_first_not_of(allowedVersionChars) != std::string_view::npos) throw MalformedVersionException(VersionComponent::patch, patch);

        std::istringstream str(patch);

        str >> _versionCore.patch;
        if (_versionCore.patch > 0 && patch.starts_with("0")) throw MalformedVersionException(VersionComponent::patch, patch);
    }
}

void SemanticVersion::VersionCore::bump(SemanticVersion::VersionComponent component) {
    switch (component) {
        case VersionComponent::major:
            major += 1;
            minor = 0;
            patch = 0;
            break;
        case VersionComponent::minor:
            minor += 1;
            patch = 0;
            break;
        case VersionComponent::patch:
            patch += 1;
            break;
        default:
            throw std::invalid_argument("Invalid bump component");
    }
}

std::strong_ordering SemanticVersion::operator<=> (const SemanticVersion& rhs) const {

    // "Precedence refers to how versions are compared to each other when ordered."
    //
    // "1. Precedence MUST be calculated by separating the version into major, minor, patch and
    //     pre-release identifiers in that order (Build metadata does not figure into precedence)."
    // "2. Precedence is determined by the first difference when comparing each of these identifiers
    //     from left to right as follows: Major, minor, and patch versions are always compared numerically."
    if (versionCore() < rhs.versionCore()) {
        return std::strong_ordering::less;
    }
    else if (versionCore() > rhs.versionCore()) {
        return std::strong_ordering::greater;
    }

    // "3. When major, minor, and patch are equal, a pre-release version has lower precedence than a normal version."
    else if (hasPreRelease() && !rhs.hasPreRelease()) {
        return std::strong_ordering::less;
    }
    else if (!hasPreRelease() && rhs.hasPreRelease()) {
        return std::strong_ordering::greater;
    }
    
    // "4. Precedence for two pre-release versions with the same major, minor, and patch version MUST be determined
    //     by comparing each dot separated identifier from left to right until a difference is found as follows:
    //
    //     1. Identifiers consisting of only digits are compared numerically.
    //     2. Identifiers with letters or hyphens are compared lexically in ASCII sort order.
    //     3. Numeric identifiers always have lower precedence than non-numeric identifiers.
    //     4. A larger set of pre-release fields has a higher precedence than a smaller set,
    //        if all of the preceding identifiers are equal.
    for (size_t i = 0; ; i += 1) {
        if (i >= _preReleaseIdentifiers.size() && i >= rhs._preReleaseIdentifiers.size()) return std::strong_ordering::equal;

        // 4.4:
        if (i >= _preReleaseIdentifiers.size() && i < rhs._preReleaseIdentifiers.size()) return std::strong_ordering::less;
        if (i < _preReleaseIdentifiers.size() && i >= rhs._preReleaseIdentifiers.size()) return std::strong_ordering::greater;

        auto lhi = _preReleaseIdentifiers[i];
        auto rhi = rhs._preReleaseIdentifiers[i];

        if (lhi.isNumeric && !rhi.isNumeric) {
            // 4.3:
            return std::strong_ordering::less;
        }  else if (!lhi.isNumeric && rhi.isNumeric) {
            // 4.3:
            return std::strong_ordering::greater;
        } else if (lhi.text != rhi.text) {
            // 4.2/4.1:
            return lhi.text <=> rhi.text;
        }
    }

    return std::strong_ordering::equal;
}

std::ostream& operator<<(std::ostream& ostr, const SemanticVersion::VersionCore& ver) {
    return ostr << ver.major << '.' << ver.minor << '.' << ver.patch;
}

std::ostream& operator<<(std::ostream& ostr, const SemanticVersion& ver) {
    ostr << ver.versionCore();
    if (ver.hasPreRelease()) ostr << '-' << ver.preRelease().value();
    if (ver.hasBuildMetadata()) ostr << '+' << ver.buildMetadata().value();
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const SemanticVersion::VersionComponent& comp) {
    switch (comp) {
        case SemanticVersion::VersionComponent::major:
            ostr << "major";
            break;
        case SemanticVersion::VersionComponent::minor:
            ostr << "minor";
            break;
        case SemanticVersion::VersionComponent::patch:
            ostr << "patch";
            break;
        case SemanticVersion::VersionComponent::preRelease:
            ostr << "pre-release";
            break;
        case SemanticVersion::VersionComponent::buildMetadata:
            ostr << "build-metadata";
            break;
        default:
            // NOP
            break;
    }
    return ostr;
}

std::vector<SemanticVersion::Identifier> SemanticVersion::parseDotSeparatedIdentifierList(const std::optional<std::string>& dotSeparatedIdentifiers) {
    std::vector<SemanticVersion::Identifier> identifierList;

    // No value at all
    if (!dotSeparatedIdentifiers) return identifierList;

    const auto& identifierString = dotSeparatedIdentifiers.value();
    size_t start = 0;
    decltype(start) dot;

    do {
        dot = identifierString.find('.', start);

        std::string_view identifier = identifierString;
        if (start > 0) identifier.remove_prefix(start);
        if (dot != std::string::npos) identifier.remove_suffix(identifierString.size() - dot);
        bool isNumeric = (identifier.find_first_not_of("0123456789") == std::string_view::npos);

        identifierList.push_back({ identifier, isNumeric });

        start = dot + 1;
    } while (dot != std::string::npos);

    //for (const auto& ident: identifierList) std::cout << ident.text << std::endl;

    return identifierList;
}
