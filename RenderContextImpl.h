#pragma once
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif // _DEBUG

#include "RenderContext.h"
#include <list>
#include <set>
#include <mutex>


using std::list;
using std::set;
using std::unique_ptr;
using std::mutex;

class RenderContextGLES2;
class WindowBase;
class WindowQT;

class WindowCallbackHandle : public IHandle {
public:
    WindowCallbackHandle(WindowBase* windowContext, list< shared_ptr<EventCallback> >::iterator& iter);
    virtual ~WindowCallbackHandle();
    // IHandle
    virtual void Release();
private:
    WindowBase*                                 _window;
    list< shared_ptr<EventCallback> >::iterator _iter;
};

class RenderContextBuilder : public IRenderContextBuilder {
public:
    RenderContextBuilder();
    virtual ~RenderContextBuilder();
    // IHandle
    virtual void Release();
    // IWindowSetup
    virtual void            SetWidth(uint32_t width);
    virtual void            SetHeight(uint32_t height);
    virtual void            SetTitle(const std::string& name);
    virtual void            SetFullscreen(bool fullscreen);
    virtual uint32_t        GetWidth() const;
    virtual uint32_t        GetHeight() const;
    virtual std::string     GetTitle() const;
    virtual bool            IsFullscreen() const;
    // IRenderContextBuilder
    virtual void            SetApiType(RC_API_TYPE apiType);
    virtual RC_API_TYPE     GetApiType() const;
    virtual IRenderContext* GetResult();
private:
    uint32_t        _width;
    uint32_t        _height;
    std::string    _title;
    bool            _fullscreen;
    RC_API_TYPE     _apiType;
};

class WindowBase : public IWindow {
    friend class WindowCallbackHandle;
public:
    WindowBase(const RenderContextBuilder* builder);
    virtual ~WindowBase() = 0;
    // IWindow
    virtual void            SetWidth(uint32_t width);
    virtual void            SetHeight(uint32_t height);
    virtual void            SetTitle(const std::string& name);
    virtual void            SetFullscreen(bool fullscreen);
    virtual uint32_t        GetWidth() const;
    virtual uint32_t        GetHeight() const;
    virtual std::string     GetTitle() const;
    virtual bool            IsFullscreen() const;
    virtual IHandle*        RegisterEventCallback(const std::shared_ptr<EventCallback>& callback);
private:
    uint32_t                            _width;
    uint32_t                            _height;
    std::string                         _title;
    bool                                _fullscreen;
    mutex                               _callbackMutex;
    list< shared_ptr<EventCallback> >   _eventCallbacks;

    void _eraseCallback(list< shared_ptr<EventCallback> >::iterator& iter);
};

class WindowQT : public WindowBase {
    friend WindowCallbackHandle;
public:
    WindowQT(const RenderContextBuilder* builder);
    virtual ~WindowQT();
    // IWindow
    virtual void            SetWidth(uint32_t width);
    virtual void            SetHeight(uint32_t height);
    virtual void            SetTitle(const std::string& name);
    virtual void            SetFullscreen(bool fullscreen);
    virtual bool            ProcessMessage();
private:
};

class RenderContextGLES2 : public IRenderContext {
public:
    RenderContextGLES2(const RenderContextBuilder* builder);
    virtual ~RenderContextGLES2();
    // IHandle
    virtual void        Release();
    // IRenderContext
    virtual IWindow*    GetWindow();
    virtual void        Clear(uint32_t flags, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float depth, uint8_t stencil);
    virtual void        Present();
private:
    unique_ptr<IWindow> _windowContext;

};