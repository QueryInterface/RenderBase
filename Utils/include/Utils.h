#pragma once
#include <string>
#include "common.h"

//class RefPtr
//{
//public:
//    RefPtr(RefCount* object)
//        : m_obj(object)
//    {
//    }
//    ~RefPtr()
//    {
//        m_obj->Release();
//        m_obj = nullptr;
//    }
//    void Reset(RefCount* object)
//    {
//        m_obj->Release();
//        m_obj = object;
//    }
//    RefPtr& operator=(RefPtr& ptr)
//    {
//        m_obj->Release();
//        ptr.m_obj->AddRef();
//        m_obj = ptr.m_obj;
//    }
//    RefPtr& operator=(RefCount* ptr)
//    {
//        m_obj->Release();
//        m_obj = ptr;
//    }
//    bool operator==(const RefPtr& ptr)
//    {
//        return m_obj == ptr.m_obj;
//    }
//    bool operator!=(const RefPtr& ptr)
//    {
//        return m_obj != ptr.m_obj;
//    }
//    bool operator>(const RefPtr& ptr)
//    {
//        return m_obj > ptr.m_obj;
//    }
//    bool operator<(const RefPtr& ptr)
//    {
//        return m_obj < ptr.m_obj;
//    }
//    bool operator>=(const RefPtr& ptr)
//    {
//        return m_obj >= ptr.m_obj;
//    }
//    bool operator<=(const RefPtr& ptr)
//    {
//        return m_obj <= ptr.m_obj;
//    }
//private:
//    RefCount* m_obj;
//};

namespace Utils {
    namespace Filesystem {
        bool            DirectoryExists(const std::wstring& dir);
        std::wstring    GetWorkingDirectory();
    };
    namespace Internal {
        std::wstring    GetMediaFolderPath();
    };
};