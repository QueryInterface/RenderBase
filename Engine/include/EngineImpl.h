#pragma once
#include "Engine.h"
#include "RenderContext.h"

class EngineImpl final : public IEngine
{
public:
    EngineImpl();
    ~EngineImpl();

    virtual ILightPtr               CreateLight(LightType type, vector3f_t position) override;
    virtual ICameraPtr              CreateCamera(const CameraSetup&) override;
    virtual IScenePtr               CreateScene() override;

    virtual void                    SetScene(IScenePtr scene) override;
    virtual IWindow&                GetWindow() const override;
    virtual void                    Run(IEngineCallbacks* callbacks) override;
private:
    IRenderContextPtr m_renderContext;
    IScenePtr         m_scene;
};