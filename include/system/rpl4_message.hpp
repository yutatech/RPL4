#ifndef RPL4_MESSAGE_HPP_
#define RPL4_MESSAGE_HPP_

#include <string>

namespace rpl {

enum class MessageLevel{
    error,
    warning,
    info
};

void Message(std::string RPI4_Message, MessageLevel lebel);
void Message(std::string RPI4_Message);

}

#endif