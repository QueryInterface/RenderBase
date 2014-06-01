#include "SceneImpl.h"
#include "RenderContext.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddObject(IObjectPtr& object)
{
    m_objects.push_back(object);
}

void Scene::AddLight(ILightPtr& light)
{
    m_lights.push_back(light);
}

void Scene::SetCamera(ICameraPtr& camera)
{
    m_camera = camera;
}

void Scene::Render() const
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}