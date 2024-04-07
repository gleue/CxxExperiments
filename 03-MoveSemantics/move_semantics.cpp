#include "Message.h"

#include <array>
#include <deque>
#include <iostream>
#include <string>

int main() {

    // Copied/moved Messages

    std::cout << "\n= Empty message:\n";
    Message<char, 0> emptyMsg;
    std::cout << emptyMsg << std::endl;

    std::cout << "\n= Original message:\n";
    std::array<int, 10> msgBody{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Message<int, 10> originalMessage("tim", msgBody);
    std::cout << originalMessage << std::endl;

    std::cout << "\n= Copied message:\n";
    auto copiedMessage{originalMessage};
    std::cout << copiedMessage << std::endl;
    std::cout << "- Original message:\n";
    std::cout << originalMessage << std::endl;

    std::cout << "\n= Copy assigned message:\n";
    decltype(originalMessage) copyAssignedMessage;
    copyAssignedMessage = originalMessage;
    std::cout << copyAssignedMessage << std::endl;
    std::cout << "- Original message:\n";
    std::cout << originalMessage << std::endl;

    std::cout << "\n= Moved message:\n";
    auto movedMessage{std::move(originalMessage)};
    std::cout << movedMessage << std::endl;
    std::cout << "- Original message:\n";
    std::cout << originalMessage << std::endl;

    std::cout << "\n= Move from message:\n";
    std::array<int, 10> altBody{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    originalMessage.resetID();
    originalMessage.setRecipient(std::move("foo"));
    originalMessage.setBody(std::move(altBody));
    std::cout << originalMessage << std::endl;

    // Message Queues

    std::deque<Message<int, 10>> messageQueue{};

    std::cout << "\n= Enqueue messages:\n";
    messageQueue.emplace_back(std::move(movedMessage));
    std::cout << "- Original moved message:\n";
    std::cout << movedMessage << std::endl;
    messageQueue.emplace_back(std::move(originalMessage));
    std::cout << "- Original message:\n";
    std::cout << originalMessage << std::endl;

    std::cout << "\n= Dequeue messages:\n";
    const auto& firstMessage = messageQueue.front(); messageQueue.pop_front();
    std::cout << "- First message:\n";
    std::cout << firstMessage << std::endl;
    const auto& nextMessage = messageQueue.front(); messageQueue.pop_front();
    std::cout << "- Next message:\n";
    std::cout << nextMessage << std::endl;
}
