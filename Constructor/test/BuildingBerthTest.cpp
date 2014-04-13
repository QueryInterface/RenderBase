#include "BuildingBerth.h"
#include <gtest/gtest.h>
#include <memory>

using namespace Constructor;

class BuildingBerthTest : public ::testing::Test
{
public:

    void SetUp()
    {
        m_builder.reset(new BuildingBerth);
    }
    void TearDown()
    {
        m_builder.reset();
    }
protected:
    std::unique_ptr<BuildingBerth> m_builder;
};

TEST_F(BuildingBerthTest, AddSpace)
{
    ASSERT_FALSE(m_builder->SetElement(ET_Space, Vector3D(0,0,0), ED_pY));
}

TEST_F(BuildingBerthTest, AddPrimitive)
{
    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(0,0,0), ED_pY));

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(1,1,1), desc.RBB);
}

TEST_F(BuildingBerthTest, AddPrimitiveInNonZeroPosition)
{
    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(2,5,3), ED_pY));

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(1,1,1), (desc.RBB - desc.LFT));
}

TEST_F(BuildingBerthTest, AddSetOfPrimitives3X3X3)
{
// complex ? structure 3x3x3 created by 3 primitives
//      [0] --- ---
//      --- [2] ---
//      --- --- [0]
// keep in mind Up is on Y axis

    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(0,0,0), ED_pY));
    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(1,2,1), ED_pY));
    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(2,0,2), ED_pY));

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(3,3,3), desc.RBB);
}

TEST_F(BuildingBerthTest, AddBigPrimitive3X3X1)
{
    m_builder->SetElement(ET_CilindricPlatform, Vector3D(1,0,1), ED_pY);

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(3,1,3), (desc.RBB));
}

// eof