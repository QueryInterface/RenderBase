#pragma once
#include <cstdint>
#include <memory>

#ifdef _WIN32
	#include "Windows.h"
    #ifdef DEFINE_EXPORT
        #define LIB_EXPORT __declspec(dllexport)
    #else
        #define LIB_EXPORT __declspec(dllimport)
    #endif // DEFINE_EXPORT
    #define CALLING_CONVENTION  __stdcall
#endif //_WIN32

#define PREVENT_COPY(className) \
    className(const className&); \
    className& operator=(const className&);

#define UNUSED(var) var;

template<typename ClonableType>
struct IClonable  
{
    virtual ClonableType Clone() const = 0;
};

struct ISceneElement 
{
};

struct IHandle
{
    virtual void Release() = 0; 
};