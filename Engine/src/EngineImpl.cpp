#include "EngineImpl.h"
#include "CameraImpl.h"
#include "SceneImpl.h"
#include "LightImpl.h"
#include "HandleImpl.h"
#include "ErrorHandler.h"
#include "Utils.h"

EngineImpl::EngineImpl()
    : m_scene(nullptr)
    , m_renderContext(nullptr)
{
    RenderContextBuilderPtr builder = RenderContextBuilder::Create();
    m_renderContext = builder->GetResult();
}

EngineImpl::~EngineImpl()
{
}

ILight::Ptr EngineImpl::CreateLight(LightType type, vector3f_t position)
{
    return static_pointer_cast<ILight>(make_shared_handle<Light>(type, position));
}

ICamera::Ptr EngineImpl::CreateCamera(const CameraDesc& setup)
{
    return static_pointer_cast<ICamera>(make_shared_handle<Camera>(setup));
}

IScene::Ptr EngineImpl::CreateScene()
{
    return static_pointer_cast<IScene>(make_shared_handle<Scene>());
}

void EngineImpl::SetScene(IScene::Ptr scene)
{
    m_scene = safe_pointer_cast<Scene>(scene);
}

IWindow& EngineImpl::GetWindow() const
{
    return m_renderContext->GetWindow();
}

void EngineImpl::Run(IEngineCallbacks* callbacks)
{
    WINDOW_MSG msg = WINDOW_MSG::FOREGROUND;
    while((msg = m_renderContext->GetWindow().ProcessMessage()) != WINDOW_MSG::QUIT) 
    {
        if (msg == WINDOW_MSG::FOREGROUND)
        {
            if (callbacks)
            {
                callbacks->OnSceneUpdate();
            }
            m_scene->Render();
        }
        m_renderContext->Present();
    }
}

IEngine& IEngine::Instance()
{
    return Singleton<EngineImpl>();
}
