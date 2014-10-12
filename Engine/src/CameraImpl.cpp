#include "CameraImpl.h"
#include "HandleImpl.h"

Camera::Camera(const CameraDesc& setup)
    : m_originalDesc(setup)
    , m_desc(setup)
{
    initCamera();
    // Set initial position of camera into internal structures
    SetPositionInit(CoordType::Global, m_desc.EyePosition);
}

Camera::~Camera()
{
}

ICameraPtr Camera::Clone() const
{
    CLONE_HANDLE(ICamera, Camera);
}

const CameraDesc& Camera::GetDesc() const
{
    m_desc.EyePosition = vector3f_t(GetMatrix(CoordType::Global) * vector4f_t(m_originalDesc.EyePosition, 1));
    m_desc.Direction = vector3f_t(GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local) * vector4f_t(m_originalDesc.Direction, 0));
    m_desc.Up = vector3f_t(GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local) * vector4f_t(m_originalDesc.Up, 0));
    return m_desc;
}

void Camera::SetFiledOfViewY(float fovy)
{
    m_desc.FieldOfViewY = fovy;
    initCamera();
}

const glm::mat4 Camera::GetViewMatrix()
{
    const CameraDesc& desc = GetDesc();
    m_viewMatrix = glm::lookAt(desc.EyePosition, desc.EyePosition + desc.Direction, desc.Up);
    return m_viewMatrix;
}

void Camera::initCamera()
{
    m_viewMatrix = glm::lookAt(m_originalDesc.EyePosition, m_originalDesc.EyePosition + m_originalDesc.Direction, m_originalDesc.Up);
	m_projectionMatrix = glm::perspective(m_originalDesc.FieldOfViewY, m_originalDesc.Aspect, m_originalDesc.NearZ, m_originalDesc.FarZ);
}
