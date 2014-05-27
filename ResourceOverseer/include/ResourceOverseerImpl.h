#pragma once
#include "ResourceOverseer.h"
#include "tiny_obj_loader.h"

class LoadedMesh : public IMesh
{
public:
    LoadedMesh(const std::wstring& path);
    ~LoadedMesh();
    virtual IMeshPtr Clone() const {return nullptr;}
    virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const override;
private:
    std::vector<tinyobj::shape_t> m_shapes;
};

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
