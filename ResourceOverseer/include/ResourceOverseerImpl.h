#pragma once
#include "IResourceOverseer.h"

class LoadedMesh : public IMesh
{
public:
    LoadedMesh();
    virtual ~LoadedMesh();
    // IHandle
    virtual void Release() override;
    // IMesh
    virtual const LayoutData_t& GetLayout()     const override;
    virtual const VertexData_t& GetMeshBuffer() const override;
    virtual const IndexData_t&  GetIndexData(unsigned int flags) const override;
private:
    LayoutData_t m_layoutData;
    VertexData_t m_vertexData;
    IndexData_t m_indexData;
};

class IResourceOverseerImpl : public IResourceOverseer
{
public:
    IResourceOverseerImpl();
    virtual ~IResourceOverseerImpl();   
    virtual IMeshPtr            CreateMesh(const wstring& path) override;
    virtual ITexturePtr         CreateTexture(const wstring& path) override;
    virtual IScriptPtr          CreateScript(const wstring& path) override;
private:
};
