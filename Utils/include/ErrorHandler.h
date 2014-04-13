#pragma once

enum ERROR_TYPE {
    ERROR_TYPE_FATAL,
    ERROR_TYPE_ERROR,
    ERROR_TYPE_WARNING,
    ERROR_TYPE_INFO,
    ERROR_TYPE_DEBUG
};

class ErrorHandler {
public:
    void Process(ERROR_TYPE errorType, char* component, char* file, int line, wchar_t* format, ...);
    static ErrorHandler* Instance();
private:
    ErrorHandler();
    ErrorHandler(const ErrorHandler&);
    ErrorHandler& operator=(const ErrorHandler&);

    static ErrorHandler* _instance;
};

#ifndef COMPONENT_NAME
    #define COMPONENT_NAME nullptr
#endif

#define VE_ERROR(...)   ErrorHandler::Instance()->Process(ERROR_TYPE_ERROR,     COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);
#define VE_WARNING(...) ErrorHandler::Instance()->Process(ERROR_TYPE_WARNING,   COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);
#define VE_INFO(...)    ErrorHandler::Instance()->Process(ERROR_TYPE_INFO,      COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);
#define VE_DEBUG(...)   ErrorHandler::Instance()->Process(ERROR_TYPE_DEBUG,     COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);

#define VE_ERROR_IF(cond, ...)      if (cond) {ErrorHandler::Instance()->Process(ERROR_TYPE_ERROR,      COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);}
#define VE_WARNING_IF(cond, ...)    if (cond) {ErrorHandler::Instance()->Process(ERROR_TYPE_WARNING,    COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);}
#define VE_INFO_IF(cond, ...)       if (cond) {ErrorHandler::Instance()->Process(ERROR_TYPE_INFO,       COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);}
#define VE_DEBUG_IF(cond, ...)      if (cond) {ErrorHandler::Instance()->Process(ERROR_TYPE_DEBUG,      COMPONENT_NAME, __FILE__, __LINE__, __VA_ARGS__);}