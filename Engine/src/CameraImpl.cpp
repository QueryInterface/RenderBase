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

const Camera::GLDesc& Camera::GetGLDesc() const
{
    return m_glDesc;
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

void Camera::SetCenter(const vector3f_t& center)
{
    SetCenterImpl(m_glDesc, center);
}

void Camera::SetCenter(float x, float y, float z)
{
    SetCenterImpl(m_glDesc, vector3f_t(x, y, z));
}

void Camera::ShiftCenter(const vector3f_t& shift)
{
    ShiftCenterImpl(m_glDesc, shift);
}

void Camera::ShiftCenter(float shiftX, float shiftY, float shiftZ)
{
    ShiftCenterImpl(m_glDesc, vector3f_t(shiftX, shiftY, shiftZ));
}

void Camera::SetPosition(const vector3f_t& pos) 
{
    SetPositionImpl(m_glDesc, pos);
    // TODO: implement attahment
}

void Camera::SetPosition(float x, float y, float z)
{
    SetPositionImpl(m_glDesc, vector3f_t(x, y, z));
}

void Camera::Shift(const vector3f_t& pos)
{
    ShiftImpl(m_glDesc, pos);
    // TODO: implement attahment
}

void Camera::Shift(float shiftX, float shiftY, float shiftZ)
{
    ShiftImpl(m_glDesc, vector3f_t(shiftX, shiftY, shiftZ));
}

void Camera::SetAngle(const vector3f_t& angles)
{
    SetAngleImpl(m_glDesc, angles);
}

void Camera::SetAngle(float angleX, float angleY, float angleZ)
{
    SetAngleImpl(m_glDesc, vector3f_t(angleX, angleY, angleZ));
}

void Camera::Rotate(const vector3f_t& angles)
{
    RotateImpl(m_glDesc, angles);
}

void Camera::Rotate(float angleX, float angleY, float angleZ)
{
    RotateImpl(m_glDesc, vector3f_t(angleX, angleY, angleZ));
}

vector3f_t Camera::GetAngle() const
{
    return m_angle;
}

vector3f_t Camera::GetPosition() const
{
    return m_position;
}

vector3f_t Camera::GetCenter() const
{
    return m_center;
}

void Camera::initCamera()
{
    IWindow* window = IEngine::Instance()->GetWindow();
    float aspect = 1.0f * window->GetWidth() / window->GetHeight();
    m_glDesc.ViewMatrix = glm::lookAt(m_setup.Eye, m_setup.At, m_setup.Up);
	m_glDesc.ProjMatrix = glm::perspective(m_setup.FieldOfViewY, aspect, m_setup.NearZ, m_setup.FarZ);
}