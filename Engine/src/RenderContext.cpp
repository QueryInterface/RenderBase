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
            {
                if (!event.key.repeat)
                {
                    for (auto& cb : m_inputCallbacks) cb->OnKeyDown(convert(event.key.keysym.sym));
                }
            } break;
        case SDL_KEYUP:
            {
                for (auto& cb : m_inputCallbacks) cb->OnKeyUp(convert(event.key.keysym.sym));
            } break;
        case SDL_MOUSEBUTTONDOWN:
            {
                for (auto& cb : m_inputCallbacks) cb->OnMouseDown(convert(event.button.button), event.button.x, event.button.y);
            } break;
        case SDL_MOUSEBUTTONUP:
            {
                for (auto& cb : m_inputCallbacks) cb->OnMouseUp(convert(event.button.button), event.button.x, event.button.y);
            } break;
        case SDL_MOUSEMOTION:
            {
                for (auto& cb : m_inputCallbacks) cb->OnMouseMove(event.motion.x, event.motion.y);
            } break;
        case SDL_FINGERDOWN:
            {
                for (auto& cb : m_inputCallbacks) cb->OnFingerDown(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
            }break;
        case SDL_FINGERUP:
            {
                for (auto& cb : m_inputCallbacks) cb->OnFingerUp(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
            } break;
        case SDL_FINGERMOTION:
            { 
                for (auto& cb : m_inputCallbacks) cb->OnFingerMove(event.tfinger.x, event.tfinger.y, event.tfinger.dx, event.tfinger.dy);
            } break;
        case SDL_MULTIGESTURE:
            {
                for (auto& cb : m_inputCallbacks) cb->OnMultiGesture(event.mgesture.numFingers, event.mgesture.x, event.mgesture.y, event.mgesture.dTheta, event.mgesture.dDist);
            } break;
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

EKey WindowSDL::convert(uint32_t sdlKey)
{
    switch (sdlKey)
    {
        case SDLK_UNKNOWN:              return EKey::EK_UNKNOWN; break;
        case SDLK_RETURN:               return EKey::EK_RETURN; break;
        case SDLK_ESCAPE:               return EKey::EK_ESCAPE; break;
        case SDLK_BACKSPACE:            return EKey::EK_BACKSPACE; break;
        case SDLK_TAB:                  return EKey::EK_TAB; break;
        case SDLK_SPACE:                return EKey::EK_SPACE; break;
        case SDLK_EXCLAIM:              return EKey::EK_EXCLAIM; break;
        case SDLK_QUOTEDBL:             return EKey::EK_QUOTEDBL; break;
        case SDLK_HASH:                 return EKey::EK_HASH; break;
        case SDLK_PERCENT:              return EKey::EK_PERCENT; break;
        case SDLK_DOLLAR:               return EKey::EK_DOLLAR; break;
        case SDLK_AMPERSAND:            return EKey::EK_AMPERSAND; break;
        case SDLK_QUOTE:                return EKey::EK_QUOTE; break;
        case SDLK_LEFTPAREN:            return EKey::EK_LEFTPAREN; break;
        case SDLK_RIGHTPAREN:           return EKey::EK_RIGHTPAREN; break;
        case SDLK_ASTERISK:             return EKey::EK_ASTERISK; break;
        case SDLK_PLUS:                 return EKey::EK_PLUS; break;
        case SDLK_COMMA:                return EKey::EK_COMMA; break;
        case SDLK_MINUS:                return EKey::EK_MINUS; break;
        case SDLK_PERIOD:               return EKey::EK_PERIOD; break;
        case SDLK_SLASH:                return EKey::EK_SLASH; break;
        case SDLK_0:                    return EKey::EK_0; break;
        case SDLK_1:                    return EKey::EK_1; break;
        case SDLK_2:                    return EKey::EK_2; break;
        case SDLK_3:                    return EKey::EK_3; break;
        case SDLK_4:                    return EKey::EK_4; break;
        case SDLK_5:                    return EKey::EK_5; break;
        case SDLK_6:                    return EKey::EK_6; break;
        case SDLK_7:                    return EKey::EK_7; break;
        case SDLK_8:                    return EKey::EK_8; break;
        case SDLK_9:                    return EKey::EK_9; break;
        case SDLK_COLON:                return EKey::EK_COLON; break;
        case SDLK_SEMICOLON:            return EKey::EK_SEMICOLON; break;
        case SDLK_LESS:                 return EKey::EK_LESS; break;
        case SDLK_EQUALS:               return EKey::EK_EQUALS; break;
        case SDLK_GREATER:              return EKey::EK_GREATER; break;
        case SDLK_QUESTION:             return EKey::EK_QUESTION; break;
        case SDLK_AT:                   return EKey::EK_AT; break;
        case SDLK_LEFTBRACKET:          return EKey::EK_LEFTBRACKET; break;
        case SDLK_BACKSLASH:            return EKey::EK_BACKSLASH; break;
        case SDLK_RIGHTBRACKET:         return EKey::EK_RIGHTBRACKET; break;
        case SDLK_CARET:                return EKey::EK_CARET; break;
        case SDLK_UNDERSCORE:           return EKey::EK_UNDERSCORE; break;
        case SDLK_BACKQUOTE:            return EKey::EK_BACKQUOTE; break;
        case SDLK_a:                    return EKey::EK_A; break;
        case SDLK_b:                    return EKey::EK_B; break;
        case SDLK_c:                    return EKey::EK_C; break;
        case SDLK_d:                    return EKey::EK_D; break;
        case SDLK_e:                    return EKey::EK_E; break;
        case SDLK_f:                    return EKey::EK_F; break;
        case SDLK_g:                    return EKey::EK_G; break;
        case SDLK_h:                    return EKey::EK_H; break;
        case SDLK_i:                    return EKey::EK_I; break;
        case SDLK_j:                    return EKey::EK_J; break;
        case SDLK_k:                    return EKey::EK_K; break;
        case SDLK_l:                    return EKey::EK_L; break;
        case SDLK_m:                    return EKey::EK_M; break;
        case SDLK_n:                    return EKey::EK_N; break;
        case SDLK_o:                    return EKey::EK_O; break;
        case SDLK_p:                    return EKey::EK_P; break;
        case SDLK_q:                    return EKey::EK_Q; break;
        case SDLK_r:                    return EKey::EK_R; break;
        case SDLK_s:                    return EKey::EK_S; break;
        case SDLK_t:                    return EKey::EK_T; break;
        case SDLK_u:                    return EKey::EK_U; break;
        case SDLK_v:                    return EKey::EK_V; break;
        case SDLK_w:                    return EKey::EK_W; break;
        case SDLK_x:                    return EKey::EK_X; break;
        case SDLK_y:                    return EKey::EK_Y; break;
        case SDLK_z:                    return EKey::EK_Z; break;
        case SDLK_CAPSLOCK:             return EKey::EK_CAPSLOCK; break;
        case SDLK_F1:                   return EKey::EK_F1; break;
        case SDLK_F2:                   return EKey::EK_F2; break;
        case SDLK_F3:                   return EKey::EK_F3; break;
        case SDLK_F4:                   return EKey::EK_F4; break;
        case SDLK_F5:                   return EKey::EK_F5; break;
        case SDLK_F6:                   return EKey::EK_F6; break;
        case SDLK_F7:                   return EKey::EK_F7; break;
        case SDLK_F8:                   return EKey::EK_F8; break;
        case SDLK_F9:                   return EKey::EK_F9; break;
        case SDLK_F10:                  return EKey::EK_F10; break;
        case SDLK_F11:                  return EKey::EK_F11; break;
        case SDLK_F12:                  return EKey::EK_F12; break;
        case SDLK_PRINTSCREEN:          return EKey::EK_PRINTSCREEN; break;
        case SDLK_SCROLLLOCK:           return EKey::EK_SCROLLLOCK; break;
        case SDLK_PAUSE:                return EKey::EK_PAUSE; break;
        case SDLK_INSERT:               return EKey::EK_INSERT; break;
        case SDLK_HOME:                 return EKey::EK_HOME; break;
        case SDLK_PAGEUP:               return EKey::EK_PAGEUP; break;
        case SDLK_DELETE:               return EKey::EK_DELETE; break;
        case SDLK_END:                  return EKey::EK_END; break;
        case SDLK_PAGEDOWN:             return EKey::EK_PAGEDOWN; break;
        case SDLK_RIGHT:                return EKey::EK_RIGHT; break;
        case SDLK_LEFT:                 return EKey::EK_LEFT; break;
        case SDLK_DOWN:                 return EKey::EK_DOWN; break;
        case SDLK_UP:                   return EKey::EK_UP; break;
        case SDLK_NUMLOCKCLEAR:         return EKey::EK_NUMLOCKCLEAR; break;
        case SDLK_KP_DIVIDE:            return EKey::EK_KP_DIVIDE; break;
        case SDLK_KP_MULTIPLY:          return EKey::EK_KP_MULTIPLY; break;
        case SDLK_KP_MINUS:             return EKey::EK_KP_MINUS; break;
        case SDLK_KP_PLUS:              return EKey::EK_KP_PLUS; break;
        case SDLK_KP_ENTER:             return EKey::EK_KP_ENTER; break;
        case SDLK_KP_1:                 return EKey::EK_KP_1; break;
        case SDLK_KP_2:                 return EKey::EK_KP_2; break;
        case SDLK_KP_3:                 return EKey::EK_KP_3; break;
        case SDLK_KP_4:                 return EKey::EK_KP_4; break;
        case SDLK_KP_5:                 return EKey::EK_KP_5; break;
        case SDLK_KP_6:                 return EKey::EK_KP_6; break;
        case SDLK_KP_7:                 return EKey::EK_KP_7; break;
        case SDLK_KP_8:                 return EKey::EK_KP_8; break;
        case SDLK_KP_9:                 return EKey::EK_KP_9; break;
        case SDLK_KP_0:                 return EKey::EK_KP_0; break;
        case SDLK_KP_PERIOD:            return EKey::EK_KP_PERIOD; break;
        case SDLK_APPLICATION:          return EKey::EK_APPLICATION; break;
        case SDLK_POWER:                return EKey::EK_POWER; break;
        case SDLK_KP_EQUALS:            return EKey::EK_KP_EQUALS; break;
        case SDLK_F13:                  return EKey::EK_F13; break;
        case SDLK_F14:                  return EKey::EK_F14; break;
        case SDLK_F15:                  return EKey::EK_F15; break;
        case SDLK_F16:                  return EKey::EK_F16; break;
        case SDLK_F17:                  return EKey::EK_F17; break;
        case SDLK_F18:                  return EKey::EK_F18; break;
        case SDLK_F19:                  return EKey::EK_F19; break;
        case SDLK_F20:                  return EKey::EK_F20; break;
        case SDLK_F21:                  return EKey::EK_F21; break;
        case SDLK_F22:                  return EKey::EK_F22; break;
        case SDLK_F23:                  return EKey::EK_F23; break;
        case SDLK_F24:                  return EKey::EK_F24; break;
        case SDLK_EXECUTE:              return EKey::EK_EXECUTE; break;
        case SDLK_HELP:                 return EKey::EK_HELP; break;
        case SDLK_MENU:                 return EKey::EK_MENU; break;
        case SDLK_SELECT:               return EKey::EK_SELECT; break;
        case SDLK_STOP:                 return EKey::EK_STOP; break;
        case SDLK_AGAIN:                return EKey::EK_AGAIN; break;
        case SDLK_UNDO:                 return EKey::EK_UNDO; break;
        case SDLK_CUT:                  return EKey::EK_CUT; break;
        case SDLK_COPY:                 return EKey::EK_COPY; break;
        case SDLK_PASTE:                return EKey::EK_PASTE; break;
        case SDLK_FIND:                 return EKey::EK_FIND; break;
        case SDLK_MUTE:                 return EKey::EK_MUTE; break;
        case SDLK_VOLUMEUP:             return EKey::EK_VOLUMEUP; break;
        case SDLK_VOLUMEDOWN:           return EKey::EK_VOLUMEDOWN; break;
        case SDLK_KP_COMMA:             return EKey::EK_KP_COMMA; break;
        case SDLK_KP_EQUALSAS400:       return EKey::EK_KP_EQUALSAS400; break;
        case SDLK_ALTERASE:             return EKey::EK_ALTERASE; break;
        case SDLK_SYSREQ:               return EKey::EK_SYSREQ; break;
        case SDLK_CANCEL:               return EKey::EK_CANCEL; break;
        case SDLK_CLEAR:                return EKey::EK_CLEAR; break;
        case SDLK_PRIOR:                return EKey::EK_PRIOR; break;
        case SDLK_RETURN2:              return EKey::EK_RETURN2; break;
        case SDLK_SEPARATOR:            return EKey::EK_SEPARATOR; break;
        case SDLK_OUT:                  return EKey::EK_OUT; break;
        case SDLK_OPER:                 return EKey::EK_OPER; break;
        case SDLK_CLEARAGAIN:           return EKey::EK_CLEARAGAIN; break;
        case SDLK_CRSEL:                return EKey::EK_CRSEL; break;
        case SDLK_EXSEL:                return EKey::EK_EXSEL; break;
        case SDLK_KP_00:                return EKey::EK_KP_00; break;
        case SDLK_KP_000:               return EKey::EK_KP_000; break;
        case SDLK_THOUSANDSSEPARATOR:   return EKey::EK_THOUSANDSSEPARATOR; break;
        case SDLK_DECIMALSEPARATOR:     return EKey::EK_DECIMALSEPARATOR; break;
        case SDLK_CURRENCYUNIT:         return EKey::EK_CURRENCYUNIT; break;
        case SDLK_CURRENCYSUBUNIT:      return EKey::EK_CURRENCYSUBUNIT; break;
        case SDLK_KP_LEFTPAREN:         return EKey::EK_KP_LEFTPAREN; break;
        case SDLK_KP_RIGHTPAREN:        return EKey::EK_KP_RIGHTPAREN; break;
        case SDLK_KP_LEFTBRACE:         return EKey::EK_KP_LEFTBRACE; break;
        case SDLK_KP_RIGHTBRACE:        return EKey::EK_KP_RIGHTBRACE; break;
        case SDLK_KP_TAB:               return EKey::EK_KP_TAB; break;
        case SDLK_KP_BACKSPACE:         return EKey::EK_KP_BACKSPACE; break;
        case SDLK_KP_A:                 return EKey::EK_KP_A; break;
        case SDLK_KP_B:                 return EKey::EK_KP_B; break;
        case SDLK_KP_C:                 return EKey::EK_KP_C; break;
        case SDLK_KP_D:                 return EKey::EK_KP_D; break;
        case SDLK_KP_E:                 return EKey::EK_KP_E; break;
        case SDLK_KP_F:                 return EKey::EK_KP_F; break;
        case SDLK_KP_XOR:               return EKey::EK_KP_XOR; break;
        case SDLK_KP_POWER:             return EKey::EK_KP_POWER; break;
        case SDLK_KP_PERCENT:           return EKey::EK_KP_PERCENT; break;
        case SDLK_KP_LESS:              return EKey::EK_KP_LESS; break;
        case SDLK_KP_GREATER:           return EKey::EK_KP_GREATER; break;
        case SDLK_KP_AMPERSAND:         return EKey::EK_KP_AMPERSAND; break;
        case SDLK_KP_DBLAMPERSAND:      return EKey::EK_KP_DBLAMPERSAND; break;
        case SDLK_KP_VERTICALBAR:       return EKey::EK_KP_VERTICALBAR; break;
        case SDLK_KP_DBLVERTICALBAR:    return EKey::EK_KP_DBLVERTICALBAR; break;
        case SDLK_KP_COLON:             return EKey::EK_KP_COLON; break;
        case SDLK_KP_HASH:              return EKey::EK_KP_HASH; break;
        case SDLK_KP_SPACE:             return EKey::EK_KP_SPACE; break;
        case SDLK_KP_AT:                return EKey::EK_KP_AT; break;
        case SDLK_KP_EXCLAM:            return EKey::EK_KP_EXCLAM; break;
        case SDLK_KP_MEMSTORE:          return EKey::EK_KP_MEMSTORE; break;
        case SDLK_KP_MEMRECALL:         return EKey::EK_KP_MEMRECALL; break;
        case SDLK_KP_MEMCLEAR:          return EKey::EK_KP_MEMCLEAR; break;
        case SDLK_KP_MEMADD:            return EKey::EK_KP_MEMADD; break;
        case SDLK_KP_MEMSUBTRACT:       return EKey::EK_KP_MEMSUBTRACT; break;
        case SDLK_KP_MEMMULTIPLY:       return EKey::EK_KP_MEMMULTIPLY; break;
        case SDLK_KP_MEMDIVIDE:         return EKey::EK_KP_MEMDIVIDE; break;
        case SDLK_KP_PLUSMINUS:         return EKey::EK_KP_PLUSMINUS; break;
        case SDLK_KP_CLEAR:             return EKey::EK_KP_CLEAR; break;
        case SDLK_KP_CLEARENTRY:        return EKey::EK_KP_CLEARENTRY; break;
        case SDLK_KP_BINARY:            return EKey::EK_KP_BINARY; break;
        case SDLK_KP_OCTAL:             return EKey::EK_KP_OCTAL; break;
        case SDLK_KP_DECIMAL:           return EKey::EK_KP_DECIMAL; break;
        case SDLK_KP_HEXADECIMAL:       return EKey::EK_KP_HEXADECIMAL; break;
        case SDLK_LCTRL:                return EKey::EK_LCTRL; break;
        case SDLK_LSHIFT:               return EKey::EK_LSHIFT; break;
        case SDLK_LALT:                 return EKey::EK_LALT; break;
        case SDLK_LGUI:                 return EKey::EK_LGUI; break;
        case SDLK_RCTRL:                return EKey::EK_RCTRL; break;
        case SDLK_RSHIFT:               return EKey::EK_RSHIFT; break;
        case SDLK_RALT:                 return EKey::EK_RALT; break;
        case SDLK_RGUI:                 return EKey::EK_RGUI; break;
        case SDLK_MODE:                 return EKey::EK_MODE; break;
        case SDLK_AUDIONEXT:            return EKey::EK_AUDIONEXT; break;
        case SDLK_AUDIOPREV:            return EKey::EK_AUDIOPREV; break;
        case SDLK_AUDIOSTOP:            return EKey::EK_AUDIOSTOP; break;
        case SDLK_AUDIOPLAY:            return EKey::EK_AUDIOPLAY; break;
        case SDLK_AUDIOMUTE:            return EKey::EK_AUDIOMUTE; break;
        case SDLK_MEDIASELECT:          return EKey::EK_MEDIASELECT; break;
        case SDLK_WWW:                  return EKey::EK_WWW; break;
        case SDLK_MAIL:                 return EKey::EK_MAIL; break;
        case SDLK_CALCULATOR:           return EKey::EK_CALCULATOR; break;
        case SDLK_COMPUTER:             return EKey::EK_COMPUTER; break;
        case SDLK_AC_SEARCH:            return EKey::EK_AC_SEARCH; break;
        case SDLK_AC_HOME:              return EKey::EK_AC_HOME; break;
        case SDLK_AC_BACK:              return EKey::EK_AC_BACK; break;
        case SDLK_AC_FORWARD:           return EKey::EK_AC_FORWARD; break;
        case SDLK_AC_STOP:              return EKey::EK_AC_STOP; break;
        case SDLK_AC_REFRESH:           return EKey::EK_AC_REFRESH; break;
        case SDLK_AC_BOOKMARKS:         return EKey::EK_AC_BOOKMARKS; break;
        case SDLK_BRIGHTNESSDOWN:       return EKey::EK_BRIGHTNESSDOWN; break;
        case SDLK_BRIGHTNESSUP:         return EKey::EK_BRIGHTNESSUP; break;
        case SDLK_DISPLAYSWITCH:        return EKey::EK_DISPLAYSWITCH; break;
        case SDLK_KBDILLUMTOGGLE:       return EKey::EK_KBDILLUMTOGGLE; break;
        case SDLK_KBDILLUMDOWN:         return EKey::EK_KBDILLUMDOWN; break;
        case SDLK_KBDILLUMUP:           return EKey::EK_KBDILLUMUP; break;
        case SDLK_EJECT:                return EKey::EK_EJECT; break;
        case SDLK_SLEEP:                return EKey::EK_SLEEP; break;

        case SDL_BUTTON_LEFT:           return EKey::EK_MOUSE_BUTTON_LEFT; break;
        case SDL_BUTTON_MIDDLE:         return EKey::EK_MOUSE_BUTTON_MIDDLE; break;
        case SDL_BUTTON_RIGHT:          return EKey::EK_MOUSE_BUTTON_RIGHT; break;
        default:
            {
                VE_ERROR(L"Invalid SDL key");
                return EKey::EK_UNKNOWN;
            }
    }
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