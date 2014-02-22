#pragma once

#ifdef _WIN32
	#include "Windows.h"
    #ifdef DEFINE_EXPORT
        #define LIB_EXPORT __declspec(dllexport)
    #else
        #define LIB_EXPORT __declspec(dllimport)
    #endif // DEFINE_EXPORT
    #define CALLING_CONVENTION  __stdcall
#endif //_WIN32