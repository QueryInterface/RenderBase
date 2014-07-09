#pragma once
#include "Engine.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

class Camera : public ICamera
{
public:
    struct GLDesc
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjMatrix;
    };
public:
    Camera(const CameraSetup& setup);
    virtual ~Camera();
    virtual const GLDesc& GetGLDesc() const;

    // ICamera
    virtual ICameraPtr Clone() const override;
    virtual const CameraSetup&  GetCameraSetup() const override;
    virtual void                SetFiledOfViewY(float fovy) override;
private:
    void initCamera();
    
    CameraSetup m_setup;
    GLDesc      m_glDesc;
};

typedef std::shared_ptr<Camera> CamerPtr;