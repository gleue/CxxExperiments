#pragma once

#include "NBackSequence.h"

#include <string>
#include <vector>

class NBackTest final {

    std::size_t millis{0};

    NBackSequence sequence{};

public:

    NBackTest(unsigned short n, std::size_t duration, float probability): millis{duration}, sequence{n} {
        sequence.setRepetitionProbability(probability);
    }

    void run(const std::string& subjectName, unsigned numberOfStimuli);

    void reset() { sequence.reset(); }

private:

    bool performTest(std::vector<unsigned char>& answers);
};
