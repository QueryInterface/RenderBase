#pragma once
#include "common.h"
#include "Engine.h"
#include <list>
#include <set>
#include <mutex>

#include "SDL.h"
#include "SDL_opengles2.h"
#include "SDL_egl.h"

using std::list;
using std::set;
using std::mutex;

class RenderContextGLES2;
class WindowBase;
class RenderContextBuilder;

struct IRenderContext;

typedef shared_ptr<RenderContextBuilder> RenderContextBuilderPtr;
typedef shared_ptr<IRenderContext> IRenderContextPtr;

enum class WINDOW_MSG 
{
    FOREGROUND,
    BACKGROUND,
    QUIT
};

class WindowCallbackHandle : public IHandle 
{
public:
    WindowCallbackHandle(WindowBase* windowContext, list< shared_ptr<InputCallback> >::iterator& iter);
    virtual ~WindowCallbackHandle();
private:
    WindowBase*                                 _window;
    list< shared_ptr<InputCallback> >::iterator _iter;

    PREVENT_COPY(WindowCallbackHandle);
};

class WindowBase : public IWindow 
{
    friend class WindowCallbackHandle;
public:
    WindowBase();
    virtual ~WindowBase() = 0;
    // IWindow
    virtual IHandle*        RegisterInputCallback(const std::shared_ptr<InputCallback>& callback) override;
    // WindowBase
    virtual WINDOW_MSG      ProcessMessage() = 0;
    virtual void            Present() = 0;
private:
    mutex                               _callbackMutex;
    list< shared_ptr<InputCallback> >   _InputCallbacks;

    void _eraseCallback(list< shared_ptr<InputCallback> >::iterator& iter);

    PREVENT_COPY(WindowBase);
};

struct IRenderContext : public IHandle
{
	virtual WindowBase* GetWindow() = 0;
	virtual void        Present()   = 0;
};

class RenderContextBuilder
{
public:
    RenderContextBuilder();
    virtual ~RenderContextBuilder();

    void            SetWidth(uint32_t width);
    void            SetHeight(uint32_t height);
    void            SetTitle(const std::string& name);
    void            SetFullscreen(bool fullscreen);
    uint32_t        GetWidth() const;
    uint32_t        GetHeight() const;
    std::string     GetTitle() const;
    bool            IsFullscreen() const;

    IRenderContextPtr GetResult();

    static RenderContextBuilderPtr Create();
private:
    uint32_t        m_width;
    uint32_t        m_height;
    std::string     m_title;
    bool            m_fullscreen;
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
	virtual WindowBase* GetWindow() override;
	virtual void        Present() override;
private:
    unique_ptr<WindowBase> _window;

    PREVENT_COPY(RenderContextGLES2);
};
