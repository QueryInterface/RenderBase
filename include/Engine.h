#pragma once

#include "common.h"
#include "MathBasics.h"
#include "Resources.h"
#include "EngineInput.h"
#include <string>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;
using std::string;

struct IScene;
struct IProgram;
struct ILight;
struct ICamera;
struct IObject;
struct IWindow;

enum class LightType
{
    Spot,
    Direct
};

enum class CoordType
{
    Local,
    Global
};

struct IWindowCallbacks 
{
    virtual void OnKeyDown(EKey key) {key;}
    virtual void OnKeyUp(EKey key) {key;}
    virtual void OnMouseDown(EKey key, uint32_t x, uint32_t y) {key;x;y;}
    virtual void OnMouseUp(EKey key, uint32_t  x, uint32_t  y) {key;x;y;}
    virtual void OnMouseMove(uint32_t x, uint32_t y) {x;y;}
    virtual void OnFingerDown(float x, float y, float dx, float dy) {x;y;dx;dy;}
    virtual void OnFingerUp(float x, float y, float dx, float dy) {x;y;dx;dy;}
    virtual void OnFingerMove(float x, float y, float dx, float dy) {x;y;dx;dy;}
    virtual void OnMultiGesture(uint16_t numfingers, float x, float y, float theta, float dist) {numfingers;x;y;theta;dist;}
};

struct ISceneElement : public IHandle
{
    virtual void                SetPosition(CoordType type, const vector3f_t& pos)                  = 0;
    virtual void                SetScale(CoordType type, const vector3f_t& scale)                   = 0;
    virtual void                Shift(CoordType type, const vector3f_t& shift)                      = 0;
    virtual void                Rotate(CoordType type, const vector3f_t& angles)                    = 0;
    virtual void                Rotate(CoordType type, const quat& q)                               = 0;
    // Returns position of object center after local and global transformations
    //virtual const vector3f_t&   GetPosition()                                                       = 0;
    // Returns position of object after global transformation
    virtual const vector3f_t&   GetPosition(CoordType type)                                         = 0;
    // Returns initPosition after specified transformation
    virtual       vector3f_t    GetPosition(CoordType type, const vector3f_t& initPosition)         = 0;
    virtual       vector3f_t    GetDirection(CoordType type, const vector3f_t& initDirection)       = 0;
    virtual const vector3f_t&   GetScale(CoordType type)                                            = 0;
};

struct IWindow
{
    virtual void        SetWidth(uint32_t width)            = 0;
    virtual void        SetHeight(uint32_t height)          = 0;
    virtual void        SetTitle(const std::string& title)  = 0;
    virtual void        SetFullscreen(bool fullscreen)      = 0;

    virtual uint32_t    GetWidth() const                    = 0;
    virtual uint32_t    GetHeight() const                   = 0;
    virtual std::string GetTitle() const                    = 0;
    virtual bool        IsFullscreen() const                = 0;

    virtual void        RegisterInputCallbacks(IWindowCallbacks* callbacks) = 0;
    virtual void        UnregisterInputCallbacks(IWindowCallbacks* callbacks) = 0;
};

// scene interfaces
struct ILight
    : public IClonable< std::shared_ptr<ILight> >
    , public ISceneElement
{
    typedef std::shared_ptr<ILight> Ptr;
};

struct CameraDesc
{
    vector3f_t  EyePosition;
    vector3f_t  Direction;
    vector3f_t  Up;
    float       Aspect;
    float       FieldOfViewY;
    float       NearZ;
    float       FarZ;
};

struct ICamera 
    : public IClonable< std::shared_ptr<ICamera> >
    , public ISceneElement 
{
    typedef std::shared_ptr<ICamera> Ptr;
    virtual const CameraDesc& GetDesc() const = 0;
    virtual void SetFiledOfViewY(float fovy) = 0;
};

struct IObject
    : public ISceneElement
    , public IClonable< std::shared_ptr<IObject> >
{
    typedef std::shared_ptr<IObject> Ptr;

    virtual IMeshPtr        GetMesh() const                                         = 0;
    virtual ITexturePtr     GetTexture() const                                      = 0;

    virtual void            AttachBidirectional(IObject::Ptr object)                  = 0;
    virtual void            AttachDirectional(IObject::Ptr object)                    = 0;
    virtual uint32_t        GetNumAttached() const                                  = 0;
    virtual IObject::Ptr    GetAttached(uint32_t index) const                       = 0;
    virtual void            Detach()                                                = 0;
    virtual void            Detach(IObject::Ptr object)                               = 0;

    static LIB_EXPORT IObject::Ptr CALLING_CONVENTION CreateObject(IMeshPtr mesh, ITexturePtr texture);
};

// Base level of all objects
struct IScene : public IHandle
{
    typedef std::shared_ptr<IScene> Ptr;

    virtual void AddObject(IObject::Ptr& object)              = 0;
    virtual void AddLight(ILight::Ptr& light)                 = 0;
    virtual void SetAmbientLight(const vector3f_t& light)   = 0;
    virtual void SetCamera(ICamera::Ptr& camera)              = 0;
};

struct IEngineCallbacks
{
    virtual void OnSceneUpdate() = 0;
};

struct IEngine
{
    virtual void                   SetScene(IScene::Ptr scene)                           = 0;
    virtual IWindow&               GetWindow() const                                   = 0;
    virtual void                   Run(IEngineCallbacks* callbacks)                    = 0;

    virtual ILight::Ptr            CreateLight(LightType type, vector3f_t position)    = 0;
    virtual ICamera::Ptr           CreateCamera(const CameraDesc& setup)               = 0;
    virtual IScene::Ptr            CreateScene()                                       = 0;

    static LIB_EXPORT IEngine&  CALLING_CONVENTION Instance();
};