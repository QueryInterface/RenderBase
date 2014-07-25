#include "LightImpl.h"
#include "HandleImpl.h"

Light::Light(LightType type, vector3f_t position)
    : m_type(type)
{
    SetPosition(CoordType::World, position);
}

Light::~Light()
{
}

ILightPtr Light::Clone() const
{
    CLONE_HANDLE(ILight, Light);
}