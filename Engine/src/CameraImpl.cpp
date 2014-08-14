#include "CameraImpl.h"
#include "HandleImpl.h"

Camera::Camera(const CameraDesc& setup)
    : m_desc(setup)
{
    initCamera();
    // Set initial position of camera into internal structures
    SetPositionInit(CoordType::Global, m_desc.Eye);
    // Register additional vectors for update when camera moves/rotates
    EnableVectorUpdate(&m_desc.Eye);
    EnableVectorUpdate(&m_desc.At);
    EnableVectorUpdate(&m_desc.Up);
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
    m_viewMatrix = glm::lookAt(m_desc.Eye, m_desc.At, m_desc.Up);
	m_projectionMatrix = glm::perspective(m_desc.FieldOfViewY, aspect, m_desc.NearZ, m_desc.FarZ);
}
