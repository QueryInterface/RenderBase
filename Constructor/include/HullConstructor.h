/////////////////////////////////////////////////////////////////////////////////
//
// class HullConstructor generates mesh for object created by Object constructor.
//    the class doesn't generate mesh
//
// the class is responcible for:
//    - building and optimizing mesh using object core
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Constructor.h"

#include <list>
#include <memory>

#include "Resources.h"

namespace ConstructorImpl
{
    class Core;
// on a low level object consists from a set of Cores
    class Hull : public IMesh
    {
    public:
        // IMesh interface
        virtual const LayoutData_t& GetLayout()     const {return m_layout;};
        virtual const VertexData_t& GetMeshBuffer() const {return m_vertices;};
        virtual void GetIndexData(unsigned int, IndexData_t&) const;

        // IHandle interface
        virtual void Release() {};

        //IClonable interface
        virtual IMeshPtr Clone() const { return nullptr;};
    public:
        Hull();
        virtual ~Hull() {};

        // Construct mesh for object
        void ConstructMesh(Core& objectCore);

    private:
        LayoutData_t                    m_layout;
        VertexData_t                    m_vertices;
        IndexData_t                     m_indices;

        Hull(const Hull& arg);
        const Hull& operator=(const Hull& arg);
    };

}//end  of namespace constructor

// eof