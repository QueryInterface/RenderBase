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
    virtual ILightPtr       Clone() const;
    // ISceneElement
    position_update_functions
    angle_update_functions
    angle_update_functions_ex
private:
    LightType               m_type;
};

typedef std::shared_ptr<Light> LightPtr;