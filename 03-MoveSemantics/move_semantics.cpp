#include "Message.h"

#include <array>
#include <deque>
#include <iostream>
#include <string>

int main() {

    // Copied/moved Messages

    std::cout << "\nDefault construction:\n";
    Message<char, 0> emptyMsg;
    std::cout << "\n=> Empty message: " << emptyMsg << std::endl;

    std::cout << "\nCustom construction:\n";
    std::array<int, 10> msgBody{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Message<int, 10> originalMessage("tim", msgBody);
    std::cout << "\n=> Original message: " << originalMessage << std::endl;

    std::cout << "\nCopy construction:\n";
    auto copiedMessage{originalMessage};
    std::cout << "\n=> Copied message: " << copiedMessage << std::endl;
    std::cout << "-> Original message: " << originalMessage << std::endl;

    std::cout << "\nDefault construction:\n";
    decltype(originalMessage) copyAssignedMessage;
    std::cout << "\nCopy assigment:\n";
    copyAssignedMessage = originalMessage;
    std::cout << "\n=> Copy assigned message: " << copyAssignedMessage << std::endl;
    std::cout << "-> Original message: " << originalMessage << std::endl;

    std::cout << "\nMove construction:\n";
    auto movedMessage{std::move(originalMessage)};
    std::cout << "\n=> Moved message: " << movedMessage << std::endl;
    std::cout << "-> Original message:" << originalMessage << std::endl;

    std::cout << "\nRe-initialization of moved from object:\n";
    std::array<int, 10> altBody{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    originalMessage.resetID();
    originalMessage.setRecipient(std::move("foo"));
    originalMessage.setBody(std::move(altBody));
    std::cout << "=> Moved from message: " << originalMessage << std::endl;

    // Message Queues

    std::deque<Message<int, 10>> messageQueue{};

    std::cout << "\nEnqueing messages:\n";
    messageQueue.emplace_back(std::move(movedMessage));
    std::cout << "\n=> Enqueued message (moved): " << movedMessage << std::endl << std::endl;
    messageQueue.emplace_back(originalMessage);
    std::cout << "\n=> Enqueued message (copied): " << originalMessage << std::endl;

    std::cout << "\nDequeing messages:\n";
    auto firstMessage = messageQueue.front(); messageQueue.pop_front();
    std::cout << "\n=> Dequeued first message: " << firstMessage << std::endl << std::endl;
    auto nextMessage = messageQueue.front(); messageQueue.pop_front();
    std::cout << "\n-> Dequeued next message: " << nextMessage << std::endl;
}
