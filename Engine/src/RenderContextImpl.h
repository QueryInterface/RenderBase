#pragma once
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif // _DEBUG

#include "RenderContext.h"
#include <list>
#include <set>
#include <mutex>

#ifdef USE_QT_LIBS
#pragma warning(push)
#pragma warning (disable : 4127)
#pragma warning (disable : 4512)
#include "QtGui/QWindow.h"
#include <QGuiApplication>
#pragma warning(pop)
#else
#include "SDL.h"
#endif //USE_QT_LIBS

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

#ifdef USE_QT_LIBS
class WindowQT final
    : protected QWindow 
    , public WindowBase {
    Q_OBJECT
    friend WindowCallbackHandle;
public:
    WindowQT(const RenderContextBuilder* builder);
    virtual ~WindowQT();
    // IWindow
    virtual void        SetWidth(uint32_t width) override;
    virtual void        SetHeight(uint32_t height) override;
    virtual void        SetTitle(const std::string& name) override;
    virtual void        SetFullscreen(bool fullscreen) override;
    virtual uint32_t    GetWidth() const override;
    virtual uint32_t    GetHeight() const override;
    virtual std::string GetTitle() const override;
    virtual bool        IsFullscreen() const override;
    virtual WINDOW_MSG  ProcessMessage();
    // WindowQT
    virtual void        Present() override;
protected:
    bool                event(QEvent *event);
    void                exposeEvent(QExposeEvent *event);
    void                resizeEvent(QResizeEvent *event);
private:
    int32_t                    _appParam;
    QGuiApplication            _app; 
    unique_ptr<QOpenGLContext> _qtGlContext;
    bool                       _exposed;

    PREVENT_COPY(WindowQT);
};
#endif //USE_QT_LIBS

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
    virtual WINDOW_MSG  ProcessMessage();
    // WindowQT
    virtual void        Present() override;
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
    // IRenderContext
	virtual IWindow*    GetWindow();
	virtual void        Present();
private:
    unique_ptr<WindowBase> _window;

    PREVENT_COPY(RenderContextGLES2);
};
