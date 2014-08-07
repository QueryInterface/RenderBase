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

const CameraSetup& Camera::GetCameraSetup() const
{
    return m_setup;
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