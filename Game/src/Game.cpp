#include "Utils.h"
#include <chrono>
#include "Game.h"

Game::Game()
    : m_engine(IEngine::Instance())
    , m_window(m_engine.GetWindow())
    , m_resourceOverseer(IResourceOverseer::Instance())
    , m_camera(nullptr)
    , m_scene(nullptr)
    , m_light(nullptr)
    , m_builder(Constructor::GetConstructor())
{
    // Setup window
    m_window.SetWidth(640);
    m_window.SetHeight(480);
    m_window.SetFullscreen(false);
    // Subscribe on user input
    m_window.RegisterInputCallbacks(this);
    // Create camera
    CameraDesc cameraSetup;
    cameraSetup.Eye = vector3f_t(8.0, 5.0, -8.0);
    cameraSetup.At = vector3f_t(0.0, 0.0, 7.0);
    cameraSetup.Up = vector3f_t(0.0, 1.0, 0.0);
    cameraSetup.FieldOfViewY = 45.0;
    cameraSetup.NearZ = 0.1f;
    cameraSetup.FarZ = 100.0f;
    m_scene = m_engine.CreateScene();
    m_camera = m_engine.CreateCamera(cameraSetup);
    m_light = m_engine.CreateLight(LightType::Spot, vector3f_t(-5, -5, 7));

    m_engine.SetScene(m_scene);

    m_scene->SetCamera(m_camera);
    m_scene->AddLight(m_light);
}

Game::~Game()
{
    m_window.UnregisterInputCallbacks(this);
}

void Game::InitHelpers()
{
    IMeshPtr mesh = m_resourceOverseer.LoadMesh(Utils::Internal::GetMediaFolderPath() + L"Meshes/sphere/sphere.obj");
    ITexturePtr texture0 = m_resourceOverseer.LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.png");
    // // Create objects
    m_lightShape = IObject::CreateObject(mesh, texture0);
    m_lightShape->SetPosition(CoordType::Global, vector3f_t(-5, -5, 7));
    m_lightShape->SetScale(CoordType::Local, vector3f_t(0.2f, 0.2f, 0.2f));
    m_scene->AddObject(m_lightShape);
}

void Game::InitScene0()
{
    // Load resources
    IMeshPtr mesh = m_resourceOverseer.LoadMesh(Utils::Internal::GetMediaFolderPath() + L"Meshes/sphere/sphere.obj");
    ITexturePtr texture0 = m_resourceOverseer.LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.png");
    ITexturePtr texture1 = m_resourceOverseer.LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.obj");
    // // Create objects
    IObjectPtr object0 = IObject::CreateObject(mesh, texture0);
    object0->SetPosition(CoordType::Global, vector3f_t(-3, -3, 7));
    IObjectPtr object1 = IObject::CreateObject(mesh, texture1);
    object1->SetPosition(CoordType::Global, vector3f_t(3, 3, 7));
    m_objects.push_back(object0);
    m_objects.push_back(object1);
}

void Game::InitScene1()
{
    const size_t cubeScales = 7;
    for (size_t i = 0; i < cubeScales; ++i)
    {
        m_builder.SetElement(i%2 ? ElementType::Cilinder : ElementType::Cube, vector3i_t(16,i,16), Directions::pZ);
    }

    IMeshPtr mesh = nullptr;
    mesh.reset(&m_builder.GetMesh());
    IObjectPtr object0 = IObject::CreateObject(mesh, nullptr);
    BBox bbox = m_builder.GetBoundingBox();
    vector3i_t center = (bbox.RBB + bbox.LFT) / 2;
    object0->SetPosition(CoordType::Local, vector3f_t(-center.x, -center.y, -center.z));
    object0->SetPosition(CoordType::Global, vector3f_t(0, 0, 7));
    object0->SetScale(CoordType::Local, vector3f_t(0.5, 0.5, 0.5));
    m_objects.push_back(object0);
}

void Game::Start()
{

    for (IObjectPtr& object : m_objects)
    {
        m_scene->AddObject(object);
    }
    m_engine.Run(this);
}

void Game::OnSceneUpdate()
{
    static auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    float elapsedTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    float angle = elapsedTime / 50000.0f * 45;
    start = end;
    vector3f_t pos;
    /*    for (IObjectPtr& object : m_objects)
    {
    pos = object->GetPosition(CoordType::Global);
    object->Shift(CoordType::Global, vector3f_t(0, 0, -7));
    glm::quat objectQ = glm::angleAxis(angle, vector3f_t(-1, 0, -1));        
    objectQ = glm::normalize(objectQ);
    object->Rotate(CoordType::Global, objectQ);
    object->Shift(CoordType::Global, vector3f_t(0, 0, 7));
    pos = object->GetPosition(CoordType::Global);
    }*/
    m_lightShape->Rotate(CoordType::Global, vector3f_t(0, 0, angle / 2));
    m_light->Rotate(CoordType::Global, vector3f_t(0, 0, angle / 2));
}

void Game::OnKeyDown(EKey key) 
{
    switch (key)
    {
    case EKey::EK_W:
        {
            //m_camera->Shift(CoordType::Global, 
        } break;
    }
}

void Game::OnKeyUp(EKey key)
{
    key;
}