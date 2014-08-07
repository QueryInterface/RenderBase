#include "ResourceOverseerImpl.h"
#include "ErrorHandler.h"
#include "HandleImpl.h"
#include "tiny_obj_loader.h"

using std::wstring;
using std::vector;

//Mesh
Mesh::Mesh()
{
}

Mesh::Mesh(const std::wstring& path)
{
    size_t pos = path.find_last_of(L".");
    std::wstring ext = path.substr(pos + 1, path.length() - pos - 1);
    if (0 == ext.compare(L"obj"))
    {
        parseObj(path);
    }
    else
    {
        VE_ERROR(L"Unsupported file type");
    }
}

Mesh::~Mesh()
{
}

IMeshPtr Mesh::Clone() const
{
    CLONE_HANDLE(IMesh, Mesh);
}

const IMesh::Desc& Mesh::GetDesc() const
{
    return m_desc;
}

void Mesh::parseObj(const std::wstring& path)
{
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, path.c_str());
    if (!err.empty())
    {
        VE_ERROR(L"%S", err.c_str());
    }
    m_desc.Shapes.resize(shapes.size());
    for (uint32_t i = 0; i < shapes.size(); ++i)
    {
        m_desc.Shapes[i].Positions.Data = shapes[i].mesh.positions;
        m_desc.Shapes[i].Positions.ElementSize = 3;
        m_desc.Shapes[i].LayoutType = IMesh::LayoutType::Triangle;

        m_desc.Shapes[i].Normals.Data = shapes[i].mesh.normals;
        m_desc.Shapes[i].Normals.ElementSize = 2;
        
        m_desc.Shapes[i].TexCoords.Data = shapes[i].mesh.texcoords;
        m_desc.Shapes[i].TexCoords.ElementSize = 2;

        m_desc.Shapes[i].Indices.Data = shapes[i].mesh.indices;
        m_desc.Shapes[i].Indices.ElementSize = 1;
    }
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
    return make_shared_handle<Mesh>(path);
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

void ResourceOverseerImpl::SaveMesh(IMeshPtr mesh, const std::wstring& path) const
{
    mesh;path;
}

void ResourceOverseerImpl::SaveTexture(IMeshPtr mesh, const std::wstring& path) const
{
    mesh;path;
}

void ResourceOverseerImpl::SaveScript(IMeshPtr mesh, const std::wstring& path) const
{
    mesh;path;
}

IResourceOverseer& IResourceOverseer::Instance()
{
    static std::unique_ptr<IResourceOverseer> s_resourceOverseer;
    if (!s_resourceOverseer)
        s_resourceOverseer.reset(new ResourceOverseerImpl());
    return *s_resourceOverseer.get();    
}
