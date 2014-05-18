#include "ResourceOverseerImpl.h"
#include "ErrorHandler.h"

//LoadedMesh
LoadedMesh::LoadedMesh(const std::wstring& path)
{
    std::string err = tinyobj::LoadObj(m_shapes, path.c_str());
    if (!err.empty())
    {
        VE_ERROR(L"%s", err.c_str());
    }
}

LoadedMesh::~LoadedMesh()
{
}

void LoadedMesh::GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const
{
    flags;
    out_descriptor.layout.clear();
    out_descriptor.groups.clear();
}

/// ResourceOversserImpl
ResourceOverseerImpl::ResourceOverseerImpl()
{

}

ResourceOverseerImpl::~ResourceOverseerImpl()
{
    
}

IMeshPtr ResourceOverseerImpl::LoadMesh(const wstring& path)
{
    path;
    return nullptr;// make_shared_safe<IMesh, LoadedMesh>(path);
}

ITexturePtr ResourceOverseerImpl::LoadTexture(const wstring& path)
{
    path;
    return nullptr;
}

IScriptPtr ResourceOverseerImpl::LoadScript(const wstring& path)
{
    path;
    return nullptr;
}

IResourceOverseer* IResourceOverseer::Instance()
{
    static std::unique_ptr<IResourceOverseer> s_resourceOverseer;
    if (!s_resourceOverseer)
        s_resourceOverseer.reset(new ResourceOverseerImpl());
    return s_resourceOverseer.get();    
}
