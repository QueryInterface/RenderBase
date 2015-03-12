#pragma once
#include "ResourceOverseer.h"

class Mesh : public IMesh
{
public:
    Mesh();
    Mesh(const std::string& path);
    ~Mesh();
    IMeshPtr Clone() const;
    virtual const Desc& GetDesc() const override;
private:
    void parseObj(const std::string& path);

    Desc m_desc;
};

class ResourceOverseerImpl : public IResourceOverseer
{
public:
    virtual IMeshPtr            LoadMesh(const std::string& path) override;
    virtual ITexturePtr         LoadTexture(const std::string& path) override;
    virtual IScriptPtr          LoadScript(const std::string& path) override;

    virtual void                SaveMesh(IMeshPtr mesh, const std::string& path) const;
    virtual void                SaveTexture(IMeshPtr mesh, const std::string& path) const;
    virtual void                SaveScript(IMeshPtr mesh, const std::string& path) const;
protected:
    ResourceOverseerImpl();
    virtual ~ResourceOverseerImpl(); 
private:
    PREVENT_COPY(ResourceOverseerImpl);
};
