/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//
// the class is responcible for:
//    - create element based structure
//    - split structure on separate solid modules
//    - create skeleton for object
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Constructor.h"

#include "include/QuadTree.h"
#include "include/RangeList.h"
#include <vector>
#include <list>
#include <memory>

#include "Resources.h"

namespace ConstructorImpl
{
    struct Element
    {
        ElementType     type;
        Directions      direction;
        unsigned int    neighbourhood;
    };
// on a low level object consists from a set of compartments
    class Compartment : public IConstructable , public IMesh
    {
        typedef Utils::RangeList<Element> Pillar_t;
    public:
        // IConstructable interface
        const ConstructionDescription& ConstructionDesc() const {return m_desc;};
    
        // IMesh interface
        virtual const LayoutData_t& GetLayout()     const {return m_layout;};
        virtual const VertexData_t& GetMeshBuffer() const {return m_vertices;};
        virtual void GetIndexData(unsigned int, IndexData_t&) const;

        // IHandle interface
        virtual void Release() {};

        //IClonable interface
        virtual IMeshPtr Clone() const { return nullptr;};
    public:
        Compartment();
        virtual ~Compartment() {};

        // Adds element to specified position
        void SetElement(const ConstructionDescription& element, const Vector3D& position, Directions direction, bool updateNeighbours);

        // Construct mesh for object
        void ConstructMesh();

        // Updates neighborhoodof component in position x, y, z
        void UpdateNeighbourhood(size_t x, size_t y, size_t z);

        // Allows to iterrate through compartment components
        void IterrateObject(std::function<void(size_t, size_t, size_t, Element&)> visitor);

    private:
        ConstructionDescription         m_desc;
        Utils::QuadTree< Pillar_t >     m_pillars;

        LayoutData_t                    m_layout;
        VertexData_t                    m_vertices;
        IndexData_t                     m_indices;

        Compartment(const Compartment& arg);
        const Compartment& operator=(const Compartment& arg);
    };

}//end  of namespace constructor

// eof