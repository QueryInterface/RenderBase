#include "Library.h"
#include <memory>

// base class for all primitive meshes
class BaseMesh : public IMesh
{
public:
    BaseMesh() { }

    virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const 
    {
        flags;
        out_descriptor.groups.clear();
    }

protected:
    std::vector<float>      m_vertices;
    std::vector<LayoutItem> m_layout;
};

// Space mesh, this mesh will be provided as dummy mesh object
class SpaceMesh : public BaseMesh
{
public:
    SpaceMesh() { ILibrary::library()->RegisterMesh(ElementType::Space, *this); };
    virtual void Release() override {delete this;}
    virtual IMeshPtr Clone() const {return std::make_shared<SpaceMesh>(*this);};
private:
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> SpaceMesh::self(new SpaceMesh());

// Simple Cube mesh
class CubeMesh : public BaseMesh
{
public:
    CubeMesh()
    {
#include "../models/Cube.mdl"

        for (size_t i = 0; i < groupsCount; ++i)
        {
            m_indices[i].assign(indexGroups[i], indexGroups[i] + sizeof(indexGroups[i])/sizeof(short));
        }

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::Cube, *this);
    }

    virtual void Release() override 
    {
        delete this;
    }

    virtual IMeshPtr Clone() const 
    {
        return std::make_shared<CubeMesh>(*this);
    };

    virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const 
    {
        out_descriptor.groups.clear();
        LayoutItem li;
        li.layoutType = LayoutType::Vertices;
        li.itemSize = 3;
        li.itemsCount = m_vertices.size();
        li.items = (float*)m_vertices.data();
        out_descriptor.layout.push_back(li);

        for (size_t i = 0; i < 6; ++i )
        {
            if (flags & 1 << i)
            {
                IndexGroup mc;
                mc.indices = m_indices[i].data();
                mc.count = m_indices[i].size();
                out_descriptor.groups.push_back(mc);
            }
        }
    }

private:
    std::vector<unsigned int> m_indices[6];
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> CubeMesh::self(new CubeMesh());
//eof