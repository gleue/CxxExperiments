#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#define LOG_FUNCTION std::cout << " * " << __PRETTY_FUNCTION__ << "\n"

class BaseMessage {

    static std::size_t lastID;

protected:

    std::size_t id;
    std::string recipient{""};

public:

    // Default ctor: set unique message ID
    BaseMessage(): id(++BaseMessage::lastID) { LOG_FUNCTION; }

    // Custom ctor: set unique message ID, custom recipient
    explicit BaseMessage(const std::string& rcpt): id(++BaseMessage::lastID), recipient(std::move(rcpt)) { LOG_FUNCTION; }

    // Copy operations
    BaseMessage(const BaseMessage& msg);
    BaseMessage& operator= (const BaseMessage& msg);

    // Move operations
    BaseMessage(BaseMessage&& msg);
    BaseMessage& operator= (BaseMessage&& msg);

    // Accessors
    inline void resetID() { id = ++BaseMessage::lastID; }
    inline std::string getRecipient() const { return recipient; }
    inline void setRecipient(const std::string& rcpt) { recipient = rcpt; }
    inline void setRecipient(std::string&& rcpt) { recipient = std::move(rcpt); }

    // Output
    friend std::ostream& operator<< (std::ostream& ostr, const BaseMessage& msg);
};

template<class T, std::size_t N>
std::ostream& operator<< (std::ostream& ostr, const std::array<T, N>& arr) {
    for (const auto& a: arr) ostr << a << ' ';
    return ostr;
}

template<class T, std::size_t N>
class Message: public BaseMessage {

public:

    // Default ctor
    Message(): BaseMessage() { LOG_FUNCTION; }

    // Custom ctor: set custom recipient and body
    Message(const std::string& rcpt, const std::array<T, N>& bdy):
        BaseMessage(rcpt),
        body(std::move(bdy)) {
        LOG_FUNCTION;
    }

    // Copy operations
    Message(const Message& msg): BaseMessage(msg), body(msg.body) { LOG_FUNCTION; }
    Message& operator= (const Message& msg) {
        LOG_FUNCTION;
        BaseMessage::operator=(msg);
        body = msg.body;
        return *this;
    }

    // Move operations
    Message(Message&& msg): BaseMessage(std::move(msg)), body(std::move(msg.body)) {
        LOG_FUNCTION;
        msg.body = std::array<T, N>{};
    }
    Message& operator= (Message&& msg) {
        LOG_FUNCTION;
        BaseMessage::operator=(msg);
        body = std::move(msg.body);
        msg.body = std::array<T, N>{};
        return *this;
    }

    // Accessors
    inline void setBody(const std::array<T, N>& bdy) { body = bdy; }
    inline void setBody(std::array<T, N>&& bdy) { body = std::move(bdy); }

    // Output
    friend std::ostream& operator<< (std::ostream& ostr, const Message& msg) {
        ostr << static_cast<const BaseMessage&>(msg) << ": " << msg.body.size() << " bytes [ " << msg.body << ']';
        return ostr;
    }

private:

    std::array<T, N> body{};
};
