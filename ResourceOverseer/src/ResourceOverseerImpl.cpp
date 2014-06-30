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

Mesh::Mesh(const IMesh::GeometryDesc& desc)
{
    m_desc = desc;
    // Make deep copy
    for (auto layout : m_desc.layout)
    {
        vector<float> data(layout.items, layout.items + layout.itemsCount);
        switch (layout.layoutType)
        {
        case IMesh::LayoutType::Vertices:
            {
                m_vertices.push_back(std::move(data));
                layout.items = m_vertices.back().data();
            }
            break;
        case IMesh::LayoutType::Normals:
            {
                m_normals.push_back(std::move(data));
                layout.items = m_normals.back().data();
            }
            break;
        case IMesh::LayoutType::Texcoord0:
        case IMesh::LayoutType::Texcoord1:
            {
                m_textures.push_back(std::move(data));
                layout.items = m_textures.back().data();
            }
            break;
        default:
            VE_ERROR(L"Invalid layout type");
        }
    }
}

Mesh::~Mesh()
{
}

IMeshPtr Mesh::Clone() const
{
    CLONE_HANDLE(IMesh, Mesh);
}

void Mesh::GetGeometryDesc(GeometryDesc& out_descriptor) const
{
    out_descriptor = m_desc;
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
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, path.c_str());
    if (!err.empty())
    {
        VE_ERROR(L"%s", err.c_str());
    }

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
