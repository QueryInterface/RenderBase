#pragma once

#include "common.h"
#include "MathBasics.h"
#include <string>
#include <memory>
#include <vector>
#include "Object.h"

using std::shared_ptr;
using std::string;
using std::vector;

using std::shared_ptr;
using std::string;
using std::wstring;

struct IScene;

struct IProgram;
struct ILight;
struct ICamera;

struct IWindow;

typedef shared_ptr<IScene>              IScenePtr;

typedef shared_ptr<IProgram>            IProgramPtr;
typedef shared_ptr<ILight>              ILightPtr;
typedef shared_ptr<ICamera>             ICameraPtr;

struct EventCallback 
{
    virtual void OnKeyPress(uint32_t keyCode, bool isPress) {keyCode; isPress;}
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

    virtual IHandle*    RegisterEventCallback(const std::shared_ptr<EventCallback>& callback) = 0;
};

// scene interfaces
struct ILight
    : public IClonable<ILightPtr>
    , public ISceneElement 
{
    // Sets
    virtual void            SetPosition(vector3d pos) = 0;
    virtual void            SetPosition(float x, float y, float z) = 0;
    // Gets
    virtual vector3d GetPosition() const             = 0;    
};

struct ICamera 
    : public IClonable<ICameraPtr>
    , public ISceneElement 
{
};

// Base level of all objects
struct IScene : public IHandle
{
    virtual void AddObject(IObjectPtr& object)  = 0;
    virtual void AddLight(ILightPtr& light)     = 0;
    virtual void SetCamera(ICameraPtr& camera)  = 0;

    virtual void Render() const                 = 0;
};

struct IEngine
{
    virtual void                    SetScene(IScenePtr scene)                           = 0;
    virtual IWindow*                GetWindow() const                                   = 0;
    virtual void                    Run()                                               = 0;

    virtual ILightPtr               CreateLight()                                       = 0;
    virtual ICameraPtr              CreateCamera(vector3d eye, vector3d at, vector3d up)= 0;
    virtual IScenePtr               CreateScene()                                       = 0;

    static LIB_EXPORT IEngine*  CALLING_CONVENTION Instance();
};