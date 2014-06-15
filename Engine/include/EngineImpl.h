#pragma once
#include "Engine.h"
#include "RenderContext.h"

class EngineImpl final : public IEngine
{
public:
    EngineImpl();
    ~EngineImpl();

    virtual ILightPtr               CreateLight() override;
    virtual ICameraPtr              CreateCamera(vector3f_t eye, vector3f_t at, vector3f_t up) override;
    virtual IScenePtr               CreateScene() override;

    virtual void                    SetScene(IScenePtr scene) override;
    virtual IWindow*                GetWindow() const override;
    virtual void                    Run() override;
private:
    IRenderContextPtr m_renderContext;
    IScenePtr         m_scene;
};