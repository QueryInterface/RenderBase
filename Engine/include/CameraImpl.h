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
    virtual const CameraDesc    GetDesc() const override;
    virtual void                SetFiledOfViewY(float fovy) override;
    scene_elements_impl;
    virtual const glm::mat4 GetViewMatrix() {return GetMatrix(CoordType::Global) * GetMatrix(CoordType::Local) * m_viewMatrix;}
    virtual const glm::mat4& GetProjectionMatrix() const {return m_projectionMatrix;}
private:
    void initCamera();

    CameraDesc          m_originalDesc;
    mutable CameraDesc  m_desc;
    vector3f_t          m_eye;
    vector3f_t          m_at;
    vector3f_t          m_up;
    glm::mat4           m_viewMatrix;
    glm::mat4           m_projectionMatrix;
};

typedef std::shared_ptr<Camera> CamerPtr;