#pragma once
#include <string>
#include "common.h"
#include <typeinfo>

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