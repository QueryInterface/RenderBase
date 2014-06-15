#include "EngineImpl.h"
#include "CameraImpl.h"
#include "SceneImpl.h"
#include "LightImpl.h"
#include "HandleImpl.h"

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

ILightPtr EngineImpl::CreateLight()
{
    return static_pointer_cast<ILight>(make_shared_handle<Light>());
}

ICameraPtr EngineImpl::CreateCamera(vector3f_t eye, vector3f_t at, vector3f_t up)
{
    return static_pointer_cast<ICamera>(make_shared_handle<Camera>(eye, at, up));
}

IScenePtr EngineImpl::CreateScene()
{
    return static_pointer_cast<IScene>(make_shared_handle<Scene>());
}

void EngineImpl::SetScene(IScenePtr scene)
{
    m_scene = scene;
}

IWindow* EngineImpl::GetWindow() const
{
    return m_renderContext->GetWindow();
}

void EngineImpl::Run()
{
    WINDOW_MSG msg = WINDOW_MSG::FOREGROUND;
    while((msg = m_renderContext->GetWindow()->ProcessMessage()) != WINDOW_MSG::QUIT) 
    {
        if (msg == WINDOW_MSG::FOREGROUND)
        {
            m_scene->Render();
        }
        m_renderContext->Present();
    }
}

IEngine* IEngine::Instance()
{
    static std::unique_ptr<IEngine> s_engine;
    if (!s_engine)
        s_engine.reset(new EngineImpl());
    return s_engine.get();   
}
