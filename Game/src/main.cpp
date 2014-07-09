#include "Engine.h"
#include "ResourceOverseer.h"
#include "Object.h"
#include "Utils.h"

int main() {
    try 
    {
        IResourceOverseer* resourceOverseer = IResourceOverseer::Instance();
        IEngine* engine = IEngine::Instance();
        IWindow* window = engine->GetWindow();

        // Setup window
        window->SetWidth(640);
        window->SetHeight(480);
        window->SetFullscreen(false);
        // Load resources
        IMeshPtr mesh = resourceOverseer->LoadMesh(Utils::Internal::GetMediaFolderPath() + L"Meshes/cube.obj");
        ITexturePtr texture0 = resourceOverseer->LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.png");
        ITexturePtr texture1 = resourceOverseer->LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.obj");
        // // Create objects
        IObjectPtr object0 = IObject::CreateObject(mesh, texture0);
        object0->SetPosition(0, 0, 0);
        IObjectPtr object1 = IObject::CreateObject(mesh, texture1);
        object1->SetPosition(1, 1, 0);
        // Create light
        ILightPtr light = engine->CreateLight();
        // CreateScene
        IScenePtr scene = engine->CreateScene();
        // Create camera
        ICameraPtr camera = engine->CreateCamera(vector3f_t(0.5, 0.5, 0.0), vector3f_t(0.0, 0.0, 1.0), vector3f_t(0., -1.0, 0.0));
        // Attach objects to scene
        scene->AddObject(object0);
        //scene->AddObject(object1);
        scene->SetCamera(camera);
        scene->AddLight(light);
        // Set scene
        engine->SetScene(scene);
        // Run
        engine->Run();

    }
    catch (std::exception& ex) 
    {
        ex;
    }
    return 0;
}
