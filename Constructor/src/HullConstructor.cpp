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
}

void Hull::ConstructMesh(Core& objectCore)
{
    objectCore.IterrateObject([&](size_t x, size_t y, size_t z, Element& e)
    {
        x;y;z;
        GeometryDesc desc;
        ILibrary::library()->GetMesh(e.type).GetGeometryDesc(~e.neighbourhood, desc);
        for (auto group : desc.groups)
        {
            m_indices.insert(m_indices.end(), group.indices, group.indices + group.count);
            for (size_t j = 0; j < group.count; ++j)
            {
                //m_vertices.insert(m_vertices.end(), &group.geometry[group.indices[j] * 3], &group.geometry[group.indices[j] * 3] + 3);
            }
        }
    });
}

void Hull::GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const
{
    flags;
    MeshComponent mc;
    mc.indices = m_indices.data();
    mc.count = m_indices.size();
    out_descriptor.groups.push_back(mc);
}
// eof