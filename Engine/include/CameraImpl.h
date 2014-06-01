#pragma once
#include "Engine.h"

class Camera : public ICamera
{
public:
    Camera(vector3d eye, vector3d at, vector3d up);
    virtual ~Camera();
    virtual ICameraPtr Clone() const;
private:
    vector3d m_eye;
    vector3d m_at;
    vector3d m_up;
};