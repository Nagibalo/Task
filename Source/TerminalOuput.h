#ifndef TERMINALOUTPUT_H
#define TERMINALOUTPUT_H
#include <iostream>
#include <ctime>
#include <string>

namespace out {

enum LogType {
    INFO,
    ERROR,
    ECHO_SERVICE,
    RESPONCE,
    FILE_SERVICE
};

const static std::string log_types[5] = {"INFO", "ERROR", "ECHO_SERVICE", "RESPONCE", "FILE_SERVICE"};

static void PrintMessage(const std::string &message, LogType type)
{
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {" << log_types[type] << "} " << message << '\n';
}

static void PrintMessage(const char *message, LogType type)
{
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {" << log_types[type] << "} " << message << '\n';
}

}
#endif // TERMINALOUTPUT_H