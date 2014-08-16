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

struct ObjectProperties
{
    std::string    name;
    std::string    meshName;
    std::string    materialName;
    std::string    elementName;
};

class GameObjectBase
    : public IGameObject
{
public:
    GameObjectBase(std::string name) : m_name(name) {}
    GameObjectBase(ObjectProperties& objProperties) {objProperties;}

    virtual bool     IsComplete() const {return m_isComplete;};
    virtual const std::string& GetName()  const {return m_name;};
    virtual uint32_t GetElementId() const {return 0;};
    virtual uint32_t GetMeshId() const {return 0;};
    
private:
    bool m_isComplete;
    std::string m_name;
    PREVENT_COPY(GameObjectBase)
};


// eof