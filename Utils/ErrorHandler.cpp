#include "ErrorHandler.h"
#include "common.h"
#include <stdio.h>
#include <stdexcept>
#include <sstream>

#define MAX_MESSAGE_SIZE 512
ErrorHandler::ErrorHandler() {
}

void ErrorHandler::Process(ERROR_TYPE errorType, char* component, char* file, int line, wchar_t* format, ...) {
    wchar_t message[MAX_MESSAGE_SIZE];
    std::wstringstream messageStream;
    va_list args;
    va_start(args, format);
    vswprintf(message, format, args);
    messageStream << "(" << file << ": " << line << ") " << message;
    ::MessageBoxW(NULL, messageStream.str().c_str(), L"Error", NULL);
    throw std::exception("");
    va_end(args);
}

ErrorHandler* ErrorHandler::Instance() {
    if (!_instance) {
        _instance = new ErrorHandler();
    }
    return _instance;
}

ErrorHandler* ErrorHandler::_instance = nullptr;