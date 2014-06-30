#pragma once
#include "ResourceOverseer.h"

class Mesh : public IMesh
{
public:
    Mesh();
    Mesh(const IMesh::GeometryDesc& desc);
    ~Mesh();
    IMeshPtr Clone() const;
    virtual void GetGeometryDesc(GeometryDesc& out_descriptor) const override;
private:
    GeometryDesc m_desc;
    std::vector< std::vector<float> >       m_vertices;
    std::vector< std::vector<float> >       m_normals;
    std::vector< std::vector<float> >       m_textures;
    std::vector< std::vector<uint32_t> >    m_indices;
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
