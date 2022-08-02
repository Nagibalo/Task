#ifndef TERMINALOUTPUT_H
#define TERMINALOUTPUT_H
#include <iostream>
#include <ctime>
#include <string>

#define IS_DEBUG true

namespace out {

enum LogType {
    INFO,
    ERROR,
    STATUS,
    ECHO_SERVICE,
    RESPONCE,
    FILE_SERVICE
};

const static std::string log_types[6] = {"INFO", "ERROR", "STATUS", "ECHO_SERVICE", "RESPONCE", "FILE_SERVICE"};

static void PrintMessage(const std::string &message, LogType type)
{
    if(!IS_DEBUG && (type != INFO && type != ERROR))
        return;
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {" << log_types[type] << "} " << message << '\n' << std::flush;
}

static void PrintMessage(const char *message, LogType type)
{
    if(!IS_DEBUG && (type != INFO && type != ERROR))
        return;
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {" << log_types[type] << "} " << message << '\n' << std::flush;
}

}
#endif // TERMINALOUTPUT_H