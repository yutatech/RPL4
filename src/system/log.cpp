#include "rpl4/system/log.hpp"

#include <iostream>

#ifdef RPL4_LOG_LEVEL
    #include <cstdarg>
#endif

namespace rpl {

void Log(LogLevel level, const char* format, ...){
#ifdef RPL4_LOG_LEVEL
    if (level <= RPL4_LOG_LEVEL) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        std::string message =  std::string(buffer);

        if (level == LogLevel::Fatal) {
            std::cout << "\033[1;35m[RPL4 Fatal] " << message << "\033[m" << std::endl;
            std::cout << "\033[1;35m  **Program will terminate...\033[m" << std::endl;
            exit(-1);
        }
        else if (level == LogLevel::Error)
            std::cout << "\033[1;31m[RPL4 Error] " << message << "\033[m" << std::endl;
        else if (level == LogLevel::Warning)
            std::cout << "\033[33m[RPL4 Warning] " << message << "\033[m" << std::endl;
        else if (level == LogLevel::Info)
            std::cout << "\033[34m[RPL4 Info] " << message << "\033[m" << std::endl;
        else if (level == LogLevel::Debug)
            std::cout << "[RPL4 Debug] " << message << std::endl;
    }
#else
    (void) format;  // Suppress unused variable warning
    if (level == LogLevel::Fatal) {
        std::cout << "[RPL4] Fatal Error occured. Program will terminate..." << std::endl;
        exit(-1);
    }
#endif
    
}

}