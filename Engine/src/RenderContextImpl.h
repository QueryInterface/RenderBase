#pragma once
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif // _DEBUG

#include "Engine.h"
#include <list>
#include <set>
#include <mutex>

#include "SDL.h"

using std::list;
using std::set;
using std::unique_ptr;
using std::mutex;

class RenderContextGLES2;
class WindowBase;
class WindowQT;

enum class RC_API_TYPE {
    GLES2 = 0,
};

enum class WINDOW_MSG {
    FOREGROUND,
    BACKGROUND,
    QUIT
};

struct IRenderContext
{
};

struct IRenderContextBuilder 
    : public IHandle {

    virtual void            SetWidth(uint32_t width) = 0;
    virtual void            SetHeight(uint32_t height) = 0;
    virtual void            SetTitle(const std::string& title) = 0;
    virtual void            SetFullscreen(bool fullscreen) = 0;
    virtual void            SetApiType(RC_API_TYPE apiType) = 0;

    virtual IRenderContext* GetResult() = 0;

    static LIB_EXPORT IRenderContextBuilder* CALLING_CONVENTION Create();
};


class WindowCallbackHandle : public IHandle {
public:
    WindowCallbackHandle(WindowBase* windowContext, list< shared_ptr<EventCallback> >::iterator& iter);
    virtual ~WindowCallbackHandle();
    // IHandle
    virtual void Release();
private:
    WindowBase*                                 _window;
    list< shared_ptr<EventCallback> >::iterator _iter;

    PREVENT_COPY(WindowCallbackHandle);
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
    WindowBase();
    virtual ~WindowBase() = 0;
    // IWindow
    virtual IHandle*        RegisterEventCallback(const std::shared_ptr<EventCallback>& callback);
    // WindowBase
    virtual void            Present() = 0;
private:
    mutex                               _callbackMutex;
    list< shared_ptr<EventCallback> >   _eventCallbacks;

    void _eraseCallback(list< shared_ptr<EventCallback> >::iterator& iter);

    PREVENT_COPY(WindowBase);
};

class WindowSDL final : public WindowBase {
public:
    WindowSDL(const RenderContextBuilder* builder);
    virtual ~WindowSDL();
    // IWindow
    virtual void        SetWidth(uint32_t width) override;
    virtual void        SetHeight(uint32_t height) override;
    virtual void        SetTitle(const std::string& name) override;
    virtual void        SetFullscreen(bool fullscreen) override;
    virtual uint32_t    GetWidth() const override;
    virtual uint32_t    GetHeight() const override;
    virtual std::string GetTitle() const override;
    virtual bool        IsFullscreen() const override;
    // WindowSDL
    virtual WINDOW_MSG  ProcessMessage();
    virtual void        Present();
private:
    SDL_Window*    _window;
    SDL_GLContext  _glcontext;
};

class RenderContextGLES2 final : public IRenderContext {
public:
	RenderContextGLES2(const RenderContextBuilder* builder);
	virtual ~RenderContextGLES2();
    // IHandle
	virtual void        Release();
	virtual IWindow*    GetWindow();
	virtual void        Present();
private:
    unique_ptr<WindowBase> _window;

    PREVENT_COPY(RenderContextGLES2);
};
