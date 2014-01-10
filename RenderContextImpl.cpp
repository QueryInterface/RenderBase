#include "RenderContextImpl.h"
#include <vector>
#include "ErrorHandler.h"
#include <Windows.h>
#include <d3dx9.h>
#include "TypeAdapter.h"

#define WINDOW_CLASS_GUID "{7956733B-91BD-4F53-B4F9-4A13AA7C7478}"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EventCallbackHandle ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventCallbackHandle::EventCallbackHandle(WindowContextWin* windowContext, list< shared_ptr<EventCallback> >::iterator& iter)
    : _windowContext(windowContext)
    , _iter(iter) {
}

EventCallbackHandle::~EventCallbackHandle() {
}

void EventCallbackHandle::Release() {
    if (_windowContext) {
        std::lock_guard<std::mutex> lock(_windowContext->_callbackMutex);
        _windowContext->_eventCallbacks.erase(_iter);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextBuilder ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextBuilder::RenderContextBuilder() 
    : _width(640)
    , _height(480) 
    , _apiType(RC_API_DX) 
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
        case RC_API_DX:
            return new RenderContextDX9(this);
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
/// WindowContextWin ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
WindowContextWin::WindowContextWin(const RenderContextBuilder* builder)
    : _window(nullptr)
    , _width(builder->GetWidth())
    , _height(builder->GetHeight())
    , _title(builder->GetTitle())
    , _fullscreen(builder->IsFullscreen()) {
    WNDCLASSEX wc = {
        sizeof( WNDCLASSEX ), CS_CLASSDC, msgProc, 0L, 0L,
        GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr,
        WINDOW_CLASS_GUID, nullptr
    };
    RegisterClassEx( &wc );

    // Create the application's window
    _window = CreateWindowA(WINDOW_CLASS_GUID, _title.c_str(),
                              WS_OVERLAPPEDWINDOW, 0, 0,_width, _height,
                              nullptr, nullptr, wc.hInstance, (void*)this);
    DWORD err = GetLastError();
    VE_ERROR_IF(!_window, L"Failed to create window");
    SetWindowPos(_window,0, 0, 0, _width, _height, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
    ShowWindow(_window, true);
}

WindowContextWin::~WindowContextWin() {

}

inline void WindowContextWin::SetWidth(uint32_t width) {
    _width = width;
    SetWindowPos(_window,0, 0, 0, _width, _height, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}

inline void WindowContextWin::SetHeight(uint32_t height) {
    _height = height;
}

inline void WindowContextWin::SetTitle(const std::string& name) {
    _title = name;
}

inline void WindowContextWin::SetFullscreen(bool fullscreen) {
    _fullscreen = fullscreen;
}

inline uint32_t WindowContextWin::GetWidth() const {
    return _width;
}

inline uint32_t WindowContextWin::GetHeight() const {
    return _height;
}

inline std::string WindowContextWin::GetTitle() const {
    return _title;
}

inline bool WindowContextWin::IsFullscreen() const {
    return _fullscreen;
}

IHandle* WindowContextWin::RegisterEventCallback(const std::shared_ptr<EventCallback>& callback) {
    std::lock_guard<std::mutex> lock(_callbackMutex);
    _eventCallbacks.push_back(callback);
    return new EventCallbackHandle(this, --_eventCallbacks.end());
}

bool WindowContextWin::ProcessMessage() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    if( PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
    {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return true;
}

inline HWND WindowContextWin::GetWindowHandle() const {
    return _window;
}

void WindowContextWin::onMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_SIZING:
    case WM_SIZE:
        //Present();
        break;
    case WM_KEYDOWN:
        {
            bool isPressed = false;
            if (_pressedKeys.find(wParam) == _pressedKeys.end()) {
                isPressed = true;
                _pressedKeys.insert(wParam);
            }
            std::lock_guard<std::mutex> lock(_callbackMutex);
            for (shared_ptr<EventCallback>& callback : _eventCallbacks) {
                callback->OnKeyPress(wParam, isPressed);
            }
        }
        break;
    case WM_KEYUP:
        {
            _pressedKeys.erase(wParam);
        }
        break;
    case WM_KILLFOCUS:
    case WM_SETFOCUS:
        {
            _pressedKeys.clear();
        }
        break;
    case WM_MOUSEMOVE:
        break;
    }
}

LRESULT WINAPI WindowContextWin::msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowContextWin* windowContext = (WindowContextWin*)GetWindowLong(hWnd, GWL_USERDATA);
    if (windowContext != nullptr)
        windowContext->onMessage(hWnd, msg, wParam, lParam);

    switch(msg) {
        case WM_CREATE:
            LPCREATESTRUCT lpcs;
            lpcs = (LPCREATESTRUCT)lParam;
            SetWindowLong(hWnd, GWL_USERDATA, (LONG)lpcs->lpCreateParams);
            break;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif // _WIN32

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Texture2DDX9 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
Texture2DDX9::Texture2DDX9(RenderContextDX9* renderContext, const std::wstring& texPath) 
    : _renderContext(renderContext)
    , _flags(TEX_FLAG_NONE) {
    HRESULT hr = D3DXCreateTextureFromFileW(renderContext->GetDevice(), texPath.c_str(), &_texture);
    VE_ERROR_IF(FAILED(hr), L"Failed texture creation with path %s", texPath.c_str());
    D3DSURFACE_DESC surfaceDesc;
    _texture->GetLevelDesc(0, &surfaceDesc);
    _width = surfaceDesc.Width;
    _height = surfaceDesc.Height;
    _format = adapter_dx_fmt::To(surfaceDesc.Format);
    _pixelSize = adapter_fmt_size::To(_format);
}

Texture2DDX9::Texture2DDX9(RenderContextDX9* renderContext, uint32_t width, uint32_t height, TEX_FORMAT format, TEX_FLAGS flags) 
    : _renderContext(renderContext) 
    , _width(width)
    , _height(height)
    , _format(format)
    , _flags(flags) {
    CComPtr<IDirect3DDevice9>& device = _renderContext->GetDevice();
    DWORD usage = D3DUSAGE_DYNAMIC;
    if (flags & TEX_FLAG_RENDER_TARGET) usage |= D3DUSAGE_RENDERTARGET;
    if (!(flags & TEX_FLAG_READ) && (flags & TEX_FLAG_WRITE)) usage |= D3DUSAGE_WRITEONLY;
    HRESULT hr = device->CreateTexture(_width, _height, 1, usage, adapter_dx_fmt::To(_format), D3DPOOL_DEFAULT, &_texture, NULL);
    VE_ERROR_IF(FAILED(hr), L"Failed texture creation with parameters: width(%d), height(%d), format(%d), flags(%d)", _width, _height, _format, _flags);
}

Texture2DDX9::~Texture2DDX9() {
}

void Texture2DDX9::Release() {
    delete this;
}

uint32_t Texture2DDX9::GetWidth() const {
    return _width;
}

uint32_t Texture2DDX9::GetHeight() const {
    return _height;
}

uint32_t Texture2DDX9::GetSize() const {
    return (_width * _height * _pixelSize + 8) >> 3;
}

TEX_FORMAT Texture2DDX9::GetFormat() const {
    return _format;
}

TEX_FLAGS Texture2DDX9::GetFlags() const {
    return _flags;
}

void Texture2DDX9::Lock(void** outData, uint32_t outPitch) {
}

void Texture2DDX9::Unlock() {
}

CComPtr<IDirect3DTexture9>& Texture2DDX9::GetPointer() {
    return _texture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// RenderContextDX
///////////////////////////////////////////////////////////////////////////////////////////////////////
RenderContextDX9::RenderContextDX9(const RenderContextBuilder* builder) {
    HRESULT hr = S_OK;
    
    _windowContext.reset(new WindowContextWin(builder));
    _d3d = Direct3DCreate9(D3D_SDK_VERSION);
    VE_ERROR_IF(!_d3d, L"Failed to create Direct3D");

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.EnableAutoDepthStencil = true;

    hr = _d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _windowContext->GetWindowHandle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &_device);
    VE_ERROR_IF(FAILED(hr), L"Failed to create device");
    hr = _device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    VE_ERROR_IF(FAILED(hr), L"Failed to set render state for device %p", _device);
}

RenderContextDX9::~RenderContextDX9() {
}

void RenderContextDX9::Release() {
    delete this;
}

inline IWindowContext* RenderContextDX9::GetWindowContext() {
    return _windowContext.get();
}

ITexture2D* RenderContextDX9::CreateTexture2D(const std::wstring& texPath) {
    try {
        return new Texture2DDX9(this, texPath);
    }
    catch (...) {
        return nullptr;
    }
}

ITexture2D* RenderContextDX9::CreateTexture2D(uint32_t width, uint32_t height, TEX_FORMAT format, TEX_FLAGS flags) {
    try {
        return new Texture2DDX9(this, width, height, format, flags);
    }
    catch (...) {
        return nullptr;
    }
}

CComPtr<IDirect3DDevice9>& RenderContextDX9::GetDevice() {
    return _device;
}

void RenderContextDX9::Clear(uint32_t flags, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float depth, uint8_t stencil) {
    DWORD clearFlags = 0;
    if (flags & CLEAR_FLAG_COLOR)   clearFlags |= D3DCLEAR_TARGET;
    if (flags & CLEAR_FLAG_DEPTH)   clearFlags |= D3DCLEAR_ZBUFFER;
    if (flags & CLEAR_FLAG_STENCIL) clearFlags |= D3DCLEAR_STENCIL;
    HRESULT hr = _device->Clear(0, nullptr, clearFlags, D3DCOLOR_RGBA(r, g, b, a), depth, stencil);
    VE_WARNING_IF(FAILED(hr), L"Failed to clear render target");
}

void RenderContextDX9::Present() {
    _device->Present(NULL, NULL, NULL, NULL);
}

#endif //_WIN32