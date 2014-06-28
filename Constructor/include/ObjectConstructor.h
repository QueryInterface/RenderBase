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
        Directions                      direction;
        Directions                      secondaryDirection; //secondary direction required for generated elements
        unsigned int                    neighbourhood;
    };
// on a low level object consists from a set of Cores
    class Core : public IConstructable
    {
        ConstructionDescription m_reference;
        typedef Utils::RangeList<Element> Pillar_t;
    public:
        // IConstructable interface
        const ConstructionDescription& ConstructionDesc() const {return m_desc;};
    public:
        Core();
        virtual ~Core() {};

        // Adds element to specified position
        void SetElement(const ConstructionDescription& element, const vector3i_t& position, Directions direction, bool updateNeighbours);

        Element* GetElement(const vector3i_t& position);

        // Updates neighborhoodof component in pos
        void UpdateNeighbourhood(const vector3i_t& pos);

        // Allows to iterrate through Core components
        void IterrateObject(std::function<void(size_t, size_t, size_t, Element&)> visitor);

        // is Outdated
        bool IsOutdated() const {return m_isDirty;}
        void ResetCore() {m_isDirty = false;}

    private:
        const NeighborDesc* findRelation(const Element& item, vector3i_t& direction);
        void morph(const vector3i_t& position, const Element& item);
        vector3i_t rotate(const vector3i_t& vec, Directions dst) const;

    private:
        ConstructionDescription         m_desc;
        Utils::QuadTree< Pillar_t >     m_pillars;

        bool                            m_isDirty;

        Core(const Core& arg);
        const Core& operator=(const Core& arg);
    };

}//end  of namespace constructor

// eof