#pragma once
#include "Engine.h"
#include "RenderContext.h"

class EngineImpl : public IEngine
{
public:
    EngineImpl();
    ~EngineImpl();

    virtual void                    SetScene(IScenePtr scene);
    virtual IWindow*                GetWindow() const;
    virtual void                    Run();
private:
    IRenderContextPtr m_renderContext;
};