#include "NBackSequence.h"

unsigned char NBackSequence::nextCharacter() {

    // TODO: Generate sample with random repetition
    unsigned char next = 'a';
    
    sequence.push_back(next);
    return next;
}

bool NBackSequence::verifyAnswer(std::size_t index, bool wasNBack) {
    if (index < n || index >= sequence.size()) return false;

    auto sample = sequence[index];
    auto back = sequence[index - n];

    return (wasNBack && sample == back) || (!wasNBack && sample != back);
}
