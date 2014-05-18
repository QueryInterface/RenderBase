#include "EngineImpl.h"

EngineImpl::EngineImpl()
{
    RenderContextBuilderPtr builder = RenderContextBuilder::Create();
    m_renderContext = builder->GetResult();
}

EngineImpl::~EngineImpl()
{
}

void EngineImpl::SetScene(IScenePtr scene)
{
    scene;   
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
