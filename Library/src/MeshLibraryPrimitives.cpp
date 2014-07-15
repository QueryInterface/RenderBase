#include "Library.h"
#include <memory>
#include "HandleImpl.h"

///////////////////////////////////////////////////////////////////////////////////
// base class for all primitive meshes
///////////////////////////////////////////////////////////////////////////////////
class BaseMesh : public ILibraryMesh
{
public:
    BaseMesh() { }
    const IMesh::Desc& GetDesc() const override
    {
        return m_desc;
    }

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        properties;
        out_descriptor.Positions.Data.clear();
    }

    virtual void Release() {};

protected:
    void rotate(const float* in, uint32_t dst, const vector3f_t& offset, float* out) const
    {
        memcpy(out, in, sizeof(float) * 3);

        switch(dst & MODIFICATOR_MASK)
        {
        case Directions::LeftToRight:
            out[0] = 1 - out[0];
            break;
        }
        
        float tmp = out[0];
        switch(dst & DIRECTION_MASK)
        {
        case Directions::nX :
            out[0] = 1 - out[2];
            out[2] = tmp;
            break;
        case Directions::pX :
            out[0] = out[2];
            out[2] = 1 - tmp;
            break;
        case Directions::nZ : 
            out[0] = 1 - out[0];
            out[2] = 1 - out[2];
            break;
        default:
            break;
        }
        out[0] += offset.x; out[1] += offset.y; out[2] += offset.z;
    }

    void copyTriangles(IMesh::Shape& out_descriptor, const vector3f_t& offset, uint32_t orientation, const index_t* indexBlock, size_t size) const
    {
        // triangles supported only. for now
        assert(0 == size % 3);
        std::vector<float>& vertices = out_descriptor.Positions.Data;
        vertices.reserve(vertices.size() + size * 3);

        const int directIndexOrder[3] = {0, 1, 2};
        const int morrorIndexOrder[3] = {1, 0, 2};
        const int *indexOrder = (orientation & MODIFICATOR_MASK) ? morrorIndexOrder : directIndexOrder;

        // work with vertex triplets
        for (size_t i = 0; i < size; i += 3)
        {
            float outVertex[9] = {};
            rotate(&m_vertices[ indexBlock[i + indexOrder[0]] * 3], orientation, offset, outVertex);
            rotate(&m_vertices[ indexBlock[i + indexOrder[1]] * 3], orientation, offset, outVertex + 3);
            rotate(&m_vertices[ indexBlock[i + indexOrder[2]] * 3], orientation, offset, outVertex + 6);
            vertices.insert(vertices.end(), outVertex, outVertex + 9);
        }
    }

    std::vector<float>      m_vertices;
    IMesh::Desc             m_desc;
};
///////////////////////////////////////////////////////////////////////////////////
// Space mesh, this mesh will be provided as dummy mesh object
///////////////////////////////////////////////////////////////////////////////////
class SpaceMesh : public BaseMesh
{
public:
    SpaceMesh() { ILibrary::library()->RegisterMesh(ElementType::Space, *this); };
    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, SpaceMesh);
    };
private:
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> SpaceMesh::self(new SpaceMesh());

///////////////////////////////////////////////////////////////////////////////////
// Simple Cube mesh
///////////////////////////////////////////////////////////////////////////////////
class CubeMesh : public BaseMesh
{
public:
    CubeMesh()
    {
        const size_t groupsCount = 6;
        index_t indexGroups[groupsCount][groupsCount] = 
        {
            {3, 2, 4, 4, 2, 5,}, // front  +x
            {6, 1, 4, 4, 1, 3,}, // top    +y
            {1, 0, 2, 1, 2, 3,}, // right  +z
            {0, 1, 7, 7, 1, 6,}, // back   -x
            {2, 0, 7, 5, 2, 7,}, // bottom -y
            {5, 7, 4, 4, 7, 6,}, // left   -z
        };

        float vertices[] = 
        {//     vertex         normal    texcoord
        //     front face
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
        //     back face
            1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
        };

        for (size_t i = 0; i < groupsCount; ++i)
        {
            m_indices[i].assign(indexGroups[i], indexGroups[i] + sizeof(indexGroups[i])/sizeof(index_t));
        }

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::Cube, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, CubeMesh);
    };

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        for (size_t i = 0; i < 6; ++i )
        {
            if (properties.flags & (1 << i))
            {
                copyTriangles(out_descriptor, properties.offset, properties.orientation, m_indices[i].data(), m_indices[i].size()); 
            }
        }
    }

private:
    std::vector<index_t> m_indices[6];
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> CubeMesh::self(new CubeMesh());

///////////////////////////////////////////////////////////////////////////////////
// Simple Wedge mesh
///////////////////////////////////////////////////////////////////////////////////
class WedgeMesh : public BaseMesh
{
public:
    WedgeMesh()
    {
        float vertices[] = 
        {//     vertex         normal    texcoord
        //     front edge
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
        //     back face
            1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
        };

        index_t indexGroups[] = 
        {
            5, 0, 4,          // front  +x
            2, 0, 1, 4, 0, 2, // top    +y
                              //        +z
            2, 1, 3,          // back   -x
            1, 0, 5, 3, 1, 5, // bottom -y
            3, 5, 2, 2, 5, 4, // left   -z
        };

        m_indices.assign(indexGroups, indexGroups + sizeof(indexGroups)/sizeof(index_t));

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::Wedge, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, WedgeMesh);
    };

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        uint32_t flags = properties.flags;
        // pY, pZ
        if (flags & Directions::pX || flags & (size_t)Directions::nX || flags & (size_t)Directions::pZ || flags & (size_t)Directions::pY)
        {
            copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[3], 6);
        }

        const int groups[] = {9,  3,  3,  0, 12, 18};
        const int sizes[]  = {3,  6,  6,  3,  6,  6};

        for (size_t i = 0; i < 6; ++i )
        {
            if (flags & (1 << i) && i != DirectionIndices::pY_idx && i != DirectionIndices::pZ_idx)
            {
                copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[ groups[i] ], sizes[i]);
            }
        }
    }

private:
    std::vector<index_t> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> WedgeMesh::self(new WedgeMesh());

///////////////////////////////////////////////////////////////////////////////////
// Wedge angle mesh
///////////////////////////////////////////////////////////////////////////////////
class WedgeOuterAngleMesh : public BaseMesh
{
public:
    WedgeOuterAngleMesh()
    {
        float vertices[] = 
        {//     vertex         normal    texcoord
        //     front edge
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 0.0f,
        };

        index_t indexGroups[] = 
        {
            2, 4, 1,          // front  +x
            0, 4, 3, 1, 4, 0, // left   +y
            3, 4, 2,          // top    +z
               //               -x
            1, 0, 3, 2, 1, 3, // bottom -y
            //                  -z
        };

        m_indices.assign(indexGroups, indexGroups + sizeof(indexGroups)/sizeof(index_t));

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::WedgeOutCorner, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, WedgeOuterAngleMesh);
    };

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        //uint32_t flags = properties.flags;

        // pyramid of outer angle is invisible only if all neighbours are available except nY
        //if (flags & (~Directions::nY))
        {
            copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[0], 18);
        }
        /*if (flags & Directions::pX)
        {
            copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[0], 3);
        }
        /*if (flags & Directions::pX)
        {
            copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[0], 3);
        }
        if (flags & Directions::nY)
        {
            copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[ 12 ], 6);
        }*/

        //copyTriangles(out_descriptor, properties.offset, properties.orientation, m_indices.data(), m_indices.size());
    }

private:
    std::vector<index_t> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> WedgeOuterAngleMesh::self(new WedgeOuterAngleMesh());

class WedgeInnerAngleMesh : public BaseMesh
{
public:
    WedgeInnerAngleMesh()
    {
        float vertices[] = 
        {//     vertex         normal    texcoord
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
        };

        index_t indexGroups[] = 
        {
            4, 1, 6,            // +x
            1, 3, 2, 4, 3, 1,   // +y
            1, 2, 0,            // +z
            5, 0, 3, 3, 0, 2,   // -x
            5, 6, 0, 0, 6, 1,   // -y
            3, 4, 5, 5, 4, 6,   // -z
        };

        m_indices.assign(indexGroups, indexGroups + sizeof(indexGroups)/sizeof(index_t));

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::WedgeInCorner, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, WedgeInnerAngleMesh);
    };

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[0], m_indices.size());
    }

private:
    std::vector<index_t> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> WedgeInnerAngleMesh::self(new WedgeInnerAngleMesh());

///////////////////////////////////////////////////////////////////////////////////
// Common cilinder mesh
///////////////////////////////////////////////////////////////////////////////////
class Cilinder : public BaseMesh
{
public:
    Cilinder()
    {
        float vertices[] = 
        {//     vertex         normal    texcoord
            0.782843f, 0.f, 0.217157f,
            0.500000f, 0.f, 0.100000f,
            0.217157f, 0.f, 0.217157f,
            0.100000f, 0.f, 0.500000f,
            0.217157f, 0.f, 0.782843f,
            0.500000f, 0.f, 0.900000f,
            0.782843f, 0.f, 0.782843f,
            0.900000f, 0.f, 0.500000f,
            0.782843f, 1.f, 0.217157f,
            0.500000f, 1.f, 0.100000f,
            0.217157f, 1.f, 0.217157f,
            0.100000f, 1.f, 0.500000f,
            0.217157f, 1.f, 0.782843f,
            0.500000f, 1.f, 0.900000f,
            0.782843f, 1.f, 0.782843f,
            0.900000f, 1.f, 0.500000f,
        };

        index_t indexGroups[] = 
        {
            0,  1,  8,
            8,  1,  9,
            1,  2,  9,
            9,  2,  10,
            2,  3,  10,
            10, 3,  11,
            3,  4,  11,
            11, 4,  12,
            4,  5,  12,
            12, 5,  13,
            5,  6,  13,
            13, 6,  14,
            6,  7,  14,
            14, 7,  15,
            7,  0,  15,
            15, 0,  8,
            6,  5,  7,
            5,  4,  7,
            4,  3,  7,
            3,  2,  7,
            2,  1,  7,
            1,  0,  7,
            15, 8,  14,
            8,  9,  14,
            9,  10, 14,
            10, 11, 14,
            11, 12, 14,
            12, 13, 14,
        };

        m_indices.assign(indexGroups, indexGroups + sizeof(indexGroups)/sizeof(index_t));

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::Cilinder, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, Cilinder);
    };

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        copyTriangles(out_descriptor, properties.offset, properties.orientation, &m_indices[0], m_indices.size());
    }

private:
    std::vector<index_t> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> Cilinder::self(new Cilinder());
//eof