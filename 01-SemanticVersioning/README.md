# Semantic Versioning

This exercise started as an investigation of [three-way comparisons](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison) (aka the "spaceship operator") in C++20.

Comparing version infos seemed a good use case. See [semver.org](https://semver.org) for more infos on semantic versioning.

## Requirements

- C++ compiler supporting C++20 (I use [clang](https://clang.llvm.org), others should work but have not been tested)
- [CMake](https://cmake.org) >= 4.0

## Building

- see [../README.md](../README.md)

## Running

- Run the `semver` tool
  ```console
  $ <build-dir>/01-SemanticVersioning/semver --help
  ```

Use `semver --help` to get an overview of the command line options.

There are four modes of operation.

### 1. Parse and print version components

Use options to specify which version component should be printed. If no options are given all are printed.
```console
$ semver [-Mmprb] <version>
```
Returns 0 if the `<version>` is a proper semantic version according to the [specs](https://semver.org/#semantic-versioning-specification-semver), 2 otherwise.

**Examples:**
```console
$ semver -M 1.2.3
1

$ semver -Mm 1.2.3
1.2

$ semver -r 1.2.3-alpha
alpha

$ semver -b 1.2.3-alpha+1234
1234

$ semver -Mmpr 1.2.3-alpha+1234
1.2.3-alpha
```

### 2. Bump version component

Increase a given component by 1 and reset the subordinate components to 0:

Use options to specify which version component should be printed. If no options are given all are printed.
```console
$ semver -B (major|minor|patch) [-Mmprb] <version>
```
Returns 0 if the `<version>` is a proper semantic version according to the [specs](https://semver.org/#semantic-versioning-specification-semver), 2 otherwise and 3 if an invalid component is specified.

**Examples:**
```console
$ semver -B major 1.2.3
2.0.0

$ semver -B minor 1.2.3
1.3.0

$ semver -B patch 1.2.3
1.2.4
```

### 3. Compare two versions

Again options may be used to specify which components should appear in the output.
```console
$ semver [-Mmprb] <version-1> <version-2>
```
Returns 0 if both version strings have equal precedence, 1 if `version-1` is greater than `version-2` and -1 (255) otherwise. Returns 2 when either argument is not a proper version according to the [specs](https://semver.org/#semantic-versioning-specification-semver).

**Examples:**
```console
$ semver 1.2.3 1.2.4
1.2.3 < 1.2.4

$ semver 1.2.3-beta 1.2.3-alpha
1.2.3-beta > 1.2.3-alpha

$ semver 1.2.3-alpha+1234 1.2.3-alpha+5678
1.2.3-alpha+1234 == 1.2.3-alpha+5678
```

### 4. Check if a given version is in a version range

Again options may be used to specify which components should appear in the output.
```console
$ semver [-Mmprb] <min-version> <max-version> <check-version>
```
Returns 0 if `<check-version>` is in range `[min-version,max-version)` and 1 otherwise. Returns 2 when either argument is not a proper version according to the [specs](https://semver.org/#semantic-versioning-specification-semver).

**Examples:**
```console
$ semver 1.2.3 2.0.0 1.5.6
1.5.6 in [1.2.3,2.0.0)

$ semver 1.2.3 2.0.0 2.1.0
2.1.0 not in [1.2.3,2.0.0)
```

## Known issues / Possible improvements

- Use regex to parse version strings
