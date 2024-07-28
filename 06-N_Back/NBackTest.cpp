#include "NBackTest.h"

#include <chrono>
#include <iostream>
#include <termios.h>
#include <vector>

std::ostream& operator<< (std::ostream& ostr, const std::vector<unsigned char> chars) {
    std::for_each(chars.begin(), chars.end(), [&](auto ch){ ostr << ch; });
    return ostr;
}

// Terminal I/O see https://stackoverflow.com/a/912796
//
static struct termios prepareConsole() {
    struct termios old{0};
    
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    
    return old;
}

static char getCharacter() {
    return 0;
}

static void resetConsole(struct termios old) {
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror ("tcsetattr ~ICANON");
}

void NBackTest::run(const std::string& subjectName, unsigned numberOfIterations) {

    auto consoleSettings = prepareConsole();

    // Reserve capacity for answers
    std::vector<unsigned char> answers(numberOfIterations, 0);

    // Note start time
    auto startTime = std::chrono::system_clock::now();

    // Show `iterations` samples and record answers
    for (unsigned iter = 0; iter < numberOfIterations; iter++) {

        // Show next character in test sequence
        auto nextCharacter = sequence.nextCharacter();

        std::cout << nextCharacter << std::flush;

        // Read non-blocking from console for max. `millis` ms
        auto t1 = std::chrono::high_resolution_clock::now();
        unsigned char answer {0};

        do {
            answer = getCharacter();
            switch (answer) {
                case '\x1b': // Escape
                    iter = numberOfIterations;
                    break;
                case '\x20': // Space
                    break;
                default:
                    // Ignore other kexpresses
                    break;
            }
        } while (duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1).count() < millis);
        
        // Wipe character by sending Backspace
        std::cout << '\b' << std::flush;

        // Store answer
        if (answer == '\x20') {
            answers[iter] = 'Y';
        }
        else if (answer == 0) {
            answers[iter] = 'N';
        }
    }

    // Compute result
    unsigned numberOfAnswers{0};
    unsigned numberOfCorrectAnswers{0};
    // NOTE: sequence.length <= answers.size()
    for (std::size_t idx = 0; idx < sequence.length(); idx++) {
        if (!answers[idx]) break;

        ++numberOfAnswers;
        if (sequence.verifyAnswer(idx, answers[idx] == 'Y')) ++numberOfCorrectAnswers;
    }

    float precentageOfCorrectAnswers = float(numberOfCorrectAnswers) / float(numberOfAnswers) * 100.;

    // Show result
    std::cout << numberOfCorrectAnswers << " of " << numberOfAnswers << " answer correct: " << precentageOfCorrectAnswers << "%\n";

    // Write log
    std::cout << "Subject=" << subjectName << '\n'
              << "Duration=" << millis << "ms\n"
              << "N=" << sequence.getN() << " Start=" << startTime << '\n'
              << "Samples=" << sequence.getSequence() << '\n'
              << "Answers=" << answers << '\n';

    resetConsole(consoleSettings);
}
