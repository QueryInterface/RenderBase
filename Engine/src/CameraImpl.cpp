#include "CameraImpl.h"
#include "HandleImpl.h"

Camera::Camera(const CameraSetup& setup)
    : m_setup(setup)
{
    initCamera();
}

Camera::~Camera()
{
}

ICameraPtr Camera::Clone() const
{
    CLONE_HANDLE(ICamera, Camera);
}

const vector3f_t& Camera::GetEye()
{
    updateState();
    return m_eye;
}

const vector3f_t& Camera::GetAt()
{
    updateState();
    return m_at;
}

const vector3f_t& Camera::GetUp()
{
    updateState();
    return m_up;
}

const float Camera::GetFieldOfView() const
{
    return m_setup.FieldOfViewY;
}

const float Camera::GetNearZ() const
{
    return m_setup.NearZ;
}

const float Camera::GetFarZ() const
{
    return m_setup.FarZ;
}

void Camera::SetFiledOfViewY(float fovy)
{
    m_setup.FieldOfViewY = fovy;
    initCamera();
}

void Camera::initCamera()
{
    IWindow& window = IEngine::Instance().GetWindow();
    float aspect = 1.0f * window.GetWidth() / window.GetHeight();
    m_viewMatrix = glm::lookAt(m_setup.Eye, m_setup.At, m_setup.Up);
	m_projectionMatrix = glm::perspective(m_setup.FieldOfViewY, aspect, m_setup.NearZ, m_setup.FarZ);
}

void Camera::updateState()
{
    if (HasChanged(true))
    {
        vector4f_t at = GetMatrix(CoordType::World) * GetMatrix(CoordType::Local) * vector4f_t(m_setup.At, 1.0);
        m_at = vector3f_t(at);
        vector4f_t up = GetMatrix(CoordType::World) * GetMatrix(CoordType::Local) * vector4f_t(m_setup.Up, 1.0);
        m_up = vector3f_t(up);
        vector4f_t eye = GetMatrix(CoordType::World) * GetMatrix(CoordType::Local) * vector4f_t(m_setup.Eye, 1.0);
        m_eye = vector3f_t(eye);
    }
}
