#include "Library.h"
#include <memory>

// base class for all primitive meshes
class BaseMesh : public IMesh
{
public:
    BaseMesh() { }

    virtual const LayoutData_t& GetLayout() const                   { return m_layout;   }
    virtual const VertexData_t& GetMeshBuffer() const               { return m_vertices; }
    virtual const IndexData_t&  GetIndexData(unsigned int) const    { return m_indices;  }

protected:
    VertexData_t    m_vertices;
    IndexData_t     m_indices;
    LayoutData_t    m_layout;
};

// Space mesh, this mesh will be provided as dummy mesh object
class SpaceMesh : public BaseMesh
{
public:
    SpaceMesh() { ILibrary::library()->RegisterMesh(ElementType::Space, *this); };
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
        short indices[] = {
            0, 1, 2, 1, 2, 3, // front
            2, 3, 4, 2, 4, 5, // right
            1, 6, 4, 1, 4, 3, // top
            7, 5, 4, 7, 4, 6, // back
            1, 0, 7, 1, 7, 6, // left
            0, 2, 7, 2, 5, 7, // bottom
        };

        m_indices.assign(indices, indices + sizeof(indices)/sizeof(short));
        ILibrary::library()->RegisterMesh(ElementType::Cube, *this);
    }

    virtual const IndexData_t&  GetIndexData(unsigned int flags) const
    {
        flags;
        return m_indices;
    }

private:
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> CubeMesh::self(new CubeMesh());
//eof