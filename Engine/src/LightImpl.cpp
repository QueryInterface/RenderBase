#include "LightImpl.h"
#include "HandleImpl.h"

Light::Light(LightType type, vector3f_t position)
    : m_type(type)
{
    SetPosition(CoordType::Global, position);
}

Light::~Light()
{
}

ILight::Ptr Light::Clone() const
{
    CLONE_HANDLE(ILight, Light);
}