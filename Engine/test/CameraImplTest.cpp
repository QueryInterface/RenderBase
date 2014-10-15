#include "custom_macros.h"
#include <gtest/gtest.h>
#include "CameraImpl.h"
#include "HandleImpl.h"
#include "EngineStubs.h"
#define _USE_MATH_DEFINES
#include <math.h>

class CameraImplTest 
    : public ::testing::Test
{
public:
    void SetUp()
    {
        m_desc.EyePosition = vector3f_t(0, 0, 10);
        m_desc.Up = y;
        m_desc.Direction = z;
        m_desc.FieldOfViewY = 45;
        m_desc.FarZ = 100;
        m_desc.NearZ = -100;
        m_camera = make_shared_handle<Camera>(m_desc);
    }
    void TearDown()
    {
    }
protected:
    shared_ptr<Camera> m_camera;
    CameraDesc         m_desc;
};

TEST_F(CameraImplTest, BasicTest)
{
    const CameraDesc& desc = m_camera->GetDesc();
    COMPARE_FLOAT_VEC(desc.Direction, glm::normalize(m_desc.Direction));
    COMPARE_FLOAT_VEC(desc.EyePosition, m_desc.EyePosition);
    COMPARE_FLOAT_VEC(desc.Up, m_desc.Up);
    EXPECT_EQ(desc.FieldOfViewY, m_desc.FieldOfViewY);
    EXPECT_EQ(desc.FarZ, m_desc.FarZ);
    EXPECT_EQ(desc.NearZ, m_desc.NearZ);

    COMPARE_FLOAT_VEC(desc.EyePosition, m_camera->GetPosition(CoordType::Global));
}

TEST_F(CameraImplTest, RotateTest)
{
    m_camera->Rotate(CoordType::Local, vector3f_t(0, -M_PI / 2, 0));
    const CameraDesc& desc0 = m_camera->GetDesc();
    COMPARE_FLOAT_VEC(desc0.Direction, x);
    //m_camera->Rotate(CoordType::Local, vector3f_t(0, 0, -M_PI / 2));
    //const CameraDesc& desc1 = m_camera->GetDesc();
    //COMPARE_FLOAT_VEC(desc1.Direction, y);
    //m_camera->Rotate(CoordType::Local, vector3f_t(-M_PI / 2, 0, 0));
    //const CameraDesc& desc2 = m_camera->GetDesc();
    //COMPARE_FLOAT_VEC(desc2.Direction, z);
    
}

TEST_F(CameraImplTest, DISABLED_MoveTest)
{
    m_camera->Rotate(CoordType::Local, vector3f_t(0, -M_PI / 2, 0));
    const CameraDesc& desc = m_camera->GetDesc();

    COMPARE_FLOAT_VEC(desc.Direction, -x);
    COMPARE_FLOAT_VEC(desc.EyePosition, m_desc.EyePosition);
    COMPARE_FLOAT_VEC(desc.Up, m_desc.Up);
    EXPECT_EQ(desc.FieldOfViewY, m_desc.FieldOfViewY);
    EXPECT_EQ(desc.FarZ, m_desc.FarZ);
    EXPECT_EQ(desc.NearZ, m_desc.NearZ);

    COMPARE_FLOAT_VEC(desc.EyePosition, m_camera->GetPosition(CoordType::Global));
}

TEST_F(CameraImplTest, ViewMatrixTest)
{
    vector4f_t objectX = vector4f_t(10.0f * x, 1);
    vector4f_t objectY = vector4f_t(10.0f * y, 1);
    vector4f_t objectZ = vector4f_t(10.0f * z, 1);
    vector4f_t objectXInCameraSpace = m_camera->GetViewMatrix() * objectX;
    vector4f_t objectYInCameraSpace = m_camera->GetViewMatrix() * objectY;
    vector4f_t objectZInCameraSpace = m_camera->GetViewMatrix() * objectZ;
    COMPARE_FLOAT_VEC(objectXInCameraSpace, vector4f_t(-10, 0, 10, 1.0));
    COMPARE_FLOAT_VEC(objectYInCameraSpace, vector4f_t(0, 10, 10, 1.0));
    COMPARE_FLOAT_VEC(objectZInCameraSpace, vector4f_t(0, 0, 0, 1.0));
    m_camera->Rotate(CoordType::Local, vector3f_t(0, -M_PI / 2, 0));
}