#include "Library.h"
#include <memory>

// base class for all primitive meshes
class BaseMesh : public IMesh
{
public:
    BaseMesh() { }

    virtual const LayoutData_t& GetLayout() const                                 { return m_layout;   }
    virtual const VertexData_t& GetMeshBuffer() const                             { return m_vertices; }
    virtual void                GetIndexData(unsigned int, IndexData_t& ind) const    { ind.clear(); }

protected:
    VertexData_t    m_vertices;
    LayoutData_t    m_layout;
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

        m_layout.assign(layout, layout + sizeof(layout)/sizeof(LayoutData_t));

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

    virtual void GetIndexData(unsigned int flags, IndexData_t& indices) const
    {
        indices.clear();
        for (size_t i = 0; i < 6; ++i )
        {
            if (flags & 1 << i)
            {
                indices.insert(indices.end(), m_indices[i].begin(), m_indices[i].end());
            }
        }
    }

private:
    IndexData_t     m_indices[6];
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> CubeMesh::self(new CubeMesh());
//eof