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
    struct GLDesc : public GLMatrixDesc
    {
         glm::mat4 ViewMatrix;
         glm::mat4 ProjMatrix;
    };

    Camera(const CameraSetup& setup);
    virtual ~Camera();
    virtual const GLDesc& GetGLDesc() const;

    // ICamera
    virtual ICameraPtr Clone() const override;
    virtual const CameraSetup&  GetCameraSetup() const override;
    virtual void                SetFiledOfViewY(float fovy) override;

    virtual void                SetCenter(const vector3f_t& center) override;
    virtual void                SetCenter(float x, float y, float z) override;
    virtual void                ShiftCenter(const vector3f_t& shift) override;
    virtual void                ShiftCenter(float shiftX, float shiftY, float shiftZ) override;

    virtual void                SetPosition(const vector3f_t& pos) override;
    virtual void                SetPosition(float x, float y, float z) override;
    virtual void                Shift(const vector3f_t& shift) override;
    virtual void                Shift(float shiftX, float shiftY, float shiftZ) override;

    virtual void                SetAngle(const vector3f_t& angles) override;
    virtual void                SetAngle(float angleX, float angleY, float angleZ) override;
    virtual void                Rotate(const vector3f_t& angles) override;
    virtual void                Rotate(float angleX, float angleY, float angleZ) override;

    virtual vector3f_t          GetAngle() const override;  
    virtual vector3f_t          GetPosition() const override;  
    virtual vector3f_t          GetCenter() const override; 
private:
    void initCamera();
    
    CameraSetup     m_setup;
    GLDesc          m_glDesc;
};

typedef std::shared_ptr<Camera> CamerPtr;