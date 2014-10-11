#pragma once
#include "Engine.h"
#include "ResourceOverseer.h"
#include <Overmind.h>
#include <list>

struct IInputHandler;

class Game 
    : public IEngineCallbacks
    , public IWindowCallbacks
{
public:
    Game(IInputHandler& inputHandler);
    ~Game();
    void InitHelpers();
    void InitScene0();
    void InitScene1();
    void Start();
    // IEngineCallbacks
    void OnSceneUpdate() override;
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

    IInputHandler&          m_inputHandler;
    Overmind&               m_overmind;
};