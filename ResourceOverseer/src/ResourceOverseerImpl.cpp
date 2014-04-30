#include "ResourceOverseerImpl.h"

LoadedMesh::LoadedMesh()
{

}

LoadedMesh::~LoadedMesh()
{

}

void LoadedMesh::Release()
{
    delete this;
}

const LayoutData_t& LoadedMesh::GetLayout()     const
{
    return m_layoutData;
}

const VertexData_t& LoadedMesh::GetMeshBuffer() const
{
    return m_vertexData;
}

const IndexData_t& LoadedMesh::GetIndexData(unsigned int flags) const
{
    return m_indexData;
}


IResourceOverseerImpl::IResourceOverseerImpl()
{

}

IResourceOverseerImpl::~IResourceOverseerImpl()
{
    
}

IMeshPtr ResourceOverseerImpl::CreateMesh(const wstring& path)
{
    return nullptr;
}

ITexturePtr ResourceOverseerImpl::CreateTexture(const wstring& path)
{
    return nullptr;
}

IScriptPtr ResourceOverseerImpl::CreateScript(const wstring& path)
{
    return nullptr;
}

static IResourceOverseer* IResourceOverseer::instance()
{
    static std::unique_ptr<IResourceOverseer> s_resourceOverseer;
    if (!s_resourceOverseer)
        s_resourceOverseer.reset(new ResourceOverseerImpl());
    return s_resourceOverseer.get();    
}
