#pragma once
#include <string>

namespace Utils {
    namespace Filesystem {
        bool            DirectoryExists(const std::wstring& dir);
        std::wstring    GetWorkingDirectory();
    };
    namespace Internal {
        std::wstring    GetMediaFolderPath();
    };
};