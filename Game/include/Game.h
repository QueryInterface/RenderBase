#pragma once
#include "Engine.h"
#include "ResourceOverseer.h"
#include <Constructor.h>

class Game 
    : public IEngineCallbacks
    , public IWindowCallbacks
{
public:
    Game();
    ~Game();
    void InitHelpers();
    void InitScene0();
    void InitScene1();
    void Start();
    // IEngineCallbacks
    void OnSceneUpdate() override;
    // IWindowCallbacks
    void OnKeyDown(EKey key) override;
    void OnKeyUp(EKey key) override;
private:
    void centerObject(IObjectPtr& obj);

    PREVENT_COPY(Game);

    IEngine&                m_engine;
    IWindow&                m_window;
    IResourceOverseer&      m_resourceOverseer;
    IScenePtr               m_scene;
    ICameraPtr              m_camera;
    ILightPtr               m_light;
    IObjectPtr              m_lightShape;
    std::vector<IObjectPtr> m_objects;

    Constructor&            m_builder;
};