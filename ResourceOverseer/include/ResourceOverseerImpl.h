#pragma once
#include "ResourceOverseer.h"

class ResourceOverseerImpl : public IResourceOverseer
{
public:
    ResourceOverseerImpl();
    virtual ~ResourceOverseerImpl();   
    virtual IMeshPtr            LoadMesh(const wstring& path) override;
    virtual ITexturePtr         LoadTexture(const wstring& path) override;
    virtual IScriptPtr          LoadScript(const wstring& path) override;
private:
};
