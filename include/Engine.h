#pragma once

#include "common.h"
#include "MathBasics.h"
#include <string>
#include <memory>
#include <vector>

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
    : public IClonable<ILight>
    , public ISceneElement 
    , public IHandle
{
    // Sets
    virtual void            SetPosition(Vector3<float> pos) = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const             = 0;    
};

struct ICamera 
    : public IClonable<ICamera>
    , public ISceneElement 
    , public IHandle
{

};

// Base level of all objects
struct IScene : public IHandle
{
};

struct IEngine
{
    virtual void                    SetScene(IScenePtr scene)   = 0;
    virtual IWindow*                GetWindow() const           = 0;
    virtual void                    Run()                       = 0;

    static LIB_EXPORT IEngine*  CALLING_CONVENTION Instance();
};