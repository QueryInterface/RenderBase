#pragma once
#include "MathBasics.h"

class SceneElementImpl
{
public:
    struct GLMatrixDesc
    {
        glm::mat4 ObjectMatrix;
        glm::mat4 WorldMatrix;
    };

    SceneElementImpl();
    // Virtual destructor to prevent direct object creation
    virtual ~SceneElementImpl() = 0;

    void        SetCenterImpl(GLMatrixDesc& out, const vector3f_t& m_center);
    void        ShiftCenterImpl(GLMatrixDesc& out, const vector3f_t& shift);

    void        SetPositionImpl(GLMatrixDesc& out, const vector3f_t& pos);
    void        ShiftImpl(GLMatrixDesc& out, const vector3f_t& shift);

    void        SetAngleImpl(GLMatrixDesc& out, const vector3f_t& angles);
    void        RotateImpl(GLMatrixDesc& out, const vector3f_t& angles);

    void        SetScaleImpl(GLMatrixDesc& out, const vector3f_t& scales);
    void        ScaleImpl(GLMatrixDesc& out, const vector3f_t& scales);

protected:
    vector3f_t  m_center;
    vector3f_t  m_position;
    vector3f_t  m_angle;
    vector3f_t  m_scale;
};