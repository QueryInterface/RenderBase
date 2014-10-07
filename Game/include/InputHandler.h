#pragma once
#include "Engine.h"

struct IInputHandler : public IWindowCallbacks
{
    virtual void Update(ICameraPtr& camera, float elapsedMs) = 0;
};

class GameInputHandler : public IInputHandler
{
    enum MoveFlags
    {
        Up,
        Right,
        Forward,
    };
public:
    GameInputHandler();
    ~GameInputHandler();
    void SetMoveSpeed(float speed);
    void SetRotateSpeed(float speed);
    // IWindowCallbacks
    virtual void OnKeyDown(EKey key) override;
    virtual void OnKeyUp(EKey key) override;
    virtual void OnMouseDown(EKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseUp(EKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseMove(uint32_t x, uint32_t y) override;
    // IInputHandler
    virtual void Update(ICameraPtr& camera, float elapsedMs) override;
private:
    float       m_moveSpeed;
    float       m_rotateSpeed;
    vector3f_t  m_direction;

    vector3f_t  m_rotatePrevious;
    int32_t     m_moveFlags[3];
};
