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
        const IMesh::Desc* GetDesc() const override;

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

        IMesh::Desc m_desc;

        Hull(const Hull& arg);
        const Hull& operator=(const Hull& arg);
    };

}//end  of namespace constructor

// eof