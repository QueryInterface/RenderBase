#include "Engine.h"
#include "ResourceOverseer.h"
#include "Utils.h"
#include <chrono>
#include <Constructor.h>

class Game : public IEngineCallbacks
{
public:
    Game();
    ~Game();
    void InitScene0();
    void InitScene1();
    void Start();
    // IEngineCallbacks
    void OnSceneUpdate();
private:
    void centerObject(IObjectPtr& obj);

    PREVENT_COPY(Game);

    IEngine*                m_engine;
    IWindow*                m_window;
    IResourceOverseer*      m_resourceOverseer;
    IScenePtr               m_scene;
    ICameraPtr              m_camera;
    ILightPtr               m_light;
    std::vector<IObjectPtr> m_objects;

    // singletone iternal life. no need to have ptr
    Constructor&            m_builder;
};

Game::Game()
    : m_engine(IEngine::Instance())
    , m_window(m_engine->GetWindow())
    , m_resourceOverseer(IResourceOverseer::Instance())
    , m_camera(nullptr)
    , m_scene(nullptr)
    , m_light(nullptr)
    , m_builder(Constructor::GetConstructor())
{
    // Setup window
    m_window->SetWidth(640);
    m_window->SetHeight(480);
    m_window->SetFullscreen(false);

    // Create camera
    CameraSetup cameraSetup;
    cameraSetup.Eye = vector3f_t(0.0, 0.0, 0.0);
    cameraSetup.At = vector3f_t(0.0, 0.0, 1.0);
    cameraSetup.Up = vector3f_t(0.0, -1.0, 0.0);
    cameraSetup.FieldOfViewY = 45.0;
    cameraSetup.NearZ = 0.1f;
    cameraSetup.FarZ = 20.0f;
    m_scene = m_engine->CreateScene();
    m_camera = m_engine->CreateCamera(cameraSetup);
    m_light = m_engine->CreateLight(LightType::Spot, vector3f_t(-100, -100, 7));

    m_engine->SetScene(m_scene);

    m_scene->SetCamera(m_camera);
    m_scene->AddLight(m_light);
}

Game::~Game()
{
}

void Game::InitScene0()
{
    try
    {
        // Load resources
        IMeshPtr mesh = m_resourceOverseer->LoadMesh(Utils::Internal::GetMediaFolderPath() + L"Meshes/sphere/sphere.obj");
        ITexturePtr texture0 = m_resourceOverseer->LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.png");
        ITexturePtr texture1 = m_resourceOverseer->LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.obj");
        // // Create objects
        IObjectPtr object0 = IObject::CreateObject(mesh, texture0);
        object0->SetPosition(CoordType::World, vector3f_t(-1, -1, 7));
        IObjectPtr object1 = IObject::CreateObject(mesh, texture1);
        object1->SetPosition(CoordType::World, vector3f_t(1, 1, 7));
        m_objects.push_back(object0);
        m_objects.push_back(object1);
    }
    catch (std::exception& ex) 
    {
        ex;
    }
}

void Game::InitScene1()
{
    m_builder.SetElement(ElementType::Cilinder, vector3i_t(0,0,0), Directions::nX, true);

    try
    {
        IMeshPtr mesh = nullptr;
        mesh.reset(&m_builder.GetMesh());
        IObjectPtr object0 = IObject::CreateObject(mesh, nullptr);
        BBox bbox = m_builder.GetBoundingBox();
        vector3i_t center = (bbox.RBB + bbox.LFT) / 2;
        object0->SetPosition(CoordType::Local, vector3f_t(-center.x, -center.y, -center.z));
        object0->SetPosition(CoordType::World, vector3f_t(0, 0, 7));
        object0->SetScale(CoordType::Local, vector3f_t(0.5, 0.5, 0.5));
        m_objects.push_back(object0);
    }
    catch (std::exception& ex) 
    {
        ex;
    }
}

void Game::Start()
{
    try 
    {
        for (IObjectPtr& object : m_objects)
        {
            m_scene->AddObject(object);
        }
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
	float angle = elapsedTime / 50000.0f * 45;
    start = end;
    vector3f_t pos;
    for (IObjectPtr& object : m_objects)
    {
        pos = object->GetPosition(CoordType::World);
        object->Shift(CoordType::World, vector3f_t(0, 0, -7));
        glm::quat objectQ = glm::angleAxis(angle, vector3f_t(-1, 0, -1));        
        objectQ = glm::normalize(objectQ);
        object->Rotate(CoordType::World, objectQ);
        object->Shift(CoordType::World, vector3f_t(0, 0, 7));
        pos = object->GetPosition(CoordType::World);
    }
    m_light->Rotate(CoordType::World, vector3f_t(0, 0, angle / 2));
}


int main() 
{
    Game game;
    //game.InitScene0();
    game.InitScene1();
    game.Start();
}
