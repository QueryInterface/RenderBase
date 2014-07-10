#pragma once
#include "Engine.h"
#include "SceneElementImpl.h"

class Light 
    : public ILight
    , protected SceneElementImpl
{
public:
    Light(LightType type, vector3f_t position);
    virtual ~Light();
    virtual ILightPtr           Clone() const;
    virtual const GLMatrixDesc& GetGLDesc() const;

    // ISceneElement
    virtual void            SetCenter(const vector3f_t& m_center) override;
    virtual void            SetCenter(float x, float y, float z) override;
    virtual void            ShiftCenter(const vector3f_t& shift) override;
    virtual void            ShiftCenter(float shiftX, float shiftY, float shiftZ) override;

    virtual void            SetPosition(const vector3f_t& pos) override;
    virtual void            SetPosition(float x, float y, float z) override;
    virtual void            Shift(const vector3f_t& shift) override;
    virtual void            Shift(float shiftX, float shiftY, float shiftZ) override;

    virtual void            SetAngle(const vector3f_t& angles) override;
    virtual void            SetAngle(float angleX, float angleY, float angleZ) override;
    virtual void            Rotate(const vector3f_t& angles) override;
    virtual void            Rotate(float angleX, float angleY, float angleZ) override;

    virtual vector3f_t      GetAngle() const override;  
    virtual vector3f_t      GetPosition() const override;  
    virtual vector3f_t      GetCenter() const override;
private:
    GLMatrixDesc            m_glDesc;
    LightType               m_type;
};

typedef std::shared_ptr<Light> LightPtr;