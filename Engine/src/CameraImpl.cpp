#include "CameraImpl.h"
#include "HandleImpl.h"

Camera::Camera(vector3d eye, vector3d at, vector3d up)
    : m_eye(eye)
    , m_at(at)
    , m_up(up)
{
}

Camera::~Camera()
{
}

ICameraPtr Camera::Clone() const
{
    CLONE_HANDLE(ICamera, Camera, m_eye, m_at, m_up);
}