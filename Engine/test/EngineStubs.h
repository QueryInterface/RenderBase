#pragma once
#include "Engine.h"

class WindowStub : public IWindow
{
public:
    WindowStub()
        : m_width(640)
        , m_height(480)
        , m_fullscreen(false)
        , m_title("")
    {
    }
    virtual void        SetWidth(uint32_t width) {m_width = width;}
    virtual void        SetHeight(uint32_t height) {m_height = height;}
    virtual void        SetTitle(const std::string& title) {m_title = title;}
    virtual void        SetFullscreen(bool fullscreen) {m_fullscreen = fullscreen;}

    virtual uint32_t    GetWidth() const {return m_width;}
    virtual uint32_t    GetHeight() const {return m_height;}
    virtual std::string GetTitle() const {return m_title;}
    virtual bool        IsFullscreen() const {return m_fullscreen;}

    virtual void        RegisterInputCallbacks(IWindowCallbacks* callbacks) {callbacks;}
    virtual void        UnregisterInputCallbacks(IWindowCallbacks* callbacks) {callbacks;}
private:
    uint32_t        m_width;
    uint32_t        m_height;
    std::string     m_title;
    bool            m_fullscreen;
};

class EngineStub : public IEngine
{
public:
    EngineStub()
    {

    }
    virtual void                    SetScene(IScenePtr scene) {scene;}
    virtual IWindow&                GetWindow() const {return m_window;}
    virtual void                    Run(IEngineCallbacks* callbacks) {callbacks;}

    virtual ILightPtr               CreateLight(LightType type, vector3f_t position) {type; position; return nullptr;};
    virtual ICameraPtr              CreateCamera(const CameraDesc& setup) {setup; return nullptr;}
    virtual IScenePtr               CreateScene() {return nullptr;}
private:
    mutable WindowStub m_window;
};

LIB_EXPORT IEngine&  CALLING_CONVENTION IEngine::Instance()
{
    static std::unique_ptr<IEngine> s_engine;
    if (!s_engine)
        s_engine.reset(new EngineStub());
    return *s_engine.get();   
}
