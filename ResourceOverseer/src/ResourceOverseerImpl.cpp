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

IMesh::Desc* Mesh::GetDesc() const
{
    return &m_desc;
}

void Mesh::parseObj(const std::wstring& path)
{
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, path.c_str());
    if (!err.empty())
    {
        VE_ERROR(L"%s", err.c_str());
    }
    for (auto shape : shapes)
    {
        m_desc.Positions.Data = shape.mesh.positions;
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


    IMesh::GeometryDesc desc;
    for (auto shape : shapes)
    {
        IMesh::LayoutItem vertexLayout;
        vertexLayout.layoutType = IMesh::LayoutType::Vertices;
        vertexLayout.items = shape.mesh.positions.data();
        vertexLayout.itemsCount = shape.mesh.positions.size();
        vertexLayout.itemSize = 3;

        IMesh::LayoutItem normalLayout;
        normalLayout.layoutType = IMesh::LayoutType::Normals;
        normalLayout.items = shape.mesh.normals.data();
        normalLayout.itemsCount = shape.mesh.normals.size();
        normalLayout.itemSize = 3;

        IMesh::LayoutItem textureLayout;
        textureLayout.layoutType = IMesh::LayoutType::Texcoord0;
        textureLayout.items = shape.mesh.texcoords.data();
        textureLayout.itemsCount = shape.mesh.texcoords.size();
        textureLayout.itemSize = 2;

        IMesh::IndexGroup indexGroup;
        indexGroup.indices = shape.mesh.indices.data();
        indexGroup.count = shape.mesh.indices.size();

        desc.layout.push_back(vertexLayout);
        desc.layout.push_back(normalLayout);
        desc.layout.push_back(textureLayout);
        desc.groups.push_back(indexGroup);
    }

    std::shared_ptr<Mesh> mesh = make_shared_handle<Mesh>(desc);
    path;
    return nullptr;// make_shared_safe<IMesh, Mesh>(path);
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

IResourceOverseer* IResourceOverseer::Instance()
{
    static std::unique_ptr<IResourceOverseer> s_resourceOverseer;
    if (!s_resourceOverseer)
        s_resourceOverseer.reset(new ResourceOverseerImpl());
    return s_resourceOverseer.get();    
}
