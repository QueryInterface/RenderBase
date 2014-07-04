#include "HullConstructor.h"
#include "ObjectConstructor.h"
#include "Library.h"
#include <assert.h>

using namespace ConstructorImpl;

#ifndef min 
#define min(a, b) (a)<(b) ? (a) : (b)
#endif

#ifndef max 
#define max(a, b) (a)>(b) ? (a) : (b)
#endif

Hull::Hull() 
{
    m_desc.Shapes.resize(1);
    m_desc.Shapes[0].Indices.ElementSize = 1;
    m_desc.Shapes[0].Positions.ElementSize = 3;
    m_desc.Shapes[0].Positions.LayoutType = IMesh::LayoutType::Triangle;
}

vector3f_t Hull::rotate(const vector3f_t& vec, Directions dst) const
{
    switch(dst)
    {
    case Directions::nX : return vector3f_t(1-vec.z, vec.y,   vec.x);
    case Directions::pX : return vector3f_t(  vec.z, vec.y, 1-vec.x);
    case Directions::nZ : return vector3f_t(1-vec.x, vec.y, 1-vec.z);
    }
    return vec;
}

void Hull::ConstructMesh(Core& objectCore)
{
    objectCore.IterrateObject([&](size_t x, size_t y, size_t z, Element& e)
    {
        ILibraryMesh::GeometryDesc desc;
        auto& indices = m_desc.Shapes[0].Indices.Data;
        size_t blockStart = indices.size();
        ILibrary::library()->GetMesh(e.construction->primitiveUID).GetGeometryDesc(~e.neighbourhood, desc);
        for (auto group : desc.groups)
        {
            indices.insert(indices.end(), group.indices, group.indices + group.count);
        }
        for (size_t i = blockStart; i < indices.size(); ++i)
        {
            size_t idx = indices[i] * 3;
            float* v = &desc.layout[(size_t)ILibraryMesh::LayoutType::Vertices].items[idx];
            vector3f_t vertex = rotate(vector3f_t(v[0], v[1], v[2]), e.direction);

            m_desc.Shapes[0].Positions.Data.push_back(vertex.x + x);
            m_desc.Shapes[0].Positions.Data.push_back(vertex.y + y);
            m_desc.Shapes[0].Positions.Data.push_back(vertex.z + z);
        }
    });
}

const IMesh::Desc* Hull::GetDesc() const
{
    return &m_desc;
}
// eof