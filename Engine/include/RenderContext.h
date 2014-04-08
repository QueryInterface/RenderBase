#pragma once
#include "common.h"
#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;

enum class RC_API_TYPE {
    GLES2 = 0,
};

enum class WINDOW_MSG {
    FOREGROUND,
    BACKGROUND,
    QUIT
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


struct IWindow {
    virtual void        SetWidth(uint32_t width) = 0;
    virtual void        SetHeight(uint32_t height) = 0;
    virtual void        SetTitle(const std::string& title) = 0;
    virtual void        SetFullscreen(bool fullscreen) = 0;

    virtual uint32_t    GetWidth() const = 0;
    virtual uint32_t    GetHeight() const = 0;
    virtual std::string GetTitle() const = 0;
    virtual bool        IsFullscreen() const = 0;

    virtual IHandle*    RegisterEventCallback(const std::shared_ptr<EventCallback>& callback) = 0;
    virtual WINDOW_MSG  ProcessMessage() = 0;
};

struct IRenderContext : public IHandle {
    // Setters
    // Getters
    virtual IWindow*        GetWindow() = 0;
    // Render
    virtual void            Present() = 0;
};

struct IRenderContextBuilder 
    : public IHandle {

    virtual void            SetWidth(uint32_t width) = 0;
    virtual void            SetHeight(uint32_t height) = 0;
    virtual void            SetTitle(const std::string& title) = 0;
    virtual void            SetFullscreen(bool fullscreen) = 0;
    virtual void            SetApiType(RC_API_TYPE apiType) = 0;

    virtual IRenderContext* GetResult() = 0;

    static LIB_EXPORT IRenderContextBuilder* CALLING_CONVENTION Create();
};



