#pragma once
#include "Engine.h"

class Camera : public ICamera
{
public:
    Camera(vector3f_t eye, vector3f_t at, vector3f_t up);
    virtual ~Camera();
    virtual ICameraPtr Clone() const;
private:
    vector3f_t m_eye;
    vector3f_t m_at;
    vector3f_t m_up;
};