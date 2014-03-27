#include "RenderContextImpl.h"
#include "ErrorHandler.h"
#include <QtGui/QOpenGLContext>
#include <vector>

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
WindowQT::WindowQT(const RenderContextBuilder* builder) 
    : QWindow((QWindow*)nullptr)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextGLES2 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextGLES2::RenderContextGLES2(const RenderContextBuilder* builder)
    : _window(new WindowQT(builder)) {
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