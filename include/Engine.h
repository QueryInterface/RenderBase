#pragma once

#include "common.h"
#include "MathBasics.h"
#include "Resources.h"
#include <string>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;
using std::string;
using std::wstring;

struct IScene;
struct IProgram;
struct ILight;
struct ICamera;
struct IObject;
struct IWindow;

typedef shared_ptr<IScene>              IScenePtr;
typedef shared_ptr<IProgram>            IProgramPtr;
typedef shared_ptr<ILight>              ILightPtr;
typedef shared_ptr<ICamera>             ICameraPtr;
typedef shared_ptr<IObject>             IObjectPtr;

enum class LightType
{
    Spot
};

enum class CoordType
{
    Local,
    World
};

struct InputCallback 
{
    virtual void OnKeyDown(uint32_t keyCode) {keyCode;}
    virtual void OnKeyUp() {}
    virtual void OnMouseDown() {}
    virtual void OnMouseUp() {}
    virtual void OnMouseMove() {}
    virtual void OnFingerDown() {}
    virtual void OnFingerUp() {}
    virtual void OnFingerMove() {}
    virtual void OnMultiGesture() {}
};

struct ISceneElement : public IHandle
{
    virtual void        SetPosition(CoordType type, const vector3f_t& pos)                  = 0;
    virtual void        SetScale(CoordType type, const vector3f_t& scale)                   = 0;
    virtual void        Shift(CoordType type, const vector3f_t& shift)                      = 0;
    virtual void        Rotate(CoordType type, const vector3f_t& angles)                    = 0;
    virtual void        Rotate(CoordType type, const quat& q)                               = 0;

    virtual vector3f_t  GetPosition()                                                       = 0;
    virtual vector3f_t  GetPosition(CoordType type)                                         = 0;
    virtual vector3f_t  GetDirection(CoordType type, const vector3f_t& initDirection)       = 0;
    virtual vector3f_t  GetScale(CoordType type)                                            = 0;
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

    virtual IHandle&    RegisterInputCallback(const std::shared_ptr<InputCallback>& callback) = 0;
};

// scene interfaces
struct ILight
    : public IClonable<ILightPtr>
    , public ISceneElement
{
};

struct CameraSetup
{
    vector3f_t  Eye;
    vector3f_t  At;
    vector3f_t  Up;
    float       FieldOfViewY;
    float       NearZ;
    float       FarZ;
};

struct ICamera 
    : public IClonable<ICameraPtr>
    , public ISceneElement 
{
    virtual const CameraSetup&  GetCameraSetup() const      = 0;
    virtual void                SetFiledOfViewY(float fovy) = 0;
};

struct IObject
    : public ISceneElement
    , public IClonable<IObjectPtr>
{
    virtual IMeshPtr        GetMesh() const                                         = 0;
    virtual ITexturePtr     GetTexture() const                                      = 0;

    virtual void            AttachBidirectional(IObjectPtr object)                  = 0;
    virtual void            AttachDirectional(IObjectPtr object)                    = 0;
    virtual uint32_t        GetNumAttached() const                                  = 0;
    virtual IObjectPtr      GetAttached(uint32_t index) const                       = 0;
    virtual void            Detach()                                                = 0;
    virtual void            Detach(IObjectPtr object)                               = 0;

    static LIB_EXPORT IObjectPtr CALLING_CONVENTION CreateObject(IMeshPtr mesh, ITexturePtr texture);
};

// Base level of all objects
struct IScene : public IHandle
{
    virtual void AddObject(IObjectPtr& object)  = 0;
    virtual void AddLight(ILightPtr& light)     = 0;
    virtual void SetCamera(ICameraPtr& camera)  = 0;

    virtual void Render()                       = 0;
};

struct IEngineCallbacks
{
    virtual void OnSceneUpdate() = 0;
};

struct IEngine
{
    virtual void                    SetScene(IScenePtr scene)                           = 0;
    virtual IWindow&                GetWindow() const                                   = 0;
    virtual void                    Run(IEngineCallbacks* callbacks)                    = 0;

    virtual ILightPtr               CreateLight(LightType type, vector3f_t position)    = 0;
    virtual ICameraPtr              CreateCamera(const CameraSetup& setup)              = 0;
    virtual IScenePtr               CreateScene()                                       = 0;

    static LIB_EXPORT IEngine&  CALLING_CONVENTION Instance();
};