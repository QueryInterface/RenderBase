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
    Camera(const CameraSetup& setup);
    virtual ~Camera();
    // IClonable
    virtual ICameraPtr Clone() const override;
    // ICamera
    virtual const CameraSetup&  GetCameraSetup() const override;
    virtual void                SetFiledOfViewY(float fovy) override;
    scene_elements_functions_impl
    scene_elements_gets_impl
    // Camera
    // Unlike 'GetPosition' returns position of camera. Keep in mind that GetPosition functions instead returns position of assigned center in world space.
    virtual const glm::mat4 GetViewMatrix() const {return m_viewMatrix * glm::inverse(m_worldMatrix * m_elementMatrix);}
    virtual const glm::mat4& GetProjectionMatrix() const {return m_projectionMatrix;}
    virtual vector3f_t GetCameraPosition() const;
private:
    void initCamera();
    
    CameraSetup     m_setup;
    vector4f_t      m_cameraPosition;
    glm::mat4       m_viewMatrix;
    glm::mat4       m_projectionMatrix;
};

typedef std::shared_ptr<Camera> CamerPtr;