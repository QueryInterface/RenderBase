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
    // ISceneElement
    scene_elements_impl;

private:
    LightType  m_type;
};

typedef std::shared_ptr<Light> LightPtr;