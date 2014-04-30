#pragma once
#include "Resources.h"

struct IResourceOverseer 
{
    virtual IMeshPtr            CreateMesh(const wstring& path)                 = 0;
    virtual ITexturePtr         CreateTexture(const wstring& path)              = 0;
    virtual IScriptPtr          CreateScript(const wstring& path)               = 0;

    static IResourceOverseer*   instance();
};

