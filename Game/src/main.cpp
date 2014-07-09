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
        object0->SetPosition(-1, -1, 7);
        IObjectPtr object1 = IObject::CreateObject(mesh, texture1);
        object1->SetPosition(1, 0, 6);
        // Create light
        ILightPtr light = engine->CreateLight();
        // CreateScene
        IScenePtr scene = engine->CreateScene();
        // Create camera

        CameraSetup cameraSetup;
        cameraSetup.Eye = vector3f_t(0.0, 0.0, 0.0);
        cameraSetup.At = vector3f_t(0.0, 0.0, 1.0);
        cameraSetup.Up = vector3f_t(0.0, 1.0, 0.0);
        cameraSetup.FieldOfViewY = 45.0;
        cameraSetup.NearZ = 0.1f;
        cameraSetup.FarZ = 10.0f;
        ICameraPtr camera = engine->CreateCamera(cameraSetup);
        // Attach objects to scene
        scene->AddObject(object0);
        //scene->AddObject(object1);
        scene->SetCamera(camera);
        scene->AddLight(light);
        // Set scene
        engine->SetScene(scene);
        // Run
        engine->Run();

 //   static auto start = std::chrono::high_resolution_clock::now();
 //   auto end = std::chrono::high_resolution_clock::now();
 //   float elapsedTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	//float angle = elapsedTime / 1000.0f * 45;
	//modelMatrix *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0, 0.0, 0.0)) *
	//				glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 1.0, 0.0)) *
	//				glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));

    }
    catch (std::exception& ex) 
    {
        ex;
    }
    return 0;
}
