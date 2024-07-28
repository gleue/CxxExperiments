#pragma once

#include "NBackSequence.h"

#include <string>

class NBackTest final {

    std::size_t millis{0};

    NBackSequence sequence{};

public:

    NBackTest(unsigned short n, std::size_t duration): millis{duration}, sequence{n} {}

    void run(const std::string& name, unsigned iterations);

    void reset() { sequence.reset(); }
};
