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
        typedef std::vector<LayoutItem>     LayoutData_t;
        typedef std::vector<float>          VertexData_t;
        typedef std::vector<unsigned int>   IndexData_t;
    public:
        // IMesh interface
        virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const;

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

        vector3f_t rotate(const vector3f_t& vec, Directions dst) const;

        LayoutData_t                    m_layout;
        VertexData_t                    m_vertices;
        IndexData_t                     m_indices;

        Hull(const Hull& arg);
        const Hull& operator=(const Hull& arg);
    };

}//end  of namespace constructor

// eof