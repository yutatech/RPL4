#include "system/rpl4_message.hpp"

#include <iostream>

namespace rpl {

void Message(std::string message, MessageLevel lebel){
#ifdef RPL4_DEBUG
    if (lebel == MessageLevel::info)
        std::cout << "Info : " << message << std::endl;
    else if (lebel == MessageLevel::warning)
        std::cout << "\033[33mWarning : " << message << "\033[m"  << std::endl;
    else if (lebel == MessageLevel::error) {
        std::cout << "\033[1;31mError : " << message << "\033[m" << std::endl;
        exit(-1);
    }
#endif
}

void Message(std::string message){
    Message(message, MessageLevel::info);
}

}