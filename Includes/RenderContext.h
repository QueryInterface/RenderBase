#pragma once
#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;

enum RC_API_TYPE {
    RC_API_DX   = 0,
};

enum TEX_FORMAT {
    TEX_FORMAT_UNKNOWN,
    TEX_FORMAT_A8R8G8B8,
    TEX_FORMAT_X8R8G8B8,
    TEX_FORMAT_A8
};

enum TEX_FLAGS {
    TEX_FLAG_NONE           = 0x0,
    TEX_FLAG_READ           = 0x1,
    TEX_FLAG_WRITE          = 0x2,
    TEX_FLAG_RENDER_TARGET  = 0x4,
};

enum SHADER_TYPE {
    SHADER_TYPE_VERTEX,
    SHADER_TYPE_PIXEL
};

enum CLEAR_FLAGS {
    CLEAR_FLAG_NONE         = 0x0,
    CLEAR_FLAG_COLOR        = 0x1,
    CLEAR_FLAG_DEPTH        = 0x2,
    CLEAR_FLAG_STENCIL      = 0x4
};

struct IHandle {
    virtual void Release() = 0;
};

struct IRenderable {
    virtual void Render() = 0;
};

struct EventCallback {
    virtual void OnKeyPress(uint32_t keyCode, bool isPress) {keyCode; isPress;}
};

struct IWindowSetup {
    virtual void            SetWidth(uint32_t width) = 0;
    virtual void            SetHeight(uint32_t height) = 0;
    virtual void            SetTitle(const std::string& title) = 0;
    virtual void            SetFullscreen(bool fullscreen) = 0;

    virtual uint32_t        GetWidth() const = 0;
    virtual uint32_t        GetHeight() const = 0;
    virtual std::string     GetTitle() const = 0;
    virtual bool            IsFullscreen() const = 0;
};

struct IWindowContext : public IWindowSetup {
    virtual IHandle*    RegisterEventCallback(const std::shared_ptr<EventCallback>& callback) = 0;
    virtual bool        ProcessMessage() = 0;
};

struct ITexture2D : public IHandle {
    virtual uint32_t    GetNumMipLevels() const = 0;
    virtual uint32_t    GetWidth() const = 0;
    virtual uint32_t    GetHeight() const = 0;
    virtual uint32_t    GetSize() const = 0;
    virtual TEX_FORMAT  GetFormat() const = 0;
    virtual uint32_t    GetPixelSize() const = 0;
    virtual TEX_FLAGS   GetFlags() const = 0;
    virtual void        Lock(uint32_t level, void** outData, uint32_t& outPitch) = 0;
    virtual void        Unlock() = 0;
};

struct IRenderContext : public IHandle {
    // Setters
    // Getters
    virtual IWindowContext* GetWindowContext() = 0;
    // Object creation
    virtual ITexture2D*     CreateTexture2D(const std::wstring& texPath) = 0;
    virtual ITexture2D*     CreateTexture2D(uint32_t mips, uint32_t width, uint32_t height, TEX_FORMAT format, TEX_FLAGS flags) = 0;
    // Render
    virtual void            Clear(uint32_t flags, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float depth, uint8_t stencil) = 0;
    virtual void            Present() = 0;
};

struct IRenderContextBuilder 
    : public IHandle
    , public IWindowSetup {

    virtual void            SetApiType(RC_API_TYPE apiType) = 0;
    virtual RC_API_TYPE     GetApiType() const = 0;
    virtual IRenderContext* GetResult() = 0;

    static IRenderContextBuilder* Create();
};