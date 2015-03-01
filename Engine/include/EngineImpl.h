#pragma once
#include "Engine.h"
#include "RenderContext.h"
#include "SceneImpl.h"

class EngineImpl : public IEngine
{
public:
    virtual ILight::Ptr             CreateLight(LightType type, vector3f_t position) override;
    virtual ICamera::Ptr            CreateCamera(const CameraDesc&) override;
    virtual IScene::Ptr             CreateScene() override;

    virtual void                    SetScene(IScene::Ptr scene) override;
    virtual IWindow&                GetWindow() const override;
    virtual void                    Run(IEngineCallbacks* callbacks) override;
protected:
    EngineImpl();
    ~EngineImpl();
private:
    PREVENT_COPY(EngineImpl);

    IRenderContextPtr m_renderContext;
    shared_ptr<Scene> m_scene;
};