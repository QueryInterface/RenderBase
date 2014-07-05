#include "CameraImpl.h"
#include "HandleImpl.h"

Camera::Camera(vector3f_t eye, vector3f_t at, vector3f_t up)
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
    CLONE_HANDLE(ICamera, Camera);
}