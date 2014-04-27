#include "MeshLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

MeshLibrary::MeshLibrary() 
{
    m_primitives.resize(ElementType::SimplePrimitivesCount);
}

const IMesh& MeshLibrary::GetMeshObject(unsigned int id)
{
    return *m_primitives[id];
}

void MeshLibrary::RegisterMesh(unsigned int id, const IMesh& mesh)
{
    m_primitives[id] = &mesh;
}

#include "MeshLibraryPrimitives.cpp"
// eof