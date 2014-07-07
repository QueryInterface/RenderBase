#include "Library.h"
#include <memory>
#include "HandleImpl.h"

// base class for all primitive meshes
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
        switch(dst)
        {
        case Directions::nX :
            out[0] = 1-in[2];
            out[2] = in[0];
            break;
        case Directions::pX :
            out[0] = in[2];
            out[2] = 1 - in[0];
            break;
        case Directions::nZ : 
            out[0] = 1 - in[0];
            out[2] = 1 - in[2];
            break;
        default:
            break;
        }
        out[0] += offset.x; out[1] += offset.y; out[2] += offset.z;
    }

    void copyFaces(IMesh::Shape& out_descriptor, const vector3f_t& offset, uint32_t orientation, const uint16_t* indexBlock, size_t size) const
    {
        std::vector<float>& vertices = out_descriptor.Positions.Data;
        vertices.reserve(vertices.size() + size * 3);
        for (size_t i = 0; i < size; ++i)
        {
            float outVertex[3] = {};
            rotate(&m_vertices[ indexBlock[i] * 3 ], orientation, offset, outVertex);
            vertices.insert(vertices.end(), outVertex, outVertex + 3);
        }
    }

    std::vector<float>      m_vertices;
    IMesh::Desc             m_desc;
};

// Space mesh, this mesh will be provided as dummy mesh object
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

// Simple Cube mesh
class CubeMesh : public BaseMesh
{
public:
    CubeMesh()
    {
        const size_t groupsCount = 6;
        short indexGroups[groupsCount][groupsCount] = 
        {
            {2, 3, 4, 2, 4, 5,}, // front  +x
            {1, 6, 4, 1, 4, 3,}, // top    +y
            {0, 1, 2, 2, 1, 3,}, // right  +z
            {1, 0, 7, 1, 7, 6,}, // back   -x
            {0, 2, 7, 2, 5, 7,}, // bottom -y
            {7, 5, 4, 7, 4, 6,}, // left   -z
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
            m_indices[i].assign(indexGroups[i], indexGroups[i] + sizeof(indexGroups[i])/sizeof(short));
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
                copyFaces(out_descriptor, properties.offset, properties.orientation, m_indices[i].data(), m_indices[i].size()); 
            }
        }
    }

private:
    std::vector<uint16_t> m_indices[6];
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> CubeMesh::self(new CubeMesh());

// Simple Wedge mesh
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

        short indexGroups[] = 
        {
            0, 5, 4,          // front  +x
            0, 2, 1, 0, 4, 2, // top    +y
                              //        +z
            1, 2, 3,          // back   -x
            0, 1, 5, 1, 3, 5, // bottom -y
            5, 3, 2, 5, 2, 4, // left   -z
        };

        m_indices.assign(indexGroups, indexGroups + sizeof(indexGroups)/sizeof(short));

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
            copyFaces(out_descriptor, properties.offset, properties.orientation, &m_indices[3], 6);
        }

        const int groups[] = {0, 3, 3, 9, 12, 18};
        const int sizes[] = {3, 6, 6, 3, 6, 6};

        for (size_t i = 0; i < 6; ++i )
        {
            if (flags & (1 << i) && i != DirectionIndices::nY_idx && i != DirectionIndices::nZ_idx)
            {
                copyFaces(out_descriptor, properties.offset, properties.orientation, &m_indices[ groups[i] ], sizes[i]);
            }
        }
    }

private:
    std::vector<uint16_t> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> WedgeMesh::self(new WedgeMesh());

// Wedge angle mesh
class WedgeAngleMesh : public BaseMesh
{
public:
    WedgeAngleMesh()
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

        short indexGroups[] = 
        {
            4, 2, 1,          // front  +x
            4, 3, 2,          // top    +y
            4, 0, 3,          // back   -x
            4, 1, 0,          // left   -z
            0, 1, 3, 1, 2, 3, // bottom -y
        };

        m_indices.assign(indexGroups, indexGroups + sizeof(indexGroups)/sizeof(short));

        m_vertices.assign(vertices, vertices + sizeof(vertices)/sizeof(float));

        ILibrary::library()->RegisterMesh(ElementType::WedgeOutCorner, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        CLONE_HANDLE(IMesh, WedgeAngleMesh);
    };

    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const
    {
        copyFaces(out_descriptor, properties.offset, properties.orientation, m_indices.data(), m_indices.size());
    }

private:
    std::vector<uint16_t> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> WedgeAngleMesh::self(new WedgeAngleMesh());
//eof