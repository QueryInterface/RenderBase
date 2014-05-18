#pragma once
#include <cstdint>
#include <memory>

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;

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

class RefPtr
{
public:
    RefPtr(RefCount* object)
        : m_obj(object)
    {
    }
    ~RefPtr()
    {
        m_obj->Release();
        m_obj = nullptr;
    }
    void Reset(RefCount* object)
    {
        m_obj->Release();
        m_obj = object;
    }
    RefPtr& operator=(RefPtr& ptr)
    {
        m_obj->Release();
        ptr.m_obj->AddRef();
        m_obj = ptr.m_obj;
    }
    RefPtr& operator=(RefCount* ptr)
    {
        m_obj->Release();
        m_obj = ptr;
    }
    bool operator==(const RefPtr& ptr)
    {
        return m_obj == ptr.m_obj;
    }
    bool operator!=(const RefPtr& ptr)
    {
        return m_obj != ptr.m_obj;
    }
    bool operator>(const RefPtr& ptr)
    {
        return m_obj > ptr.m_obj;
    }
    bool operator<(const RefPtr& ptr)
    {
        return m_obj < ptr.m_obj;
    }
    bool operator>=(const RefPtr& ptr)
    {
        return m_obj >= ptr.m_obj;
    }
    bool operator<=(const RefPtr& ptr)
    {
        return m_obj <= ptr.m_obj;
    }
private:
    RefCount* m_obj;
};