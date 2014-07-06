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

void Hull::ConstructMesh(Core& objectCore)
{
    m_hullDescription.vertices.clear();
    objectCore.IterrateObject([&](size_t x, size_t y, size_t z, Element& e)
    {
        MeshProperties prop = {~e.neighbourhood, vector3f_t(x,y,z), e.direction};
        ILibrary::library()->GetMesh(e.construction->primitiveUID).ConstructGeometry(prop, m_hullDescription);
        m_desc.Shapes[0].Positions.Data = m_hullDescription.vertices;
    });
}

const IMesh::Desc& Hull::GetDesc() const
{
    return m_desc;
}
// eof