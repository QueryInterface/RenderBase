#pragma once
#include "Engine.h"
#include "SceneElementImpl.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class Camera 
    : public ICamera
    , protected SceneElementImpl
{
public:
    Camera(const CameraDesc& setup);
    virtual ~Camera();
    // IClonable
    virtual ICameraPtr Clone() const override;
    // ICamera
    virtual const CameraDesc&   GetDesc() const override;
    virtual void                SetFiledOfViewY(float fovy) override;
    scene_elements_impl;
    // Unlike 'GetPosition' returns position of camera. Keep in mind that GetPosition functions instead returns position of assigned center in world space.
    virtual const glm::mat4 GetViewMatrix() {return m_viewMatrix * glm::inverse(GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local));}
    virtual const glm::mat4& GetProjectionMatrix() const {return m_projectionMatrix;}
private:
    void initCamera();

    CameraDesc      m_desc;
    vector3f_t      m_eye;
    vector3f_t      m_at;
    vector3f_t      m_up;
    glm::mat4       m_viewMatrix;
    glm::mat4       m_projectionMatrix;
};

typedef std::shared_ptr<Camera> CamerPtr;