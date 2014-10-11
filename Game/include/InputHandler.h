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
    void SetMoveSpeed(float speed) {m_moveSpeed = speed;}
    void SetRotateSpeed(float speed) {m_rotateSpeed = speed;};
    void InvertYAxis(bool invert);
    // IWindowCallbacks
    virtual void OnKeyDown(EKey key) override;
    virtual void OnKeyUp(EKey key) override;
    virtual void OnMouseDown(EKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseUp(EKey key, uint32_t x, uint32_t y) override;
    virtual void OnMouseMove(uint32_t x, uint32_t y) override;
    // IInputHandler
    virtual void Update(ICameraPtr& camera, float elapsedMs) override;
private:
    // Shift parameters
    float       m_moveSpeed;
    int32_t     m_moveFlags[3];
    // Rotate paremters
    float       m_rotateSpeed;
    bool        m_yAsixInvert;
    bool        m_mouseDown;
    vector2u_t  m_mousePosPrevious;
    vector2u_t  m_mousePosCurrent;
};
