#include "LightImpl.h"
#include "HandleImpl.h"

Light::Light()
{
}

Light::~Light()
{
}

ILightPtr Light::Clone() const
{
    CLONE_HANDLE(ILight, Light);
}

void Light::SetPosition(vector3f_t pos)
{
    m_position = pos;
}

void Light::SetPosition(float x, float y, float z)
{
    SetPosition(vector3f_t(x, y, z));
}

vector3f_t Light::GetPosition() const
{
    return m_position;
}