#include "RenderContext.h"
#include "ErrorHandler.h"
#include <vector>
#include "SDL_opengles2.h"
#include "SDL_egl.h"
#include "HandleImpl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EventCallbackHandle ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowCallbackHandle::WindowCallbackHandle(WindowBase* windowContext, list< shared_ptr<EventCallback> >::iterator& iter)
    : _window(windowContext)
    , _iter(iter) {
}

WindowCallbackHandle::~WindowCallbackHandle() {
    if (_window) {
        _window->_eraseCallback(_iter);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextBuilder ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextBuilder::RenderContextBuilder() 
    : m_width(640)
    , m_height(480)
    , m_fullscreen(false)
    , m_title("Window") {
}

RenderContextBuilder::~RenderContextBuilder() {

}

inline void RenderContextBuilder::SetWidth(uint32_t width) {
    m_width = width;
}

inline void RenderContextBuilder::SetHeight(uint32_t height) {
    m_height = height;
}

inline void RenderContextBuilder::SetTitle(const std::string& name) {
    m_title = name;
}

inline void RenderContextBuilder::SetFullscreen(bool fullscreen) {
    m_fullscreen = fullscreen;
}

inline uint32_t RenderContextBuilder::GetWidth() const {
    return m_width;
}

inline uint32_t RenderContextBuilder::GetHeight() const {
    return m_height;
}

inline std::string RenderContextBuilder::GetTitle() const {
    return m_title;
}

inline bool RenderContextBuilder::IsFullscreen() const {
    return m_fullscreen;
}

IRenderContextPtr RenderContextBuilder::GetResult() {
    return make_shared<RenderContextGLES2>(this);
}

RenderContextBuilderPtr RenderContextBuilder::Create() {
    return make_shared<RenderContextBuilder>();
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
    return make_handle<WindowCallbackHandle>(this, --_eventCallbacks.end());
}

inline void WindowBase::_eraseCallback(list< shared_ptr<EventCallback> >::iterator& iter) {
    _eventCallbacks.erase(iter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowSDL //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowSDL::WindowSDL(const RenderContextBuilder* builder) {
    SDL_Init(SDL_INIT_VIDEO);
#ifdef WIN32
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif //WIN32
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
    : _window(new WindowSDL(builder)) {
}

RenderContextGLES2::~RenderContextGLES2() {
}

void RenderContextGLES2::Release() {
    delete this;
}

WindowBase* RenderContextGLES2::GetWindow() {
    return _window.get();
}

void RenderContextGLES2::Present() {
    _window->Present();
}