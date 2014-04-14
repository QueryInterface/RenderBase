#pragma once
#include "common.h"
#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::string;
using std::vector;

struct IWindow;

typedef shared_ptr<IWindow> IWindowPtr;

struct IHandle {
    virtual void Release() = 0;
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
};

struct IRenderContext : public IHandle {
};


