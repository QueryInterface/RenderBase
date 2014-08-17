#pragma once
#include "Engine.h"
#include "ResourceOverseer.h"
#include <Constructor.h>
#include <list>

class CameraMove
{
public:
    CameraMove();
    enum class Type : uint8_t
    {
        Left, 
        Right,      
        Up, 
        Down, 
        Forward, 
        Backward
    };
    void SetMoveSpeed(float speed);
    void EnableMove(CameraMove::Type type, const vector3f_t& shift);
    void DisableMove(CameraMove::Type type);
    void EnableRotate(uint32_t x, uint32_t  y);
    void DisableRotate();
    void OnMouseMove(uint32_t x, uint32_t y);
    void Process(ICameraPtr& camera, float timeElapsed);
private:
    float       m_moveSpeed;
    vector3f_t  m_shifts[6];
    uint8_t     m_moveMask;
    uint32_t    m_previousX;
    uint32_t    m_previousY;
    uint32_t    m_currentX;
    uint32_t    m_currentY;
    bool        m_enableRotate;
};

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
    virtual void OnKeyDown(EKey key) override;
    virtual void OnKeyUp(EKey key) override;
    virtual void OnMouseDown(EKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseUp(EKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseMove(uint32_t x, uint32_t y) override;
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
    // Movement commands
    CameraMove              m_cameraMove;
    float                   m_cameraMoveSpeed;
    float                   m_cameraRotateSpeed;

    Constructor&            m_builder;
};