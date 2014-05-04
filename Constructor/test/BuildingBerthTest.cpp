#include "BuildingBerth.h"
#include <gtest/gtest.h>
#include <memory>

using namespace ConstructorImpl;

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
    ASSERT_FALSE(m_builder->SetElement(ElementType::Space, Vector3D(0,0,0), Directions::pY));
}

TEST_F(BuildingBerthTest, AddPrimitive)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY));

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(1,1,1), desc.RBB);
}

TEST_F(BuildingBerthTest, AddPrimitiveInNonZeroPosition)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, Vector3D(2,5,3), Directions::pY));

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

    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY));
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, Vector3D(1,2,1), Directions::pY));
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, Vector3D(2,0,2), Directions::pY));

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(3,3,3), desc.RBB);
}

TEST_F(BuildingBerthTest, AddBigPrimitive3X3X1)
{
    m_builder->SetElement(ElementType::CilindricPlatform, Vector3D(1,0,1), Directions::pY);

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(3,1,3), (desc.RBB));
}

TEST_F(BuildingBerthTest, ConstructPillar)
{
    for (size_t i = 0; i < 100; ++i)
    {
        m_builder->SetElement(ElementType::Cube, Vector3D(0,0,i), Directions::pY);
    }

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(1,1,100), (desc.RBB));
}

TEST_F(BuildingBerthTest, ConstructSolidQube)
{
    const size_t cubeScales = 64;
    for (size_t x = 0; x < cubeScales; ++x)
    {
        for (size_t y = 0; y < cubeScales; ++y)
        {
            for (size_t z = 0; z < cubeScales; ++z)
            {
                m_builder->SetElement(ElementType::Cube, Vector3D(x,y,z), Directions::pY);
            }
        }
    }

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(cubeScales, cubeScales, cubeScales), (desc.RBB));
}

TEST_F(BuildingBerthTest, ConstructSpongeSystem)
{
    const size_t cubeScales = 64;
    for (size_t x = 0; x < cubeScales; ++x)
    {
        for (size_t y = 0; y < cubeScales; ++y)
        {
            for (size_t z = 0; z < cubeScales; ++z)
            {
                if (x+y+z % 2)
                    m_builder->SetElement(ElementType::Cube, Vector3D(x,y,z), Directions::pY);
            }
        }
    }

    const ConstructionDescription desc = m_builder->GetCompartment().ConstructionDesc();
    EXPECT_EQ(Vector3D(cubeScales, cubeScales, cubeScales), (desc.RBB));
}

TEST_F(BuildingBerthTest, ElementNeighbourhood)
{
    m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,1,0), Directions::pY);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,2,0), Directions::pY);
    Element element;
    int i = 0;
    m_builder->GetCompartment().IterrateObject(
        [&](size_t, size_t, Utils::RangeList<Element>& item){
            ++i;
            element = *item.get_item_at(1);
    });
    ASSERT_EQ(1, i);
    ASSERT_EQ(Directions::pY | Directions::nY, element.neighbourhood);
}
// eof