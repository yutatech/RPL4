#ifndef RPL4_MESSAGE_HPP_
#define RPL4_MESSAGE_HPP_

#include <string>

namespace rpl {

enum class LogLevel {
    Fatal   = 0,
    Error   = 1,
    Warning = 2,
    Info    = 3,
    Debug   = 4
};

void Log(LogLevel lebel, const char* format, ...);

}

#endif