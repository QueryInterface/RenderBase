#pragma once
#include "Engine.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class Camera : public ICamera
{
public:
    struct GLDesc
    {
    };
public:
    Camera(vector3f_t eye, vector3f_t at, vector3f_t up);
    virtual ~Camera();
    virtual ICameraPtr Clone() const;
private:
    vector3f_t m_eye;
    vector3f_t m_at;
    vector3f_t m_up;

    glm::mat4  
};

typedef std::shared_ptr<Camera> CamerPtr;