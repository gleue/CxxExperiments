#pragma once

#include <vector>

class NBackSequence final {

    unsigned short n;

    std::vector<unsigned char> sequence{};

public:

    NBackSequence(unsigned short N = 0): n{N} {}

    unsigned char nextCharacter();

    std::size_t length() const { return sequence.size(); }
    std::vector<unsigned char> getSequence() const { return sequence; }
    unsigned short getN() const { return n; }

    bool verifyAnswer(std::size_t index, bool wasNBack);

    void reset() { sequence = std::vector<unsigned char>{}; }
};
