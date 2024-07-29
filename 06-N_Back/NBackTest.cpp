#include "NBackTest.h"

#include <chrono>
#include <iostream>
#include <termios.h>
#include <unistd.h>

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
    old.c_cc[VMIN] = 0;
    old.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    
    return old;
}

static void resetConsole(struct termios old) {
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror ("tcsetattr ~ICANON");
}

static char readConsole() {
    char buffer{0};
    if (read(0, &buffer, 1) < 0) perror("read");
    return buffer;
}

void NBackTest::run(const std::string& subjectName, unsigned numberOfStimuli) {

    auto consoleSettings = prepareConsole();

    // Reserve capacity for answers
    std::vector<unsigned char> answers(numberOfStimuli, 0);

    // Note start time
    auto startTime = std::chrono::system_clock::now();

    // Show stimuli and record answers
    performTest(answers);

    // Compute result
    unsigned numberOfAnswers{0};
    unsigned numberOfCorrectAnswers{0};
    std::vector<unsigned char> hits(numberOfStimuli, 0);

    // NOTE: sequence.length <= answers.size()
    for (std::size_t idx = 0; idx < sequence.length(); idx++) {
        if (!answers[idx]) break;

        ++numberOfAnswers;
        if (sequence.verifyAnswer(idx, answers[idx] == 'Y')) {
            ++numberOfCorrectAnswers;
            hits[idx] = 'x';
        }
        else {
            hits[idx] = '-';
        }
    }

    float precentageOfCorrectAnswers = (numberOfAnswers > 0) ? (numberOfCorrectAnswers) / float(numberOfAnswers) * 100. : 0.0;

    // Show result
    std::cout << "Result: " << numberOfCorrectAnswers << " of " << numberOfAnswers << " answers correct (" << precentageOfCorrectAnswers << "%)\n";

    // Write log
    std::cout << "\n*** LOG ***\n"
              << "Subject=" << subjectName << '\n'
              << "Duration=" << millis << "ms\n"
              << "N=" << sequence.getN() << " Start=" << startTime << '\n'
              << "Samples=" << sequence.getSequence() << '\n'
              << "Answers=" << answers << '\n'
              << "Correct=" << hits << '\n';

    resetConsole(consoleSettings);
}

bool NBackTest::performTest(std::vector<unsigned char>& answers) {

    for (unsigned stim = 0; stim < answers.size(); stim++) {

        // Show next character in test sequence
        auto nextCharacter = sequence.nextCharacter();

        std::cout << nextCharacter << std::flush;

        // Read non-blocking from console for max. `millis` ms
        auto t1 = std::chrono::high_resolution_clock::now();
        unsigned char answer {0};

        do {
            answer = readConsole();
            // Ignore any input but `ESC` and `SPACE`
            if (answer == '\x1b' || answer == '\x20') break;
        } while (duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1).count() < millis);
        
        // Wipe character
        std::cout << "\b \b" << std::flush;

        // Store answer or end test
        switch (answer) {
        case '\x1b':
            return false;
        case '\x20':
            answers[stim] = 'Y';
            break;
        case '\x00':
            answers[stim] = 'N';
            break;
        }

        // TODO: We could nap between stimuli
    }

    return true;
}
