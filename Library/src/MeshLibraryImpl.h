/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Library.h"
#include <vector>

/////////////////////////////////////////////////////////////////////
///
/// object library public interface. Library is a singletone
///
/////////////////////////////////////////////////////////////////////
namespace LibraryImpl
{
    class MeshLibrary
    {
    public:
        const ILibraryMesh& GetMeshObject(unsigned int id);
        void RegisterMesh(unsigned int id, const ILibraryMesh& mesh);

        MeshLibrary();
        virtual ~MeshLibrary() {};

    private: // arguments
        std::vector<const ILibraryMesh*>  m_primitives;
    };
}
//end  of namespace

// eof