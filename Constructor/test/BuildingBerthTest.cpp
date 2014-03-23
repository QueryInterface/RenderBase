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

TEST_F(BuildingBerthTest, EmptyBuildingBerth)
{
    ASSERT_EQ(0, m_builder->GetCompartmentsCount());
}

TEST_F(BuildingBerthTest, AddSpace)
{
    ASSERT_FALSE(m_builder->SetElement(ET_Space, Vector3D(0,0,0), ED_pY));
    ASSERT_EQ(0, m_builder->GetCompartmentsCount());
}

TEST_F(BuildingBerthTest, AddPrimitive)
{
    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(0,0,0), ED_pY));
    ASSERT_EQ(1, m_builder->GetCompartmentsCount());

    const ElementDescription desc = m_builder->GetCompartment(0).GetObjectDescription();
    ASSERT_EQ(1, desc.Dimentions.x);
    ASSERT_EQ(1, desc.Dimentions.y);
    ASSERT_EQ(1, desc.Dimentions.z);
}

TEST_F(BuildingBerthTest, AddPrimitive3X3X3)
{
// complex ? structure 3x3x3 created by 3 primitives
//      [0] --- ---
//      --- [2] ---
//      --- --- [0]
// keep in mind Up is on Y axis

    ASSERT_TRUE(m_builder->SetElement(ET_Cube, Vector3D(0,0,0), ED_pY));
    ASSERT_FALSE(m_builder->SetElement(ET_Cube, Vector3D(1,2,1), ED_pY));
    ASSERT_FALSE(m_builder->SetElement(ET_Cube, Vector3D(2,0,2), ED_pY));
    ASSERT_EQ(1, m_builder->GetCompartmentsCount());

    const ElementDescription desc = m_builder->GetCompartment(0).GetObjectDescription();
    ASSERT_EQ(3, desc.Dimentions.x);
    ASSERT_EQ(3, desc.Dimentions.y);
    ASSERT_EQ(3, desc.Dimentions.z);
}

// eof