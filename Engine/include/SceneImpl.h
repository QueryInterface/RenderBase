#pragma once
#include "Engine.h"
#include <list>

class Scene : public IScene
{
public:
    Scene();
    ~Scene();

    virtual void AddObject(IObjectPtr& object) override;
    virtual void AddLight(ILightPtr& light) override;
    virtual void SetCamera(ICameraPtr& camera) override;

    virtual void Render() const override;
private:
    std::list<IObjectPtr>   m_objects;
    std::list<ILightPtr>    m_lights;
    ICameraPtr              m_camera;
};