#pragma once

#include <vector>

class NBackSequence final {

    unsigned short n;

    float probability{0.01};
    std::vector<unsigned char> sequence{};

public:

    NBackSequence(unsigned short N = 0): n{N} {}

    std::vector<unsigned char> getSequence() const { return sequence; }
    unsigned short getN() const { return n; }

    void setRepetitionProbability(float prob) { probability = prob; }
    float getRepetitionProbability() const { return probability; }

    std::size_t length() const { return sequence.size(); }

    unsigned char nextCharacter();

    bool verifyAnswer(std::size_t index, bool wasNBack) const;

    void reset() { sequence = std::vector<unsigned char>{}; }
};
