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

void Hull::ConstructMesh(Compartment& objectCore)
{
    objectCore.IterrateObject([&](size_t x, size_t y, size_t z, Element& e)
    {
        x;y;z;
        const IMesh& mesh = ILibrary::library()->GetMesh(e.type);
        m_vertices.insert(m_vertices.end(), mesh.GetMeshBuffer().begin(), mesh.GetMeshBuffer().end());
        IndexData_t indices;
        // TODO: I don't like the interface
        mesh.GetIndexData(~e.neighbourhood, indices);
        m_indices.insert(m_indices.end(), indices.begin(), indices.end());
    });
}

void Hull::GetIndexData(unsigned int, IndexData_t& indices) const
{
    indices.assign(m_indices.begin(), m_indices.end());
}
// eof