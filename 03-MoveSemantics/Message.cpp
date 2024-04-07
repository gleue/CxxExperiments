#include "Message.h"

std::size_t BaseMessage::lastID{0};

BaseMessage::BaseMessage(const BaseMessage& msg):
    id(msg.id),
    recipient(msg.recipient) {
    LOG_FUNCTION;
}

BaseMessage& BaseMessage::operator= (const BaseMessage& msg) {
    LOG_FUNCTION;

    id = msg.id;
    recipient = msg.recipient;

    return *this;
}

BaseMessage::BaseMessage(BaseMessage&& msg):
    id(msg.id),
    recipient(msg.recipient) {
    
    LOG_FUNCTION;

    msg.id = 0;
    msg.recipient.clear();
}

BaseMessage& BaseMessage::operator= (BaseMessage&& msg) {
    LOG_FUNCTION;

    id = std::move(msg.id);
    recipient = std::move(msg.recipient);
    
    msg.id = 0;
    msg.recipient.clear();

    return *this;
}

std::ostream& operator<< (std::ostream& ostr, const BaseMessage& msg) {
    ostr << "message " << msg.id << " @ " << (msg.recipient.empty() ? "-" : msg.recipient);
    return ostr;
}
