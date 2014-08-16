/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Resources.h>
#include "MathBasics.h"
#include <vector>
#include <cstdint>

/////////////////////////////////////////////////////////////////////
///
/// Library implementation of Mesh object
///
/////////////////////////////////////////////////////////////////////

struct MeshProperties
{
    uint32_t    flags;
    vector3f_t  offset;
    uint32_t    orientation;
};

struct ILibraryMesh : public IMesh
{
    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const = 0;
    virtual ~ILibraryMesh() {};
};

/////////////////////////////////////////////////////////////////////
///
/// Library implementation of Game object
///
/////////////////////////////////////////////////////////////////////

class GameObjectBase
    : public IGameObject
{
public:
    GameObjectBase(std::string name) : m_name(name) {}
    GameObjectBase(ObjectProperties& objProperties) : m_properties(objProperties){;}

    virtual const ObjectProperties& GetObjectContent()   const {return m_properties;};
    virtual const ObjectResources&  GetObjectResources() const {return m_linkedResources;};

    const std::string& GetName() const {return m_name;}
private:
    bool m_isComplete;
    std::string m_name;

    ObjectProperties m_properties;
    ObjectResources  m_linkedResources;

    PREVENT_COPY(GameObjectBase)
};


// eof