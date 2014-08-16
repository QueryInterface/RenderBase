#pragma once
#include <cstdint>
#include <memory>

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::static_pointer_cast;

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

enum class Status
{
    // Statuses
    OK = 0,
    Pending,

    // TODO: maybe here we should split enum on two???

    // Error codes
    ResourceNotFound,
    AlreadyExists,
};

template<typename ClonableType>
struct IClonable  
{
    virtual ClonableType Clone() const = 0;
};

struct IHandle
{
    virtual void Release() = 0;
};
