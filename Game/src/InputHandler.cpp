#include "InputHandler.h"

GameInputHandler::GameInputHandler()
    : m_moveSpeed(1.0)
    , m_rotateSpeed(1.0)
    , m_mousePosPrevious(0, 0)
    , m_mouseDown(false)
    , m_yAsixInvert(false)
    , m_rotationFunction(nullptr)
{
    ::memset(m_moveFlags, 0, sizeof(m_moveFlags));
    SetRotationType(RotationType::Free);
}

GameInputHandler::~GameInputHandler()
{
}

void GameInputHandler::SetRotationType(RotationType type)
{
    switch (type)
    {
    case RotationType::Free:
        {
            m_rotationFunction = &GameInputHandler::updateFreeRotation;
        } break;
    case RotationType::FirstPerson:
        {
            m_rotationFunction = &GameInputHandler::updateFPRotation;
        } break;
    default:
        assert(0);
    }
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
    switch (key)
    {
    case EKey::EK_MOUSE_BUTTON_RIGHT:
        {
            m_mouseDown = true;
            m_mousePosCurrent.x = x;
            m_mousePosCurrent.y = y;
            m_mousePosPrevious = m_mousePosCurrent;
        } break;
    default: break;
    }
}

void GameInputHandler::OnMouseUp(EKey key, uint32_t x, uint32_t y)
{
    switch (key)
    {
    case EKey::EK_MOUSE_BUTTON_RIGHT:
        {
            m_mousePosCurrent.x = x;
            m_mousePosCurrent.y = y;
            m_mouseDown = false;
        } break;
    default: break;
    }    
}

void GameInputHandler::OnMouseMove(uint32_t x, uint32_t y)
{
    if (m_mouseDown)
    {
        m_mousePosCurrent.x = x;
        m_mousePosCurrent.y = y;
    }
}

// IInputHandler
void GameInputHandler::Update(ICameraPtr& camera, float elapsedMs)
{
    assert(m_rotationFunction);
    updateShift(camera, elapsedMs);
    (this->*m_rotationFunction)(camera);
    //this->(*m_rotationFunction)(camera);
}

void GameInputHandler::updateShift(ICameraPtr& camera, float elapsedMs)
{
    if (m_moveFlags[MoveFlags::Forward] || m_moveFlags[MoveFlags::Up] || m_moveFlags[MoveFlags::Right])
    {
        float velocity = elapsedMs * m_moveSpeed / 1000; // Velocity in units per second
        CameraDesc cameraDesc = camera->GetDesc();
        vector3f_t& forward = cameraDesc.Direction;
        vector3f_t& up = cameraDesc.Up;
        vector3f_t right = glm::cross(forward, up);

        vector3f_t shift = forward * (float)m_moveFlags[MoveFlags::Forward] + up * (float)m_moveFlags[MoveFlags::Up] + right * (float)m_moveFlags[MoveFlags::Right];
        if (shift.x || shift.y || shift.z)
        {
            shift = glm::normalize(shift);
        }
        char buf[1024];
        sprintf_s(buf, "%f, %f, %f", shift.x, shift.y, shift.z);
        OutputDebugStringA(buf);
        camera->Shift(CoordType::Global, shift * velocity);
    }
}

void GameInputHandler::updateFreeRotation(ICameraPtr& camera)
{
    if (m_mousePosCurrent != m_mousePosPrevious)
    {
        CameraDesc cameraDesc = camera->GetDesc();
        vector3f_t& forward = cameraDesc.Direction;
        vector3f_t& up = cameraDesc.Up;
        vector3f_t right = glm::cross(forward, up);

        vector2i_t direction = m_mousePosCurrent - m_mousePosPrevious;
        vector2f_t direction_fn = direction;
        // Calculate percent of direction offset to window size
        direction_fn.x /= IEngine::Instance().GetWindow().GetWidth();
        direction_fn.y /= IEngine::Instance().GetWindow().GetHeight();

        direction_fn = m_rotateSpeed * direction_fn;
        direction_fn = direction_fn;
        float yaw = -direction_fn.x;
        float pitch = -direction_fn.y;
        if (m_yAsixInvert) 
            pitch = -pitch;
        quat q_yaw = glm::angleAxis(yaw, vector3f_t(0, 1, 0));
        quat q_pitch = glm::angleAxis(pitch, right);
        camera->Rotate(CoordType::Local, q_yaw);
        camera->Rotate(CoordType::Local, q_pitch);
        m_mousePosPrevious = m_mousePosCurrent;
    }    
}

void GameInputHandler::updateFPRotation(ICameraPtr& camera)
{
    camera;
}