#pragma once
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif // _DEBUG

#include "RenderContext.h"
#include <list>
#include <set>
#include <Windows.h>
#include <atlbase.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mutex>


using std::list;
using std::set;
using std::unique_ptr;
using std::mutex;

class RenderContextDX9;
class WindowContextWin;

class EventCallbackHandle : public IHandle {
public:
    EventCallbackHandle(WindowContextWin* windowContext, list< shared_ptr<EventCallback> >::iterator& iter);
    virtual ~EventCallbackHandle();
    // IHandle
    virtual void Release();
private:
    WindowContextWin*                           _windowContext;
    list< shared_ptr<EventCallback> >::iterator _iter;
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

#ifdef _WIN32
class WindowContextWin : public IWindowContext {
    friend EventCallbackHandle;
public:
    WindowContextWin(const RenderContextBuilder* builder);
    virtual ~WindowContextWin();
    // IWindowSetup
    virtual void            SetWidth(uint32_t width);
    virtual void            SetHeight(uint32_t height);
    virtual void            SetTitle(const std::string& name);
    virtual void            SetFullscreen(bool fullscreen);
    virtual uint32_t        GetWidth() const;
    virtual uint32_t        GetHeight() const;
    virtual std::string     GetTitle() const;
    virtual bool            IsFullscreen() const;
    // IWindowContext
    virtual IHandle*        RegisterEventCallback(const std::shared_ptr<EventCallback>& callback);
    virtual bool            ProcessMessage();
    // WindowContextWin
    HWND                    GetWindowHandle() const;
private:
    virtual void onMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WINAPI msgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND                            _window;
    set<WPARAM>                     _pressedKeys;
    mutex                           _callbackMutex;
    list<shared_ptr<EventCallback>> _eventCallbacks;
    uint32_t                        _width;
    uint32_t                        _height;
    std::string                     _title;
    bool                            _fullscreen;
};
#endif //_WIN32

#ifdef _WIN32
class Texture2DDX9 : public ITexture2D {
public:
    Texture2DDX9(RenderContextDX9* renderContext, const std::wstring& texPath);
    Texture2DDX9(RenderContextDX9* renderContext, uint32_t numMips, uint32_t width, uint32_t height, TEX_FORMAT format, TEX_FLAGS flags);
    virtual ~Texture2DDX9();
    // IHandle
    virtual void        Release();
    // ITexture2D
    virtual uint32_t    GetNumMipLevels() const;
    virtual uint32_t    GetWidth() const;
    virtual uint32_t    GetHeight() const;
    virtual uint32_t    GetSize() const;
    virtual TEX_FORMAT  GetFormat() const;
    virtual uint32_t    GetPixelSize() const;
    virtual TEX_FLAGS   GetFlags() const;
    virtual void        Lock(uint32_t level, void** outData, uint32_t& outPitch);
    virtual void        Unlock(uint32_t level);
    // Texture2DDX9
    CComPtr<IDirect3DTexture9>& GetPointer();
private:
    CComPtr<IDirect3DTexture9>  _texture;
    RenderContextDX9*           _renderContext;
    uint32_t                    _width;
    uint32_t                    _height;
    uint32_t                    _pixelSize;
    TEX_FORMAT                  _format;
    TEX_FLAGS                   _flags;
    uint32_t                    _numMipLevels;
};

class RenderContextDX9 : public IRenderContext {
public:
    RenderContextDX9(const RenderContextBuilder* builder);
    virtual ~RenderContextDX9();
    // IHandle
    virtual void Release();
    // IRenderContext
    virtual IWindowContext*     GetWindowContext();
    virtual ITexture2D*         CreateTexture2D(const std::wstring& texPath);
    virtual ITexture2D*         CreateTexture2D(uint32_t mips, uint32_t width, uint32_t height, TEX_FORMAT format, TEX_FLAGS flags);
    virtual void                Clear(uint32_t flags, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float depth, uint8_t stencil);
    virtual void                Present();
    // RenderContextDX9
    CComPtr<IDirect3DDevice9>&  GetDevice();
    ID3DXBuffer*                CompileShader(const string& shaderSource, const string& entryPoint, const string& profile, ID3DXConstantTable** outConstantTable = nullptr) const;
private:
    unique_ptr<WindowContextWin>    _windowContext;
    CComPtr<IDirect3D9>             _d3d;          
    CComPtr<IDirect3DDevice9>       _device;

};
#endif //_WIN32