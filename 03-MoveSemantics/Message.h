#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define LOG_FUNCTION std::cout << "* " << __PRETTY_FUNCTION__ << "\n"

class BaseMessage {

  static std::size_t lastID;

protected:

  std::size_t id;
  std::string recipient{};

public:

    // Default ctor: set unique message ID
    //
    // Needed since we also have a move constructor.
    BaseMessage(): id(++BaseMessage::lastID) { LOG_FUNCTION; }

    // Custom ctor: set unique message ID, custom recipient
    explicit BaseMessage(const std::string& rcpt): id(++BaseMessage::lastID), recipient(rcpt) { LOG_FUNCTION; }
    explicit BaseMessage(std::string&& rcpt): id(++BaseMessage::lastID), recipient(std::move(rcpt)) { LOG_FUNCTION; }

    // Copy operations
    //
  // Have to be user-declared, too, since we also
  // have user-declared move operations (see below),
  // and they would otherwise be deleted.
  BaseMessage(const BaseMessage& msg);
  BaseMessage& operator= (const BaseMessage& msg);

  // Move operations
  //
  // They are both user-declared since we want to
  // reset the moved from message's members.
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

    std::array<T, N> body{};

public:

    // Default ctor
    //
    // Needed since we also have a move constructor.
    Message(): BaseMessage() { LOG_FUNCTION; }

    // Custom ctor: set custom recipient and body
    //
    // Since we'd need a total of four constructors to
    // cover all combinations of lvalue and rvalue references
    // we pass arguments by value and move them.
    Message(std::string rcpt, std::array<T, N> bdy):
        BaseMessage(std::move(rcpt)),
        body(std::move(bdy)) {
        LOG_FUNCTION;
  }

  // Copy operations
  //
    // Have to be user-declared, too, since we also
    // have user-declared move operations (see below),
    // and they would otherwise be deleted.
    Message(const Message& msg): BaseMessage(msg), body(msg.body) { LOG_FUNCTION; }
    Message& operator= (const Message& msg) {
        BaseMessage::operator=(msg);
        LOG_FUNCTION;
    body = msg.body;
    return *this;
  }

  // Move operations
    //
    // They are both user-declared since we want to
    // reset the message body in the moved from message.
    Message(Message&& msg): BaseMessage(std::move(msg)), body(std::move(msg.body)) {
        LOG_FUNCTION;
        msg.body = std::array<T, N>{};
    }
  Message& operator= (Message&& msg) {
    BaseMessage::operator=(msg);
    LOG_FUNCTION;
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
};
