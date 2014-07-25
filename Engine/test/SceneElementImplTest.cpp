#include "SceneElementImpl.h"
#include <gtest/gtest.h>
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>

#define EPSILON 0.000001
#define EPSILON_VEC vector3f_t(EPSILON, EPSILON, EPSILON)
#define COMPARE_FLOAT(val0, val1) (val0 > val1 - EPSILON && val0 < val1 + EPSILON)
//#define COMPARE_FLOAT_VEC(val0, val1) COMPARE_FLOAT((val0).x, (val1).x) && COMPARE_FLOAT((val0).y, (val1).y) && COMPARE_FLOAT((val0).z, (val1).z)
#define COMPARE_FLOAT_VEC(val0, val1) EXPECT_NEAR((val0).x, (val1).x, EPSILON); EXPECT_NEAR((val0).y, (val1).y, EPSILON); EXPECT_NEAR((val0).z, (val1).z, EPSILON)

class SceneElementImplTest 
    : public ::testing::Test
    , public SceneElementImpl
{
public:

    void SetUp()
    {
        zero = vector3f_t(0,0,0);
        one = vector3f_t(1,1,1);
        x = vector3f_t(1, 0, 0);
        y = vector3f_t(0, 1, 0);
        z = vector3f_t(0, 0, 1);
    }
    void TearDown()
    {
    }
protected:
    void check(const vector3f_t& lPos,      const vector3f_t& wPos, 
               const vector3f_t& lScale,    const vector3f_t& wSclale,
               const vector3f_t& lInitDir,  const vector3f_t& wInitDir,
               const vector3f_t& lDir,      const vector3f_t& wDir);

    vector3f_t zero;
    vector3f_t one;
    vector3f_t x;
    vector3f_t y;
    vector3f_t z;
};

void SceneElementImplTest::check(const vector3f_t& lPos,      const vector3f_t& wPos, 
                                 const vector3f_t& lScale,    const vector3f_t& wScale,
                                 const vector3f_t& lInitDir,  const vector3f_t& wInitDir,
                                 const vector3f_t& lDir,      const vector3f_t& wDir)
{
    COMPARE_FLOAT_VEC(lPos, GetPositionImpl(CoordType::Local));
    COMPARE_FLOAT_VEC(wPos, GetPositionImpl(CoordType::World));
    COMPARE_FLOAT_VEC(lScale, GetScaleImpl(CoordType::Local));
    COMPARE_FLOAT_VEC(wScale, GetScaleImpl(CoordType::World));
    COMPARE_FLOAT_VEC(lDir, GetDirectionImpl(CoordType::Local, lInitDir));
    COMPARE_FLOAT_VEC(wDir, GetDirectionImpl(CoordType::World, wInitDir));
}

TEST_F(SceneElementImplTest, PositionTest)
{
    vector3f_t test_pos(1.11f, 3.22f, 5.33f);
    vector3f_t test_shift(2.11f, 4.22f, 6.33f);
    SetPositionImpl(CoordType::Local, test_pos);
    {
        check(test_pos, zero, one, one, z, z, z, z);
    }
    SetPositionImpl(CoordType::World, test_pos);
    {
        check(test_pos, test_pos, one, one, z, z, z, z);
    }
    ShiftImpl(CoordType::Local, -test_shift);
    {
        check(test_pos - test_shift, test_pos, one, one, z, z, z, z);
    }
    ShiftImpl(CoordType::World, test_shift);
    {
        check(test_pos - test_shift, test_pos + test_shift, one, one, z, z, z, z);
    }
}

TEST_F(SceneElementImplTest, RotateTest)
{
    vector3f_t test_angles(M_PI, M_PI/2, 2 * M_PI);
    SetPositionImpl(CoordType::Local, x);
    {
        check(x, zero, one, one, z, z, z, z);
    }
    RotateImpl(CoordType::Local, test_angles);
    {
        check(-z, zero, one, one, z, z, -x, z);
    }
    SetPositionImpl(CoordType::World, x);
    {
        check(-z, x, one, one, z, z, -x, z);
    }
    RotateImpl(CoordType::World, test_angles);
    {
        check(-z, -z, one, one, z, z, -x, -x);
    }
}

TEST_F(SceneElementImplTest, QRotateTest)
{
    vector3f_t test_angles(M_PI, M_PI/2, 2 * M_PI);
    quat q(test_angles);
    SetPositionImpl(CoordType::Local, x);
    {
        check(x, zero, one, one, z, z, z, z);
    }
    RotateImpl(CoordType::Local, q);
    {
        check(-z, zero, one, one, z, z, -x, z);
    }
    SetPositionImpl(CoordType::World, x);
    {
        check(-z, x, one, one, z, z, -x, z);
    }
    RotateImpl(CoordType::World, q);
    {
        check(-z, -z, one, one, z, z, -x, -x);
    }
    // Inverse rotation
    RotateImpl(CoordType::Local, -q);
    {
        check(x, -z, one, one, z, z, z, -x);
    }
    RotateImpl(CoordType::World, -q);
    {
        check(x, x, one, one, z, z, z, z);
    }
}
