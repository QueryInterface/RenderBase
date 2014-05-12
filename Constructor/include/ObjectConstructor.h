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
    class Compartment : public IConstructable
    {
        typedef Utils::RangeList<Element> Pillar_t;
    public:
        // IConstructable interface
        const ConstructionDescription& ConstructionDesc() const {return m_desc;};
    public:
        Compartment();
        virtual ~Compartment() {};

        // Adds element to specified position
        void SetElement(const ConstructionDescription& element, const Vector3D& position, Directions direction, bool updateNeighbours);

        // Updates neighborhoodof component in position x, y, z
        void UpdateNeighbourhood(size_t x, size_t y, size_t z);

        // Allows to iterrate through compartment components
        void IterrateObject(std::function<void(size_t, size_t, size_t, Element&)> visitor);

        // is Outdated
        bool IsOutdated() const {return m_isDirty;}
        void ResetCore() {m_isDirty = false;}

    private:
        ConstructionDescription         m_desc;
        Utils::QuadTree< Pillar_t >     m_pillars;

        bool                            m_isDirty;
    private:
        Compartment(const Compartment& arg);
        const Compartment& operator=(const Compartment& arg);
    };

}//end  of namespace constructor

// eof