#pragma once
#include <string>
#include <mutex>
#include <typeinfo>
#include "common.h"

namespace Utils {
    namespace Filesystem {
        bool            DirectoryExists(const std::wstring& dir);
        std::wstring    GetWorkingDirectory();
    };
    namespace Internal {
        std::wstring    GetMediaFolderPath();
    };
};

template <typename T, typename V>
std::shared_ptr<T> safe_pointer_cast(const std::shared_ptr<V>& p)
{
    std::shared_ptr<T> pointer = std::dynamic_pointer_cast<T>(p);
    VE_ERROR_IF(!pointer, L"Failed to cast type %s to %s", typeid(V).name(), typeid(T).name());
    return pointer;
}

template <typename TClass, typename... TArgs>
TClass& Singleton(TArgs... args)
{
    struct FriendClass : public TClass
    {
        FriendClass(TArgs... args) : TClass(args...) {}
        virtual ~FriendClass() {}
    };
    static std::unique_ptr<FriendClass> s_instance(new FriendClass(args...));
    return *s_instance.get();
}

#define ENABLE_SINGLETON template <typename TClass, typename... TArgs> friend TClass& Singleton(TArgs... args)