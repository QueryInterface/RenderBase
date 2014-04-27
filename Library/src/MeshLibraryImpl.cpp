#include "MeshLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

MeshLibrary::MeshLibrary() 
{
    m_primitives.resize(ElementType::SimplePrimitivesCount);
//    m_dummy.primitiveUID = ElementType::SimplePrimitivesCount;
}
/*
void MeshLibrary::RegisterPrimitiveMesh(unsigned int id, const Mesh& prototype)
{
    m_primitives[id] = prototype;
}
*/
const Mesh& MeshLibrary::GetMeshObject(unsigned int id, unsigned int flags)
{
    flags;
    return m_primitives[id];
    //return (type >= m_primitives.size()) ? m_dummy : m_primitives[type]->ConstructionDesc();
}

void MeshLibrary::RegisterMesh(unsigned int id, unsigned int flags, const Mesh& mesh)
{
    id; flags; mesh;
}

#include "MeshLibraryPrimitives.cpp"
// eof