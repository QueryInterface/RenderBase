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
        const Mesh& GetMeshObject(unsigned int id, unsigned int flags);
        void RegisterMesh(unsigned int id, unsigned int flags, const Mesh& mesh);

        MeshLibrary();
        virtual ~MeshLibrary() {};

    private: // arguments
        std::vector<Mesh>  m_primitives;
    };
}
//end  of namespace

// eof