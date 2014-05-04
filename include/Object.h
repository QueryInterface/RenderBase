#pragma once
#include "common.h"
#include "MathBasics.h"
#include <memory>

using std::shared_ptr;

// Specifies elements that could be added to scene graph
// There are a few ways to attach object:
// 1. Bidirectional attachment (rigid connection) - move of one object causes move of another one
// 2. Directiona attachmant - A->B move of object A causes move of object B, move of object B doesn't cause move of object A 

struct IObject;
typedef shared_ptr<IObject> IObjectPtr;

struct IObject
    : public ISceneElement
    , public IClonable<IObjectPtr>
{
    virtual void            SetPosition(const Vector3<float>& pos)                  = 0;
    virtual Vector3<float>  GetPosition() const                                     = 0;

    virtual void            Shift(const Vector3<float>& shift)                      = 0;
    //virtual void            Rotate(const Vector3<float>& angles)                    = 0;

    virtual void            AttachBidirectional(IObjectPtr object)= 0;
    virtual void            AttachDirectional(IObjectPtr object)  = 0;
    virtual uint32_t        GetNumAttached() const                                  = 0;
    virtual IObjectPtr      GetAttached(uint32_t index) const                       = 0;
    virtual void            Detach()                                                = 0;
    virtual void            Detach(IObjectPtr object)             = 0;
};