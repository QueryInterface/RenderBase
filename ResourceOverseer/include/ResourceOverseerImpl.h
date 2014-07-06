#pragma once
#include "ResourceOverseer.h"

class Mesh : public IMesh
{
public:
    Mesh();
    Mesh(const std::wstring& path);
    ~Mesh();
    IMeshPtr Clone() const;
    virtual const Desc& GetDesc() const override;
private:
    void parseObj(const std::wstring& path);

    Desc m_desc;
};

class ResourceOverseerImpl : public IResourceOverseer
{
public:
    ResourceOverseerImpl();
    virtual ~ResourceOverseerImpl();   
    virtual IMeshPtr            LoadMesh(const std::wstring& path) override;
    virtual ITexturePtr         LoadTexture(const std::wstring& path) override;
    virtual IScriptPtr          LoadScript(const std::wstring& path) override;

    virtual void                SaveMesh(IMeshPtr mesh, const std::wstring& path) const;
    virtual void                SaveTexture(IMeshPtr mesh, const std::wstring& path) const;
    virtual void                SaveScript(IMeshPtr mesh, const std::wstring& path) const;
private:
};
