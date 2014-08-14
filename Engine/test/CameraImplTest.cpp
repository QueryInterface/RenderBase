#include "custom_macros.h"
#include <gtest/gtest.h>
#include "CameraImpl.h"
#include "HandleImpl.h"
#include "EngineStubs.h"

class CameraImplTest 
    : public ::testing::Test
{
public:
    void SetUp()
    {
        m_desc.Eye = vector3f_t(0, 0, -10);
        m_desc.Up = y;
        m_desc.At = zero;
        m_desc.FieldOfViewY = 45;
        m_desc.FarZ = 100;
        m_desc.NearZ = -100;
        m_camera = make_shared_handle<Camera>(m_desc);
    }
    void TearDown()
    {
    }
protected:
    void check(const vector3f_t& lPos,      const vector3f_t& wPos, 
               const vector3f_t& lScale,    const vector3f_t& wSclale,
               const vector3f_t& lInitDir,  const vector3f_t& wInitDir,
               const vector3f_t& lDir,      const vector3f_t& wDir);

    shared_ptr<Camera> m_camera;
    CameraDesc         m_desc;
};

TEST_F(CameraImplTest, BasicTest)
{
    const CameraDesc& desc = m_camera->GetDesc();
    EXPECT_EQ(desc.At, m_desc.At);
    EXPECT_EQ(desc.Eye, m_desc.Eye);
    EXPECT_EQ(desc.Up, m_desc.Up);
    EXPECT_EQ(desc.FieldOfViewY, m_desc.FieldOfViewY);
    EXPECT_EQ(desc.FarZ, m_desc.FarZ);
    EXPECT_EQ(desc.NearZ, m_desc.NearZ);

    EXPECT_EQ(desc.Eye, m_camera->GetPosition(CoordType::Global));
}