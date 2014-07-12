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
    // IClonable
    virtual ILightPtr       Clone() const override;
    // Unlike 'GetPosition' returns position of light. Keep in mind that GetPosition functions instead returns position of assigned center in world space.
    virtual vector3f_t GetLightPosition() const;
    // ISceneElement
    scene_elements_functions_impl
    scene_elements_gets_impl

private:
    LightType  m_type;
    vector4f_t m_lightPosition;
};

typedef std::shared_ptr<Light> LightPtr;