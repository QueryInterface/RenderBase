#include "CameraImpl.h"
#include "HandleImpl.h"

Camera::Camera(const CameraDesc& setup)
    : m_originalDesc(setup)
{
    initCamera();
    // Set initial position of camera into internal structures
    SetPositionInit(CoordType::Global, m_desc.Eye);
}

Camera::~Camera()
{
}

ICameraPtr Camera::Clone() const
{
    CLONE_HANDLE(ICamera, Camera);
}

const CameraDesc Camera::GetDesc() const
{
    m_desc.Eye = vector3f_t(GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local) * vector4f_t(m_originalDesc.Eye, 0));
    m_desc.At = vector3f_t(GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local) * vector4f_t(m_originalDesc.At, 0));
    m_desc.Up = vector3f_t(GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local) * vector4f_t(m_originalDesc.Up, 0));
    return m_desc;
}

void Camera::SetFiledOfViewY(float fovy)
{
    m_desc.FieldOfViewY = fovy;
    initCamera();
}

void Camera::initCamera()
{
    IWindow& window = IEngine::Instance().GetWindow();
    float aspect = 1.0f * window.GetWidth() / window.GetHeight();
    m_viewMatrix = glm::lookAt(m_originalDesc.Eye, m_originalDesc.At, m_originalDesc.Up);
	m_projectionMatrix = glm::perspective(m_originalDesc.FieldOfViewY, aspect, m_originalDesc.NearZ, m_originalDesc.FarZ);
}
