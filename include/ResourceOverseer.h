#pragma once
#include "Resources.h"
#include "common.h"
#include <string>

using std::wstring;

struct IResourceOverseer 
{
    virtual IMeshPtr            LoadMesh(const wstring& path)                 = 0;
    virtual ITexturePtr         LoadTexture(const wstring& path)              = 0;
    virtual IScriptPtr          LoadScript(const wstring& path)               = 0;

    static LIB_EXPORT IResourceOverseer* CALLING_CONVENTION Instance();
};

