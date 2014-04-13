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

#include "ConstructorTypes.h"
#include "include/QuadTree.h"
#include <vector>
#include <list>
#include <memory>

namespace Constructor
{
// a vertical chunk of elements
    class Pillar
    {
        struct Chunk
        {
            unsigned int                        from;
            std::vector<ElementType>            elements; // if typed is ET_Space, elements are stored in this vector
        };
        std::list< Chunk > m_chunks;

    public:
        Pillar() {};
        ~Pillar() {};

        ElementType&        item(size_t Z);
        const ElementType*  get_item_at(size_t Z) const;
    };

// on a low level object consists from a set of compartments
    class Compartment : public IConstructable
    {
    public:
        // IElement interface
        const ElementDescription& GetObjectDescription() const {return m_desc;};
        Compartment();
        virtual ~Compartment() {};

    public:
        void SetElement(ElementType type, const Vector3D& position, Directions direction);

    private:
        ElementDescription                          m_desc;
        Utils::QuadTree< Pillar >  m_pillars;

        Compartment(const Compartment& arg);
        const Compartment& operator=(const Compartment& arg);
    };

///////////////////////////////////////////////////////////////////////////////////////////////
//
// primary building berth.
//
///////////////////////////////////////////////////////////////////////////////////////////////
    class BuildingBerth
    {
    public:
        BuildingBerth() {};
        ~BuildingBerth() {};

        // Attaches element to Complartment. if new compartment was created function returns true;
        bool SetElement(ElementType type, const Vector3D& position, Directions direction);

//        size_t GetCompartmentsCount() const {return m_compartments.size();}
        const Compartment& GetCompartment() const;
    private:
        Compartment m_compartment;
    };

}//end  of namespace constructor

// eof