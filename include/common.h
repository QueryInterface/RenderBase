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

class RefCount
{
public:
    RefCount()
        : m_refCount(1)
    {
    }
    virtual uint32_t AddRef()
    {
        return ++m_refCount;
    }
    virtual uint32_t Release()
    {
        if (m_refCount > 0)
        {
            --m_refCount;
            if (!m_refCount) delete this;
        }
    }
private:
    uint32_t m_refCount;
};

//template <typename BaseType, typename DerivedType, typename... Args>
//std::shared_ptr<BaseType> make_shared_safe(const Args&... args)
//{
//    DerivedType* temp = new DerivedType(args...);
//    std::shared_ptr<DerivedType> shared(temp, [&](BaseType* obj) {obj->Release();});
//    return std::static_pointer_cast<BaseType>(shared);
//}