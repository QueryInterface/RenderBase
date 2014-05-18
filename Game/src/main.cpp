#include "Engine.h"
#include "ResourceOverseer.h"

int main() {
    try 
    {
        IResourceOverseer* resourceOverseer = IResourceOverseer::Instance();
        IEngine* engine = IEngine::Instance();
        IWindow* window = engine->GetWindow();

        resourceOverseer;

        // Setup window
        window->SetWidth(640);
        window->SetHeight(480);
        window->SetFullscreen(false);
        // Load resources
        // IMeshPtr mesh = resourceOverseer->LoadMesh("path");
        // ITexturePtr texture0 = resourceOverseer->LoadTexture("path");
        // ITexturePtr texture1 = resourceOverseer->LoadTexture("path");
        // // Create objects
        // IObjectPtr object0 = IObject::CreateBasicObject(mesh, texture);
        // object0->SetPosition(0, 0, 0);
        // IObjectPtr object1 = IObject::CreaterBaseicObject(mesh, texture);
        // object1->SetPosition(1, 1, 0);
        // // Create light
        // ILightPtr light = engine->CreateLight();
        // // CreateScene
        // IScenePtr scene = engine->CreateScene();
        // // Create camera
        // ICameraPtr camera = engine->CreateCamera();
        // camera->MakeActive();
        // // Attach objects to scene
        // scene->AddObject(object0);
        // scene->AddObject(object1);
        // scene->SetCamera(camera);
        // scene->AddLight(light);
        // // Set scene
        // engine->SetScene(scene);
        // // Run
        engine->Run();

    }
    catch (std::exception& ex) 
    {
        ex;
    }
    return 0;
}
