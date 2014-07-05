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
    m_hullDescription.vertices.clear();
    objectCore.IterrateObject([&](size_t x, size_t y, size_t z, Element& e)
    {
        MeshProperties prop = {~e.neighbourhood, vector3f_t(x,y,z), e.direction};
        ILibrary::library()->GetMesh(e.construction->primitiveUID).ConstructGeometry(prop, m_hullDescription);
    });
}

void Hull::GetGeometryDesc(GeometryDesc& out_descriptor) const
{
    IndexGroup mc = {0, 0};
    mc.indices = nullptr;
    mc.count = 0;
    out_descriptor.groups.push_back(mc);

    LayoutItem li;
    li.items = (float*)m_hullDescription.vertices.data();
    li.itemSize = 3;
    li.itemsCount = m_hullDescription.vertices.size();
    out_descriptor.layout.push_back(li);
}
// eof