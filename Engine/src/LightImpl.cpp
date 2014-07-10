#include "LightImpl.h"
#include "HandleImpl.h"

Light::Light(LightType type, vector3f_t position)
    : m_type(type)
{
    m_position = position;
}

Light::~Light()
{
}

ILightPtr Light::Clone() const
{
    CLONE_HANDLE(ILight, Light);
}

const Light::GLMatrixDesc& Light::GetGLDesc() const
{
    return m_glDesc;
}

void Light::SetCenter(const vector3f_t& center)
{
    SetCenterImpl(m_glDesc, center);
}

void Light::SetCenter(float x, float y, float z)
{
    SetCenterImpl(m_glDesc, vector3f_t(x, y, z));
}

void Light::ShiftCenter(const vector3f_t& shift)
{
    ShiftCenterImpl(m_glDesc, shift);
}

void Light::ShiftCenter(float shiftX, float shiftY, float shiftZ)
{
    ShiftCenterImpl(m_glDesc, vector3f_t(shiftX, shiftY, shiftZ));
}

void Light::SetPosition(const vector3f_t& pos) 
{
    SetPositionImpl(m_glDesc, pos);
    // TODO: implement attahment
}

void Light::SetPosition(float x, float y, float z)
{
    SetPositionImpl(m_glDesc, vector3f_t(x, y, z));
}

void Light::Shift(const vector3f_t& pos)
{
    ShiftImpl(m_glDesc, pos);
    // TODO: implement attahment
}

void Light::Shift(float shiftX, float shiftY, float shiftZ)
{
    ShiftImpl(m_glDesc, vector3f_t(shiftX, shiftY, shiftZ));
}

void Light::SetAngle(const vector3f_t& angles)
{
    SetAngleImpl(m_glDesc, angles);
}

void Light::SetAngle(float angleX, float angleY, float angleZ)
{
    SetAngleImpl(m_glDesc, vector3f_t(angleX, angleY, angleZ));
}

void Light::Rotate(const vector3f_t& angles)
{
    RotateImpl(m_glDesc, angles);
}

void Light::Rotate(float angleX, float angleY, float angleZ)
{
    RotateImpl(m_glDesc, vector3f_t(angleX, angleY, angleZ));
}

vector3f_t Light::GetAngle() const
{
    return m_angle;
}

vector3f_t Light::GetPosition() const
{
    return m_position;
}

vector3f_t Light::GetCenter() const
{
    return m_center;
}