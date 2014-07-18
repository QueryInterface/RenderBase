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
};

#define EPSILON 0.000001
#define EPSILON_VEC vector3f_t(EPSILON, EPSILON, EPSILON)
#define COMPARE_FLOAT(val0, val1) (val0 > val1 - EPSILON && val0 < val1 + EPSILON)
#define COMPARE_FLOAT_VEC(val0, val1) COMPARE_FLOAT(val0.x, val1.x) && COMPARE_FLOAT(val0.y, val1.y) && COMPARE_FLOAT(val0.z, val1.z)

TEST_F(SceneElementImplTest, PositionTest)
{
    vector3f_t test_pos(1.11f, 3.22f, 5.33f);
    SetPositionImpl(test_pos);
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetPositionImpl(), test_pos));
    EXPECT_TRUE(COMPARE_FLOAT_VEC(GetCenterImpl(), zero));
}

TEST_F(SceneElementImplTest, ShiftTest)
{
    float x = 1.11f;
    float y = 3.22f;
    float z = 5.33f;
    SetPositionImpl(vector3f_t(x, y, z));
    ShiftImpl(vector3f_t(y, z, x));
    vector3f_t pos = GetPositionImpl();
    EXPECT_TRUE(COMPARE_FLOAT(pos.x, x+y));
    EXPECT_TRUE(COMPARE_FLOAT(pos.y, y+z));
    EXPECT_TRUE(COMPARE_FLOAT(pos.z, z+x));
}


// eof