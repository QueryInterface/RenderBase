#include "Engine.h"
#include "ResourceOverseer.h"
#include "Utils.h"
#include <chrono>

class Game : public IEngineCallbacks
{
public:
    Game();
    ~Game();
    void Start();
    // IEngineCallbacks
    void OnSceneUpdate();
private:
    IEngine*                m_engine;
    IWindow*                m_window;
    IResourceOverseer*      m_resourceOverseer;
    ICameraPtr              m_camera;
    std::vector<IObjectPtr> m_objects;
};

Game::Game()
    : m_engine(IEngine::Instance())
    , m_window(m_engine->GetWindow())
    , m_resourceOverseer(IResourceOverseer::Instance())
    , m_camera(nullptr)
{
}

Game::~Game()
{
}

void Game::Start()
{
    try
    {
        // Setup window
        m_window->SetWidth(640);
        m_window->SetHeight(480);
        m_window->SetFullscreen(false);
        // Load resources
        IMeshPtr mesh = m_resourceOverseer->LoadMesh(Utils::Internal::GetMediaFolderPath() + L"Meshes/cube.obj");
        ITexturePtr texture0 = m_resourceOverseer->LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.png");
        ITexturePtr texture1 = m_resourceOverseer->LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.obj");
        // // Create objects
        IObjectPtr object0 = IObject::CreateObject(mesh, texture0);
        object0->SetCenter(vector3f_t(1, 1, 1));
        object0->SetPosition(vector3f_t(-1, -1, 7));
        m_objects.push_back(object0);
        IObjectPtr object1 = IObject::CreateObject(mesh, texture1);
        object1->SetCenter(vector3f_t(1, 1, 1));
        object1->SetPosition(vector3f_t(1, 1, 7));
        m_objects.push_back(object1);
        // Create light
        ILightPtr light = m_engine->CreateLight(LightType::Spot, vector3f_t(-3, -3, 3));
        // CreateScene
        IScenePtr scene = m_engine->CreateScene();
        // Create camera
        CameraSetup cameraSetup;
        cameraSetup.Eye = vector3f_t(0.0, 0.0, 0.0);
        cameraSetup.At = vector3f_t(0.0, 0.0, 1.0);
        cameraSetup.Up = vector3f_t(0.0, 1.0, 0.0);
        cameraSetup.FieldOfViewY = 45.0;
        cameraSetup.NearZ = 0.1f;
        cameraSetup.FarZ = 10.0f;
        m_camera = m_engine->CreateCamera(cameraSetup);
        // Attach objects to scene
        scene->AddObject(object0);
        scene->AddObject(object1);
        scene->SetCamera(m_camera);
        scene->AddLight(light);
        // Set scene
        m_engine->SetScene(scene);
        // Run
        m_engine->Run(this);
    }
    catch (std::exception& ex) 
    {
        ex;
    }
}

void Game::OnSceneUpdate()
{
    static auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    float elapsedTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	float angle = elapsedTime / 1000.0f * 45;
    start = end;
    for (IObjectPtr& object : m_objects)
    {
        object->RotateAroundCenter(vector3f_t(angle, angle/2, angle));
    }
    m_camera->RotateAroundPoint(vector3f_t(0, 0, 7), vector3f_t(0.2, 0, 0));
}


int main() 
{
    Game game;
    game.Start();
}
