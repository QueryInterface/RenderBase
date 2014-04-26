#include "MeshLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

MeshLibrary::MeshLibrary() 
{
//    m_primitives.resize(ET_SimplePrimitivesCount);
//    m_dummy.primitiveUID = ET_SimplePrimitivesCount;
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

// eof