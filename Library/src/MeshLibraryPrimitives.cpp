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

    virtual void Release() {};

protected:
    std::vector<float>      m_vertices;
    std::vector<LayoutItem> m_layout;
};

// Space mesh, this mesh will be provided as dummy mesh object
class SpaceMesh : public BaseMesh
{
public:
    SpaceMesh() { ILibrary::library()->RegisterMesh(ElementType::Space, *this); };
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
            if (flags & (1 << i))
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
        return std::make_shared<WedgeMesh>(*this);
    };

    virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const 
    {
        flags;
        out_descriptor.groups.clear();
        LayoutItem li;
        li.layoutType = LayoutType::Vertices;
        li.itemSize = 3;
        li.itemsCount = m_vertices.size();
        li.items = (float*)m_vertices.data();
        out_descriptor.layout.push_back(li);

        IndexGroup mc;

        // pY, pZ
        if (flags & Directions::pX || flags & (size_t)Directions::nX || flags & (size_t)Directions::pZ || flags & (size_t)Directions::pY)
        {
            mc.indices = &m_indices[3];
            mc.count = 6;
            out_descriptor.groups.push_back(mc);
        }

        const int groups[] = {0, 3, 3, 9, 12, 18};
        const int sizes[] = {3, 6, 6, 3, 6, 6};

        for (size_t i = 0; i < 6; ++i )
        {
            if (flags & (1 << i) && i != 1 && i != 2)
            {
                IndexGroup mc;
                mc.indices = &m_indices[ groups[i] ];
                mc.count = sizes[i];
                out_descriptor.groups.push_back(mc);
            }
        }
    }

private:
    std::vector<unsigned int> m_indices;
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

        ILibrary::library()->RegisterMesh(ElementType::WedgedAngle, *this);
    }

    virtual IMeshPtr Clone() const 
    {
        return std::make_shared<WedgeAngleMesh>(*this);
    };

    virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const 
    {
        flags;
        out_descriptor.groups.clear();
        LayoutItem li;
        li.layoutType = LayoutType::Vertices;
        li.itemSize = 3;
        li.itemsCount = m_vertices.size();
        li.items = (float*)m_vertices.data();
        out_descriptor.layout.push_back(li);

        IndexGroup mc;
        mc.indices = m_indices.data();
        mc.count = m_indices.size();
        out_descriptor.groups.push_back(mc);

    }

private:
    std::vector<unsigned int> m_indices;
    static std::unique_ptr<IMesh> self;
};
std::unique_ptr<IMesh> WedgeAngleMesh::self(new WedgeAngleMesh());
//eof