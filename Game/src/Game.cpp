#include "Utils.h"
#include <chrono>
#include "Game.h"
#include <Constructor.h>

CameraMove::CameraMove()
    : m_moveMask(0)
    , m_previousX(0)
    , m_previousY(0)
    , m_currentX(0)
    , m_currentY(0)
    , m_enableRotate(false)
    , m_moveSpeed(1.0f)
{
}

void CameraMove::SetMoveSpeed(float speed)
{
    m_moveSpeed = speed;
}

void CameraMove::EnableMove(CameraMove::Type type, const vector3f_t& shift)
{
    m_moveMask |= (1 << (uint8_t)type);
    m_shifts[(uint8_t)type] = shift;
}

void CameraMove::DisableMove(CameraMove::Type type)
{
    m_moveMask &= ~(1 << (uint8_t)type);
}

void CameraMove::EnableRotate(uint32_t x, uint32_t y)
{
    m_enableRotate = true;
    m_previousX = x;
    m_previousY = y;
}

void CameraMove::DisableRotate()
{
    m_enableRotate = false;
}


void CameraMove::OnMouseMove(uint32_t x, uint32_t y)
{
    if (m_enableRotate)
    {
        m_currentX = x;
        m_currentY = y;
    }
}

void CameraMove::Process(ICameraPtr& camera, float timeElapsed)
{
    // Process shifts
    for (uint8_t i = 0; i < 6; ++i)
    {
        if (m_moveMask & (1 << i))
        {
            camera->Shift(CoordType::Global, m_shifts[i] * (timeElapsed / 1000) * m_moveSpeed);
        }
    }
    // Process rotates
    if (m_enableRotate)
    {
        //float angle = glm::acos(glm::dot())
        const CameraDesc desc = camera->GetDesc();
        float deltaX = float(m_currentX - m_previousX) / IEngine::Instance().GetWindow().GetWidth();
        float deltaY = float(m_currentY - m_previousY) / IEngine::Instance().GetWindow().GetHeight();
        float fovX = (deltaX * desc.FieldOfViewY / 180) * glm::pi<float>();
        float fovY = ((desc.FieldOfViewY / desc.Aspect) / 180) * glm::pi<float>();
        float angleX = deltaX * fovX;
        float angleY = deltaY * fovY;
        camera->Rotate(CoordType::Local, vector3f_t(angleY, angleX, 0));
        m_previousX = m_currentX;
        m_previousY = m_currentY;
    }
}


Game::Game()
    : m_engine(IEngine::Instance())
    , m_window(m_engine.GetWindow())
    , m_resourceOverseer(IResourceOverseer::Instance())
    , m_camera(nullptr)
    , m_scene(nullptr)
    , m_light(nullptr)
    , m_cameraMoveSpeed(30.0f)
    , m_cameraRotateSpeed(30.0f)
    , m_overmind(Overmind::Get())
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

    //cameraSetup.Eye = vector3f_t(0.0, 0.0, -10.0);
    //cameraSetup.At = vector3f_t(0.0, 0.0, 7.0);
    //cameraSetup.Up = vector3f_t(0.0, 1.0, 0.0);
    cameraSetup.Aspect = 1.0f * m_window.GetWidth() / m_window.GetHeight();
    cameraSetup.FieldOfViewY = 45.0;
    cameraSetup.NearZ = 0.1f;
    cameraSetup.FarZ = 100.0f;
    m_scene = m_engine.CreateScene();
    m_camera = m_engine.CreateCamera(cameraSetup);
    m_light = m_engine.CreateLight(LightType::Spot, vector3f_t(-5, -5, 7));

    m_engine.SetScene(m_scene);

    m_scene->SetCamera(m_camera);
    m_scene->AddLight(m_light);
    m_scene->SetAmbientLight(vector3f_t(0.1, 0.1, 0.1));

    m_cameraMove.SetMoveSpeed(m_cameraMoveSpeed);
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
    ITexturePtr texture1 = m_resourceOverseer.LoadTexture(Utils::Internal::GetMediaFolderPath() + L"Textures/Smile.png");
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
    if (Status::OK != m_overmind.ExecuteScript("Media/Scripts/Scene1.lua"))
    {
        std::string err = m_overmind.GetLastError();
        while(err != "")
        {
            printf("errors: %s\n", err.c_str());
        }
        return;
    }
    Constructor& builder = m_overmind.GetConstructor();
    IMeshPtr mesh = nullptr;
    mesh.reset(&builder.GetMesh());
    IObjectPtr object0 = IObject::CreateObject(mesh, nullptr);
    BBox bbox = builder.GetBoundingBox();
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
    // Move camera
    m_cameraMove.Process(m_camera, elapsedTime);

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
            const CameraDesc desc = m_camera->GetDesc();
            vector3f_t shift = glm::normalize(desc.At - desc.Eye);
            m_cameraMove.EnableMove(CameraMove::Type::Forward, shift);
        } break;
    case EKey::EK_S:
        {
            const CameraDesc desc = m_camera->GetDesc();
            vector3f_t shift = glm::normalize(desc.Eye - desc.At);
            m_cameraMove.EnableMove(CameraMove::Type::Backward, shift);
        } break;
    case EKey::EK_D:
        {
            const CameraDesc desc = m_camera->GetDesc();
            vector3f_t shift = glm::normalize(glm::cross(desc.At - desc.Eye, desc.Up));
            m_cameraMove.EnableMove(CameraMove::Type::Right, shift);
        } break;
    case EKey::EK_A:
        {
            const CameraDesc desc = m_camera->GetDesc();
            vector3f_t shift = glm::normalize(glm::cross(desc.Eye - desc.At, desc.Up));
            m_cameraMove.EnableMove(CameraMove::Type::Left, shift);
        } break;
    case EKey::EK_SPACE:
        {
            const CameraDesc desc = m_camera->GetDesc();
            vector3f_t shift = glm::normalize(desc.Up);
            m_cameraMove.EnableMove(CameraMove::Type::Up, shift);
        } break;
    case EKey::EK_LCTRL:
    case EKey::EK_RCTRL:
        {
            const CameraDesc desc = m_camera->GetDesc();
            vector3f_t shift = glm::normalize(-desc.Up);
            m_cameraMove.EnableMove(CameraMove::Type::Down, shift);
        } break;
    case EKey::EK_Q:
        {
            const CameraDesc desc = m_camera->GetDesc();
            glm::quat q = glm::angleAxis(glm::pi<float>() / 30, desc.Up);
            m_camera->Rotate(CoordType::Local, q);
        } break;
    case EKey::EK_E:
        {
            const CameraDesc desc = m_camera->GetDesc();
            glm::quat q = glm::angleAxis(-glm::pi<float>() / 30, desc.Up);
            m_camera->Rotate(CoordType::Local, q);
        } break;
    }
}

void Game::OnKeyUp(EKey key)
{
    switch (key)
    {
    case EKey::EK_W:
        {
            m_cameraMove.DisableMove(CameraMove::Type::Forward);
        } break;
    case EKey::EK_S:
        {
            m_cameraMove.DisableMove(CameraMove::Type::Backward);
        } break;
    case EKey::EK_D:
        {
            m_cameraMove.DisableMove(CameraMove::Type::Right);
        } break;
    case EKey::EK_A:
        {
            m_cameraMove.DisableMove(CameraMove::Type::Left);
        } break;
    case EKey::EK_SPACE:
        {
            m_cameraMove.DisableMove(CameraMove::Type::Up);
        } break;
    case EKey::EK_LCTRL:
    case EKey::EK_RCTRL:
        {
            m_cameraMove.DisableMove(CameraMove::Type::Down);
        } break;
    case EKey::EK_Q:
        {
            const CameraDesc desc = m_camera->GetDesc();
            glm::quat q = glm::angleAxis(glm::pi<float>() / 30, desc.Up);
            m_camera->Rotate(CoordType::Local, q);
        } break;
    case EKey::EK_E:
        {
            const CameraDesc desc = m_camera->GetDesc();
            glm::quat q = glm::angleAxis(-glm::pi<float>() / 30, desc.Up);
            m_camera->Rotate(CoordType::Local, q);
        } break;
    }
}

void Game::OnMouseDown(EKey key, uint32_t x, uint32_t y)
{
    switch (key)
    {
    case EKey::EK_MOUSE_BUTTON_LEFT:
        {
        } break;
    case EKey::EK_MOUSE_BUTTON_MIDDLE:
        {
        } break;
    case EKey::EK_MOUSE_BUTTON_RIGHT:
        {
            m_cameraMove.EnableRotate(x, y);
        } break;
    }
}

void Game::OnMouseUp(EKey key, uint32_t /*x*/, uint32_t /*y*/)
{
    switch (key)
    {
    case EKey::EK_MOUSE_BUTTON_LEFT:
        {
        } break;
    case EKey::EK_MOUSE_BUTTON_MIDDLE:
        {
        } break;
    case EKey::EK_MOUSE_BUTTON_RIGHT:
        {
            m_cameraMove.DisableRotate();
        } break;
    }
}

void Game::OnMouseMove(uint32_t x, uint32_t y)
{
    m_cameraMove.OnMouseMove(x, y);
}