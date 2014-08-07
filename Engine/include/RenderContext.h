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

class WindowBase : public IWindow 
{
    friend class WindowCallbackHandle;
public:
    WindowBase();
    virtual ~WindowBase() = 0;
    // IWindow
    virtual void        RegisterInputCallbacks(IWindowCallbacks* callbacks) override;
    virtual void        UnregisterInputCallbacks(IWindowCallbacks* callbacks) override;
    // WindowBase
    virtual WINDOW_MSG  ProcessMessage() = 0;
    virtual void        Present() = 0;
protected:
    mutex                m_callbackMutex;
private:
    set<IWindowCallbacks*> m_inputCallbacks;

    PREVENT_COPY(WindowBase);
};

struct IRenderContext : public IHandle
{
	virtual WindowBase& GetWindow() = 0;
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

class WindowSDL final : public WindowBase 
{
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
    EKey                convert(uint16_t sdlKey);

    SDL_Window*    m_window;
    SDL_GLContext  m_glcontext;

    PREVENT_COPY(WindowSDL);
};

class RenderContextGLES2 final : public IRenderContext 
{
public:
	RenderContextGLES2(const RenderContextBuilder* builder);
	virtual ~RenderContextGLES2();
    // IHandle
	virtual void        Release();
	virtual WindowBase& GetWindow() override;
	virtual void        Present() override;
private:
    unique_ptr<WindowBase> m_window;

    PREVENT_COPY(RenderContextGLES2);
};
