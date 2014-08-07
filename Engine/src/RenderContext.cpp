#include "RenderContext.h"
#include "ErrorHandler.h"
#include <vector>
#include "HandleImpl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextBuilder ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextBuilder::RenderContextBuilder() 
    : m_width(640)
    , m_height(480)
    , m_fullscreen(false)
    , m_title("Window") 
    {
}

RenderContextBuilder::~RenderContextBuilder() 
{

}

inline void RenderContextBuilder::SetWidth(uint32_t width) 
{
    m_width = width;
}

inline void RenderContextBuilder::SetHeight(uint32_t height) 
{
    m_height = height;
}

inline void RenderContextBuilder::SetTitle(const std::string& name) 
{
    m_title = name;
}

inline void RenderContextBuilder::SetFullscreen(bool fullscreen) 
{
    m_fullscreen = fullscreen;
}

inline uint32_t RenderContextBuilder::GetWidth() const 
{
    return m_width;
}

inline uint32_t RenderContextBuilder::GetHeight() const 
{
    return m_height;
}

inline std::string RenderContextBuilder::GetTitle() const 
{
    return m_title;
}

inline bool RenderContextBuilder::IsFullscreen() const 
{
    return m_fullscreen;
}

IRenderContextPtr RenderContextBuilder::GetResult() 
{
    return make_shared<RenderContextGLES2>(this);
}

RenderContextBuilderPtr RenderContextBuilder::Create() 
{
    return make_shared<RenderContextBuilder>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowBase /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowBase::WindowBase() 
{
}

WindowBase::~WindowBase() 
{
}

void WindowBase::RegisterInputCallbacks(IWindowCallbacks* callbacks)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_inputCallbacks.insert(callbacks);
}

void WindowBase::UnregisterInputCallbacks(IWindowCallbacks* callbacks)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_inputCallbacks.erase(callbacks);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WindowSDL //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowSDL::WindowSDL(const RenderContextBuilder* builder) 
{
    SDL_Init(SDL_INIT_VIDEO);
#ifdef WIN32
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif //WIN32
    m_window = SDL_CreateWindow(builder->GetTitle().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                               builder->GetWidth(), builder->GetHeight(), SDL_WINDOW_OPENGL);
    VE_ERROR_IF(!m_window, L"Window creation failed: %S", SDL_GetError());
    m_glcontext = SDL_GL_CreateContext(m_window);
    VE_ERROR_IF(!m_glcontext, L"GL context creation failed: %S", SDL_GetError());

    SetFullscreen(builder->IsFullscreen());
}

WindowSDL::~WindowSDL() 
{
    SDL_GL_DeleteContext(m_glcontext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void WindowSDL::SetWidth(uint32_t width) 
{
    SDL_SetWindowSize(m_window, width, GetHeight());
}    
     
void WindowSDL::SetHeight(uint32_t height) 
{
    SDL_SetWindowSize(m_window, GetWidth(), height);
}

void WindowSDL::SetTitle(const std::string& name) 
{
    SDL_SetWindowTitle(m_window, name.c_str());
}

void WindowSDL::SetFullscreen(bool fullscreen) 
{
    SDL_SetWindowFullscreen(m_window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

uint32_t WindowSDL::GetWidth() const 
{
    int32_t width;
    SDL_GetWindowSize(m_window, &width, nullptr);
    return (uint32_t)width;
}

uint32_t WindowSDL::GetHeight() const 
{
    int32_t height;
    SDL_GetWindowSize(m_window, nullptr, &height);
    return (uint32_t)height;
}

std::string WindowSDL::GetTitle() const 
{
    return std::string(SDL_GetWindowTitle(m_window));
}

bool WindowSDL::IsFullscreen() const 
{
    return (SDL_WINDOW_FULLSCREEN == SDL_GetWindowFlags(m_window));
}

WINDOW_MSG WindowSDL::ProcessMessage() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type) 
        {
        case SDL_KEYDOWN:
            break;
        case SDL_QUIT:
            return WINDOW_MSG::QUIT;
            break;
        }
    }
    return WINDOW_MSG::FOREGROUND;
}

void WindowSDL::Present() 
{
    SDL_GL_SwapWindow(m_window);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextGLES2 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextGLES2::RenderContextGLES2(const RenderContextBuilder* builder)
    : m_window(new WindowSDL(builder)) 
{
}

RenderContextGLES2::~RenderContextGLES2() 
{
}

void RenderContextGLES2::Release() 
{
    delete this;
}

WindowBase& RenderContextGLES2::GetWindow() 
{
    return *m_window.get();
}

void RenderContextGLES2::Present() 
{
    m_window->Present();
}