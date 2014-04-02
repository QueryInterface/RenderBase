#include "RenderContextImpl.h"
#include "ErrorHandler.h"
#include <vector>
#ifdef USE_QT_LIBS
#include <QtGui/QOpenGLContext>
#else
#include "SDL_opengles2.h"
#include "SDL_egl.h"
#endif //USE_QT_LIBS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EventCallbackHandle ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowCallbackHandle::WindowCallbackHandle(WindowBase* windowContext, list< shared_ptr<EventCallback> >::iterator& iter)
    : _window(windowContext)
    , _iter(iter) {
}

WindowCallbackHandle::~WindowCallbackHandle() {
}

inline void WindowCallbackHandle::Release() {
    if (_window) {
        _window->_eraseCallback(_iter);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextBuilder ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextBuilder::RenderContextBuilder() 
    : _width(640)
    , _height(480)
    , _fullscreen(false)
    , _apiType(RC_API_TYPE::GLES2) 
    , _title("Window") {
}

RenderContextBuilder::~RenderContextBuilder() {

}

void RenderContextBuilder::Release() {
    delete this;
}

inline void RenderContextBuilder::SetWidth(uint32_t width) {
    _width = width;
}

inline void RenderContextBuilder::SetHeight(uint32_t height) {
    _height = height;
}

inline void RenderContextBuilder::SetTitle(const std::string& name) {
    _title = name;
}

inline void RenderContextBuilder::SetFullscreen(bool fullscreen) {
    _fullscreen = fullscreen;
}

inline uint32_t RenderContextBuilder::GetWidth() const {
    return _width;
}

inline uint32_t RenderContextBuilder::GetHeight() const {
    return _height;
}

inline std::string RenderContextBuilder::GetTitle() const {
    return _title;
}

inline bool RenderContextBuilder::IsFullscreen() const {
    return _fullscreen;
}

inline void RenderContextBuilder::SetApiType(RC_API_TYPE apiType) {
    _apiType = apiType;
}

inline RC_API_TYPE RenderContextBuilder::GetApiType() const {
    return _apiType;
}

IRenderContext* RenderContextBuilder::GetResult() {
    try {
        switch (_apiType) {
#ifdef _WIN32
        case RC_API_TYPE::GLES2:
            return new RenderContextGLES2(this);
            break;
#endif //_WIN32
        default:
            VE_ERROR(L"Failed to create render context");
        }
    }
    catch (...) {
    }
    return nullptr;
}

IRenderContextBuilder* IRenderContextBuilder::Create() {
    return new RenderContextBuilder();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowBase /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowBase::WindowBase() {
}

WindowBase::~WindowBase() {
}

IHandle* WindowBase::RegisterEventCallback(const std::shared_ptr<EventCallback>& callback) {
    std::lock_guard<std::mutex> lock(_callbackMutex);
    _eventCallbacks.push_back(callback);
    return new WindowCallbackHandle(this, --_eventCallbacks.end());
}

inline void WindowBase::_eraseCallback(list< shared_ptr<EventCallback> >::iterator& iter) {
    _eventCallbacks.erase(iter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowtQT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_QT_LIBS
WindowQT::WindowQT(const RenderContextBuilder* builder) 
    : _appParam(0)
    , _app(_appParam, 0)
    , QWindow((QWindow*)nullptr)
    , _exposed(QWindow::isExposed())
    , _qtGlContext(new QOpenGLContext(this)) {

    SetWidth(builder->GetWidth());
    SetHeight(builder->GetHeight());
    SetTitle(builder->GetTitle());
    SetFullscreen(builder->IsFullscreen());
    setSurfaceType(QWindow::OpenGLSurface);
}

WindowQT::~WindowQT() {
}

void WindowQT::SetWidth(uint32_t width) {
    QWindow::setWidth(width);
}    
     
void WindowQT::SetHeight(uint32_t height) {
    QWindow::setHeight(height);
}

void WindowQT::SetTitle(const std::string& name) {
    QWindow::setTitle(name.c_str());
}

void WindowQT::SetFullscreen(bool fullscreen) {
    if (fullscreen)
        QWindow::showFullScreen();
    else
        QWindow::showNormal();
}

uint32_t WindowQT::GetWidth() const {
    return QWindow::width();
}

uint32_t WindowQT::GetHeight() const {
    return QWindow::height();
}

std::string WindowQT::GetTitle() const {
    return QWindow::title().toUtf8().constData();
}

bool WindowQT::IsFullscreen() const {
    return QWindow::visibility() == FullScreen;
}

WINDOW_MSG WindowQT::ProcessMessage() {
    if (_exposed)
        return WINDOW_MSG::FOREGROUND;
    return WINDOW_MSG::BACKGROUND;
}

void WindowQT::Present() {
    _qtGlContext->swapBuffers(this);
}

bool WindowQT::event(QEvent* event) {
    UNUSED(event);
    switch (event->type()) {
    case QEvent::Quit:
        return QWindow::event(event);
    default:
        return QWindow::event(event);
    }
}

void WindowQT::exposeEvent(QExposeEvent* event) {
    UNUSED(event);
    _exposed = QWindow::isExposed();
}

void WindowQT::resizeEvent(QResizeEvent* event) {
    UNUSED(event);
}
#endif //USE_QT_LIBS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowSDL //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowSDL::WindowSDL(const RenderContextBuilder* builder) {
    SDL_Init(SDL_INIT_VIDEO);
#ifdef USE_ANGLE
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif //USE_ANGLE
    _window = SDL_CreateWindow(builder->GetTitle().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               builder->GetWidth(), builder->GetHeight(), SDL_WINDOW_OPENGL);
    VE_ERROR_IF(!_window, L"Window creation failed: %S", SDL_GetError());
    _glcontext = SDL_GL_CreateContext(_window);
    VE_ERROR_IF(!_glcontext, L"GL context creation failed: %S", SDL_GetError());

    SetFullscreen(builder->IsFullscreen());
}

WindowSDL::~WindowSDL() {
    SDL_GL_DeleteContext(_glcontext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void WindowSDL::SetWidth(uint32_t width) {
    SDL_SetWindowSize(_window, width, GetHeight());
}    
     
void WindowSDL::SetHeight(uint32_t height) {
    SDL_SetWindowSize(_window, GetWidth(), height);
}

void WindowSDL::SetTitle(const std::string& name) {
    SDL_SetWindowTitle(_window, name.c_str());
}

void WindowSDL::SetFullscreen(bool fullscreen) {
    SDL_SetWindowFullscreen(_window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

uint32_t WindowSDL::GetWidth() const {
    int32_t width;
    SDL_GetWindowSize(_window, &width, nullptr);
    return (uint32_t)width;
}

uint32_t WindowSDL::GetHeight() const {
    int32_t height;
    SDL_GetWindowSize(_window, nullptr, &height);
    return (uint32_t)height;
}

std::string WindowSDL::GetTitle() const {
    return std::string(SDL_GetWindowTitle(_window));
}

bool WindowSDL::IsFullscreen() const {
    return (SDL_WINDOW_FULLSCREEN == SDL_GetWindowFlags(_window));
}

WINDOW_MSG WindowSDL::ProcessMessage() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
    case SDL_KEYDOWN:
        break;
    case SDL_QUIT:
        return WINDOW_MSG::QUIT;
        break;
    }
    return WINDOW_MSG::FOREGROUND;
}

void WindowSDL::Present() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(_window);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextGLES2 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextGLES2::RenderContextGLES2(const RenderContextBuilder* builder)
#ifdef USE_QT_LIBS
    : _window(new WindowQT(builder)) {
#else
    : _window(new WindowSDL(builder)) {
#endif //USE_QT_LIBS
}

RenderContextGLES2::~RenderContextGLES2() {
}

void RenderContextGLES2::Release() {
    delete this;
}

IWindow* RenderContextGLES2::GetWindow() {
    return _window.get();
}

void RenderContextGLES2::Present() {
    _window->Present();
}