#include "NBackSequence.h"

#include <random>

unsigned char NBackSequence::nextCharacter() {

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> fdist(0., 1.);

    unsigned char next;

    if (sequence.size() < n || fdist(generator) > probability) {
        // Add random character avoiding repetitions
        std::uniform_int_distribution<> idist(0, 25);

        do {
            next = 'A' + idist(generator);
        } while (sequence.size() > 0 && next == sequence.back());
    } else {
        // Repeat n-back character
        next = sequence[sequence.size() - n];
    }
    
    sequence.push_back(next);
    return next;
}

bool NBackSequence::verifyAnswer(std::size_t index, bool wasNBack) const {
    if (index < n) return true;
    if (index >= sequence.size()) return false;

    auto sample = sequence[index];
    auto back = sequence[index - n];

    return (wasNBack && sample == back) || (!wasNBack && sample != back);
}
