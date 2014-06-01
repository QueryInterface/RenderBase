#pragma once
#include "Engine.h"
#include "RenderContext.h"

class EngineImpl final : public IEngine
{
public:
    EngineImpl();
    ~EngineImpl();

    virtual ILightPtr               CreateLight() override;
    virtual ICameraPtr              CreateCamera(vector3d eye, vector3d at, vector3d up) override;
    virtual IScenePtr               CreateScene() override;

    virtual void                    SetScene(IScenePtr scene) override;
    virtual IWindow*                GetWindow() const override;
    virtual void                    Run() override;
private:
    IRenderContextPtr m_renderContext;
    IScenePtr         m_scene;
};