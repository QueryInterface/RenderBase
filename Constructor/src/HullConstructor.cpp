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
        GeometryDesc desc;
        size_t blockStart = m_indices.size();
        ILibrary::library()->GetMesh(e.construction->primitiveUID).GetGeometryDesc(~e.neighbourhood, desc);
        for (auto group : desc.groups)
        {
            m_indices.insert(m_indices.end(), group.indices, group.indices + group.count);
        }
        for (size_t i = blockStart; i < m_indices.size(); ++i)
        {
            size_t idx = m_indices[i] * 3;
            m_vertices.push_back(desc.layout[(size_t)LayoutType::Vertices].items[idx + 0] + x);
            m_vertices.push_back(desc.layout[(size_t)LayoutType::Vertices].items[idx + 1] + y);
            m_vertices.push_back(desc.layout[(size_t)LayoutType::Vertices].items[idx + 2] + z);
        }
    });
}

void Hull::GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const
{
    flags;
    IndexGroup mc;
    mc.indices = m_indices.data();
    mc.count = m_indices.size();
    out_descriptor.groups.push_back(mc);

    LayoutItem li;
    li.items = (float*)m_vertices.data();
    li.itemSize = 3;
    li.itemsCount = m_vertices.size();
    out_descriptor.layout.push_back(li);
}
// eof