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

struct ISceneElement;
struct ISceneElementHandle;
struct IScene;

struct IProgram;
struct ILight;
struct ICamera;

struct IWindow;
struct IEngine;

typedef shared_ptr<ISceneElement>       ISceneElementPtr;
typedef shared_ptr<ISceneElementHandle> ISceneElementHandlePtr;
typedef shared_ptr<IScene>              IScenePtr;

typedef shared_ptr<IProgram>            IProgramPtr;
typedef shared_ptr<ILight>              ILightPtr;
typedef shared_ptr<ICamera>             ICameraPtr;

typedef shared_ptr<IWindow>             IWindowPtr;
typedef shared_ptr<IEngine>             IEnginePtr;

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
{
    // Sets
    virtual void            SetPosition(Vector3<float> pos) = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const             = 0;    
};

struct ICamera
    : public IClonable<ICamera>
    , public ISceneElement 
{

};

// Base level of all objects
struct IScene
{
    virtual ISceneElementHandlePtr AddSceneElement(ISceneElementPtr obj) = 0;
};

struct IEngine : public IHandle 
{
    virtual void                    SetScene(IScenePtr scene)   = 0;
    virtual IWindowPtr              GetWindow() const           = 0;
    virtual void                    Run()                       = 0;
    static IEnginePtr               Create();
};