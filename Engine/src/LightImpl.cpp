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

const Light::GLDesc& Light::GetGLDesc() const
{
    return m_glDesc;
}