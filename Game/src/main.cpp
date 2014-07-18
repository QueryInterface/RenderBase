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
    cameraSetup.Up = vector3f_t(0.0, 1.0, 0.0);
    cameraSetup.FieldOfViewY = 45.0;
    cameraSetup.NearZ = 0.1f;
    cameraSetup.FarZ = 20.0f;
    m_camera = m_engine->CreateCamera(cameraSetup);
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
        //object0->SetCenter(vector3f_t(1, 1, 1));
        object0->SetPosition(vector3f_t(-1, -1, 7));
        m_objects.push_back(object0);
        IObjectPtr object1 = IObject::CreateObject(mesh, texture1);
        //object1->SetCenter(vector3f_t(1, 1, 1));
        object1->SetPosition(vector3f_t(1, 1, 7));
        m_objects.push_back(object1);
        // Create light
        m_light = m_engine->CreateLight(LightType::Spot, vector3f_t(0, 0, 0));
        // CreateScene
        IScenePtr scene = m_engine->CreateScene();

        scene->AddObject(object0);
        scene->AddObject(object1);
        scene->SetCamera(m_camera);
        scene->AddLight(m_light);
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

void Game::InitScene1()
{
    size_t size = 3;
    size_t offset = 2;
    for (size_t i = 0; i < size; ++i)
    {
        m_builder.SetElement(ElementType::Wedge, vector3i_t(offset,0,offset+i), Directions::nX, true);
        m_builder.SetElement(ElementType::Wedge, vector3i_t(offset+size-1,0,offset+i), Directions::pX, true);
    }

    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder.SetElement(ElementType::Wedge, vector3i_t(offset+i,0,offset+0), Directions::nZ, true);
        m_builder.SetElement(ElementType::Wedge, vector3i_t(offset+i,0,offset+size-1), Directions::pZ, true);
    }

    size = 7;

    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder.SetElement(ElementType::Wedge, vector3i_t(i,0,0), Directions::nZ, true);
        m_builder.SetElement(ElementType::Wedge, vector3i_t(i,0,size-1), Directions::pZ, true);
    }

    for (size_t i = 0; i < size; ++i)
    {
        m_builder.SetElement(ElementType::Wedge, vector3i_t(0,0,i), Directions::nX, true);
        m_builder.SetElement(ElementType::Wedge, vector3i_t(size-1,0,i), Directions::pX, true);
    }

    offset = 12;
    m_builder.SetElement(ElementType::Wedge, vector3i_t(offset,0,1), Directions::nX, true);
    m_builder.SetElement(ElementType::Wedge, vector3i_t(offset+1,0,0), Directions::pX, true);
    m_builder.SetElement(ElementType::Wedge, vector3i_t(offset+1,0,1), Directions::pZ, true);
    m_builder.SetElement(ElementType::Wedge, vector3i_t(offset,0,0), Directions::nZ, true);

    try
    {
        IMeshPtr mesh = nullptr;
        mesh.reset(&m_builder.GetMesh());
        IObjectPtr object0 = IObject::CreateObject(mesh, nullptr);
        BBox bbox = m_builder.GetBoundingBox();
        vector3i_t center = (bbox.RBB + bbox.LFT) / 2;
        object0->SetCenter(vector3f_t(center.x, center.y, center.z));
        object0->SetPosition(vector3f_t(0, 0, 10));
        object0->Scale(vector3f_t(0.5, 0.5, 0.5));
        m_objects.push_back(object0);

        IScenePtr scene = m_engine->CreateScene();
        scene->AddObject(object0);
        scene->SetCamera(m_camera);

        m_engine->SetScene(scene);
        m_engine->Run(this);
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
        m_engine->Run(this);
    }
    catch (std::exception& ex) 
    {
        ex;
    }
}

void Game::centerObject(IObjectPtr& obj)
{
    // Center object
    auto& meshDesc = obj->GetMesh()->GetDesc();
    float xMin = 10000;
    float yMin = 10000;
    float zMin = 10000;
    float xMax = -10000;
    float yMax = -10000;
    float zMax = -10000;
    for (auto& shape : meshDesc.Shapes)
    {
        for (uint32_t i = 0; i < shape.Positions.Data.size(); ++i)
        {
            float v = shape.Positions.Data[i];
            if (0 == i % 3 && v < xMin) xMin = v;
            if (0 == i % 3 && v > xMax) xMax = v;
            if (1 == i % 3 && v < yMin) yMin = v;
            if (1 == i % 3 && v > yMax) yMax = v;
            if (2 == i % 3 && v < zMin) zMin = v;
            if (2 == i % 3 && v > zMax) zMax = v;
        }
    }
    float xCenter = (xMax - xMin) / 2;
    float yCenter = (yMax - yMin) / 2;
    float zCenter = (zMax - zMin) / 2;

    obj->SetCenter(vector3f_t(xCenter, yCenter, zCenter));
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
        //object->RotateAroundCenter(vector3f_t(angle, angle/2, angle));
        object->RotateAroundCenter(vector3f_t(angle, angle, 0));
    }
    m_light->RotateAroundPoint(vector3f_t(0, 0, 7), vector3f_t(0, 0, angle));
    //m_camera->RotateAroundPoint(vector3f_t(0, 0, 7), vector3f_t(0.2, 0, 0));
}


int main() 
{
    Game game;
    game.InitScene0();
    //game.InitScene1();
    game.Start();
}
