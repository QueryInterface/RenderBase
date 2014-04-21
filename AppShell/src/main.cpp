#if 0
#include "Engine.h"
#else
#include "Engine.h"
#endif

int main() {
#if 0
    try 
    {
        IEnginePtr engine = Engine::Create();
        IWindowPtr window = engine->GetWindow();
        IResourceOverseerPtr resourceMgr = engine->GetResourceOverseer();

        window->SetWidth(640);
        window->SetHeight(480);
        window->SetFullscreen(false);

        IScenePtr scene = IScene::Create();
        // Create simple object
        IMeshPtr mesh = resourceMgr->CreateMesh("path");
        ITexturePtr texture = resourceMgr->CreateTexture("path");
        ISimpleObjectPtr simpleObject = resourceMgr->CreateSimpleObject(mesh, texture);
        // Create complex object
        IObjectPartPtr objectPart0 = resourceMgr->CreateObjectPart(/*type*/);
        IObjectPartPtr onjectPart1 = resourceMgr->CreateObjectPart(/*type*/);
        IComplexObjectPtr complexObject = resourceMgr->CreateComplexObject();
        complexObject->AddObjectPart(objectPart0);
        complexObject->AddObjectPart(onjectPart1);
        // Add objects to scene
        ISceneElementHandlePtr simpleObjectHandle = scene->AddSceneElement(simpleObject);
        ISceneElementHandlePtr complexObjectHandle = scene->AddSceneElement(complexObject);
        // Bind objects
        simpleObjectHandle->AttachBidirectional(complexObjectHandle);

        engine->Run();
    }
    catch(std::exception& ex)
    {
        ex;
        return -1;
    }
#else
    try {
        //IRenderContextBuilder* contextBuilder = IRenderContextBuilder::Create();
        //contextBuilder->SetWidth(640);
        //contextBuilder->SetHeight(480);
        //contextBuilder->SetTitle("Sample");
        //contextBuilder->SetApiType(RC_API_TYPE::GLES2);
        //IRenderContext* renderContext = contextBuilder->GetResult();
        //IWindow* window = renderContext->GetWindow();
        //// Render
        //WINDOW_MSG msg = WINDOW_MSG::FOREGROUND;
        //while((msg = window->ProcessMessage()) != WINDOW_MSG::QUIT) {
        //    if (msg == WINDOW_MSG::FOREGROUND)
        //    {
        //    }
        //    renderContext->Present();
        //}
        //renderContext->Release();
        //contextBuilder->Release();
    }
    catch (std::exception& ex) {
        ex;
    }
#endif
    return 0;
}
