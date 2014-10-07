#include "InputHandler.h"

GameInputHandler::GameInputHandler()
    : m_moveSpeed(1.0)
    , m_rotateSpeed(1.0)
    , m_direction(0.0, 0.0, 0.0)
    , m_rotatePrevious(0.0, 0.0, 0.0)
{
    ::memset(m_moveFlags, 0, sizeof(m_moveFlags));
}

GameInputHandler::~GameInputHandler()
{
}

inline void GameInputHandler::SetMoveSpeed(float speed)
{
    m_moveSpeed = speed;
}

inline void GameInputHandler::SetRotateSpeed(float speed)
{
    m_rotateSpeed = speed;
}

void GameInputHandler::OnKeyDown(EKey key)
{
    switch(key)
    {
    case EKey::EK_W:
        {
            ++m_moveFlags[MoveFlags::Forward];
        } break;
    case EKey::EK_S:
        {
            --m_moveFlags[MoveFlags::Forward];
        } break;
    case EKey::EK_D:
        {
            ++m_moveFlags[MoveFlags::Right];
        } break;
    case EKey::EK_A:
        {
            --m_moveFlags[MoveFlags::Right];
        } break;
    case EKey::EK_SPACE:
        {
            ++m_moveFlags[MoveFlags::Up];
        } break;
    case EKey::EK_LCTRL:
    case EKey::EK_RCTRL:
    case EKey::EK_C:
        {
            --m_moveFlags[MoveFlags::Up];
        } break;
    }
}

void GameInputHandler::OnKeyUp(EKey key)
{
    switch(key)
    {
    case EKey::EK_W:
        {
            --m_moveFlags[MoveFlags::Forward];
        } break;
    case EKey::EK_S:
        {
            ++m_moveFlags[MoveFlags::Forward];
        } break;
    case EKey::EK_D:
        {
            --m_moveFlags[MoveFlags::Right];
        } break;
    case EKey::EK_A:
        {
            ++m_moveFlags[MoveFlags::Right];
        } break;
    case EKey::EK_SPACE:
        {
            --m_moveFlags[MoveFlags::Up];
        } break;
    case EKey::EK_LCTRL:
    case EKey::EK_RCTRL:
    case EKey::EK_C:
        {
            ++m_moveFlags[MoveFlags::Up];
        } break;
    }
}

void GameInputHandler::OnMouseDown(EKey key, uint32_t x, uint32_t y)
{
    key; x; y;
}

void GameInputHandler::OnMouseUp(EKey key, uint32_t x, uint32_t y)
{
    key; x; y;
}

void GameInputHandler::OnMouseMove(uint32_t x, uint32_t y)
{
    x; y;
}

// IInputHandler
void GameInputHandler::Update(ICameraPtr& camera, float elapsedMs)
{
    float velocity = elapsedMs * m_moveSpeed / 1000; // Velocity in units per second
    CameraDesc cameraDesc = camera->GetDesc();
    vector3f_t& forward = cameraDesc.At;
    vector3f_t& up = cameraDesc.Up;
    vector3f_t right = glm::cross(forward, up);

    vector3f_t shift = forward * (float)m_moveFlags[MoveFlags::Forward] + up * (float)m_moveFlags[MoveFlags::Up] + right * (float)m_moveFlags[MoveFlags::Right];
    glm::normalize(shift);
    camera->Shift(CoordType::Global, shift * velocity);
}