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
        const ConstructionDescription*  construction;
        unsigned int                    type;
        unsigned int                    direction;
        unsigned int                    originalDirection; //secondary direction required for generated elements
        unsigned int                    neighbourhood;
    };

    typedef Utils::RangeList<Element> Pillar_t;

    class Core : public IConstructable
    {
    public:
        Core();
        virtual ~Core() {};

        ///////////////////////////////////////////////////////////////////////////////////
        // IConstructable interface
        const ConstructionDescription& ConstructionDesc() const {return m_desc;};

        ///////////////////////////////////////////////////////////////////////////////////
        // Adds element to specified position
        void SetElement(const ConstructionDescription& element, const vector3i_t& position, Directions direction, bool updateNeighbours);

        ///////////////////////////////////////////////////////////////////////////////////
        // Returns element desc on requested position
        // the function is not a constant, so element can be modified
        Element* GetElement(const vector3i_t& position);

        ///////////////////////////////////////////////////////////////////////////////////
        // Updates neighborhood of component in pos
        void UpdateNeighbourhood(const vector3i_t& pos);

        ///////////////////////////////////////////////////////////////////////////////////
        // Allows to iterrate through Core components
        void IterrateObject(std::function<void(size_t, size_t, size_t, Element&)> visitor);

        ///////////////////////////////////////////////////////////////////////////////////
        // Indicates that geometry was updated since last request
        bool IsUpdated();

    private:
        ///////////////////////////////////////////////////////////////////////////////////
        // looks for relative element of item. Item will be searched in direction
        const NeighborDesc* findNeighbor(const Element& item, const vector3i_t& direction) const;

        ///////////////////////////////////////////////////////////////////////////////////
        // converts self or neighbor to "joint/connector" objects, angle joints etc.
        void morph(const vector3i_t& position, Element& item);

        ///////////////////////////////////////////////////////////////////////////////////
        // fast rotate operation on multiple by Pi/2 angles
        vector3i_t rotate(const vector3i_t& vec, unsigned int dst) const;

    private:
        ConstructionDescription      m_desc;
        Utils::QuadTree< Pillar_t >  m_pillars;

        bool                         m_isDirty;

        ConstructionDescription      m_reference;
        Core(const Core& arg);
        const Core& operator=(const Core& arg);
    };

}//end  of namespace constructor

// eof