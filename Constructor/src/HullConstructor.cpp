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
        for (size_t i = 0; i < desc.groups.size(); ++i)
            m_indices.insert(m_indices.end(), desc.groups[i].indices, desc.groups[i].indices + desc.groups[i].count);
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