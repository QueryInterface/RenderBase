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
        const IMesh& GetMeshObject(unsigned int id);
        void RegisterMesh(unsigned int id, const IMesh& mesh);

        MeshLibrary();
        virtual ~MeshLibrary() {};

    private: // arguments
        std::vector<const IMesh*>  m_primitives;
    };
}
//end  of namespace

// eof