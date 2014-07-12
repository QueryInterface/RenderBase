#include "LightImpl.h"
#include "HandleImpl.h"

Light::Light(LightType type, vector3f_t position)
    : m_type(type)
    , m_lightPosition(0, 0, 0, 1)
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

vector3f_t Light::GetLightPosition() const
{
    return vector3f_t(m_worldMatrix * m_elementMatrix * m_lightPosition);
}
