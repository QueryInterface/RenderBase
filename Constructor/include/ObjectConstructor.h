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
    public:
        // IElement interface
        const ConstructionDescription& ConstructionDesc() const {return m_desc;};

        Compartment();
        virtual ~Compartment() {};

        void IterrateObject(std::function<void(size_t, size_t, Utils::RangeList<Element>&)> visitor) { m_pillars.for_each(visitor); };

    public:
        void SetElement(const ConstructionDescription& element, const Vector3D& position, Directions direction, bool updateNeighbours);
        void UpdateNeighbourhood(size_t x, size_t y, size_t z);

    private:
        ConstructionDescription                         m_desc;
        Utils::QuadTree< Utils::RangeList<Element> >    m_pillars;

        Compartment(const Compartment& arg);
        const Compartment& operator=(const Compartment& arg);
    };

}//end  of namespace constructor

// eof