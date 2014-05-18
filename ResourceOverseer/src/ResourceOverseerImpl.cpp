#include "ResourceOverseerImpl.h"

ResourceOverseerImpl::ResourceOverseerImpl()
{

}

ResourceOverseerImpl::~ResourceOverseerImpl()
{
    
}

IMeshPtr ResourceOverseerImpl::LoadMesh(const wstring& path)
{
    path;
    return nullptr;
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
