#include "SceneElementImpl.h"
#include <gtest/gtest.h>
#include <memory>

    //void        SetPositionImpl(const vector3f_t& pos);
    //void        ShiftImpl(const vector3f_t& shift);

    //void        SetCenterImpl(const vector3f_t& center) ;
    //void        RotateAroundCenterImpl(const vector3f_t& angles);
    //void        RotateAroundCenterAxisImpl(const vector3f_t& axis, float angle);

    //void        RotateAroundPointImpl(const vector3f_t& point, const vector3f_t& angles);
    //void        RotateAroundPointAxisImpl(const vector3f_t& point, const vector3f_t& axis, float angle);

    //void        SetScaleImpl(const vector3f_t& scales);
    //void        ScaleImpl(const vector3f_t& scales);

    //vector3f_t  GetCenterImpl() const;
    //vector3f_t  GetPositionImpl() const;
    //vector3f_t  GetAngleImpl() const;
    //vector3f_t  GetScaleImpl() const;

class SceneElementImplTest 
    : public ::testing::Test
    , public SceneElementImpl
{
public:

    void SetUp()
    {
        zero = vector3f_t(0,0,0);
    }
    void TearDown()
    {
    }
protected:
    vector3f_t zero;
    void calculateAngle();
};

#define EPSILON 0.000001
#define EPSILON_VEC vector3f_t(EPSILON, EPSILON, EPSILON)
#define COMPARE_FLOAT(val0, val1) (val0 > val1 - EPSILON && val0 < val1 + EPSILON)
#define COMPARE_FLOAT_VEC(val0, val1) COMPARE_FLOAT((val0).x, (val1).x) && COMPARE_FLOAT((val0).y, (val1).y) && COMPARE_FLOAT((val0).z, (val1).z)

TEST_F(SceneElementImplTest, PositionTest)
{
    vector3f_t test_pos(1.11f, 3.22f, 5.33f);
    SetPositionImpl(test_pos);
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetPositionImpl(), test_pos));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetCenterImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetAngleImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetScaleImpl(), vector3f_t(1,1,1)));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(m_worldMatrix * m_elementMatrix * vector4f_t(0,0,0,1), vector4f_t(test_pos, 1.0)));
}

TEST_F(SceneElementImplTest, ShiftTest)
{
    vector3f_t test_pos(1.11f, 3.22f, 5.33f);
    vector3f_t test_shift(2.11f, 4.22f, 6.33f);
    SetPositionImpl(test_pos);
    ShiftImpl(test_shift);
    vector3f_t pos = GetPositionImpl();
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetPositionImpl(), test_pos + test_shift));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetCenterImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetAngleImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetScaleImpl(), vector3f_t(1,1,1)));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(m_worldMatrix * m_elementMatrix * vector4f_t(0,0,0,1), vector4f_t(test_pos + test_shift, 1.0)));
}

TEST_F(SceneElementImplTest, CenterRotateTest)
{
    vector3f_t test_pos(1.11f, 3.22f, 5.33f);
    vector3f_t test_center(1, 1, 1);
    SetPositionImpl(test_pos);
    SetCenterImpl(test_center);
}

TEST_F(SceneElementImplTest, ScaleTest)
{
    vector3f_t test_vertex_pos(1.11f, 3.22f, 5.33f);
    vector3f_t test_scale(0.5f, 0.5f, 0.5f);
    vector3f_t test_center(1, 1, 1);
    SetCenterImpl(test_center);
    ScaleImpl(test_scale);
    SetCenterImpl(zero);
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetPositionImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetCenterImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetAngleImpl(), zero));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetScaleImpl(), test_scale));
    vector4f_t input = m_worldMatrix * m_elementMatrix * vector4f_t(test_vertex_pos, 1.0);
    vector3f_t output = (test_vertex_pos - test_center) * test_scale + test_center;
    EXPECT_TRUE(COMPARE_FLOAT_VEC(input, vector4f_t(output, 1.0)));
}

// eof