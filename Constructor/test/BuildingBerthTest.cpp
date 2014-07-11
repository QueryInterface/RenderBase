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
    ASSERT_FALSE(m_builder->SetElement(ElementType::Space, vector3i_t(0,0,0), Directions::pZ));
}

TEST_F(BuildingBerthTest, AddPrimitive)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ));

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(1,1,1), desc.RBB);
}

TEST_F(BuildingBerthTest, AddPrimitiveInNonZeroPosition)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(2,5,3), Directions::pZ));

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(1,1,1), (desc.RBB - desc.LFT));
}

TEST_F(BuildingBerthTest, AddSetOfPrimitives3X3X3)
{
// complex ? structure 3x3x3 created by 3 primitives
//      [0] --- ---
//      --- [2] ---
//      --- --- [0]
// keep in mind Up is on Y axis

    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ));
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(1,2,1), Directions::pZ));
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(2,0,2), Directions::pZ));

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(3,3,3), desc.RBB);
}

TEST_F(BuildingBerthTest, AddBigPrimitive3X3X1)
{
    m_builder->SetElement(ElementType::CilindricPlatform, vector3i_t(1,0,1), Directions::pZ);

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(3,1,3), (desc.RBB));
}

TEST_F(BuildingBerthTest, ConstructPillar)
{
    for (size_t i = 0; i < 100; ++i)
    {
        m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,i), Directions::pZ);
    }

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(1,1,100), (desc.RBB));
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
                m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pZ);
            }
        }
    }

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(cubeScales, cubeScales, cubeScales), (desc.RBB));
}

TEST_F(BuildingBerthTest, IterateThroughSolidCube)
{
    const size_t cubeScales = 64;
    for (size_t x = 0; x < cubeScales; ++x)
    {
        for (size_t y = 0; y < cubeScales; ++y)
        {
            for (size_t z = 0; z < cubeScales; ++z)
            {
                 m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pZ);
            }
        }
    }
    ASSERT_NO_THROW ( m_builder->GetCore().IterrateObject([&](size_t, size_t, size_t, Element&){}));
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
                if ((x+y+z) % 2)
                    m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pZ);
            }
        }
    }

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(cubeScales, cubeScales, cubeScales), (desc.RBB));
}

TEST_F(BuildingBerthTest, IterateThroughSpongeSystem)
{
    const size_t cubeScales = 64;
    for (size_t x = 0; x < cubeScales; ++x)
    {
        for (size_t y = 0; y < cubeScales; ++y)
        {
            for (size_t z = 0; z < cubeScales; ++z)
            {
                if ((x+y+z) % 2)
                    m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pZ);
            }
        }
    }
    ASSERT_NO_FATAL_FAILURE ( m_builder->GetCore().IterrateObject([&](size_t x, size_t y, size_t z, Element&){
        ASSERT_EQ(1, ((x+y+z) % 2));
    }));
}

TEST_F(BuildingBerthTest, ElementNeighborhood)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,1,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pZ, true);
    ASSERT_NO_FATAL_FAILURE (m_builder->GetCore().IterrateObject(
        [&](size_t, size_t y, size_t, Element& item){
            switch(y)
            {
            case 0:
                ASSERT_EQ(Directions::pY, item.neighbourhood); break;
            case 1:
                ASSERT_EQ(Directions::pY | Directions::nY, item.neighbourhood); break;
            case 2:
                ASSERT_EQ(Directions::nY, item.neighbourhood); break;
            default:
                FAIL();
            }
    }));
}

TEST_F(BuildingBerthTest, CubeToCubeNeighbors)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pZ, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));

    m_builder->SetElement(ElementType::Cube, vector3i_t(2,0,1), Directions::pZ, true);
    ASSERT_EQ(Directions::pX, el->neighbourhood);

    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,1), Directions::pZ, true);
    ASSERT_EQ(Directions::pX | Directions::nX, el->neighbourhood);

    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,0), Directions::pZ, true);
    ASSERT_EQ(Directions::pX | Directions::nX | Directions::nZ, el->neighbourhood);

    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,2), Directions::pZ, true);
    ASSERT_EQ(Directions::pX | Directions::nX | Directions::nZ | Directions::pZ, el->neighbourhood);
}

TEST_F(BuildingBerthTest, CubeToWedgeNeighbors)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pZ, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));

    m_builder->SetElement(ElementType::Wedge, vector3i_t(2,0,1), Directions::pZ, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::pZ, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pZ, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::pZ, true);
    ASSERT_EQ(Directions::pZ, el->neighbourhood);
}

TEST_F(BuildingBerthTest, RotatedNeighboursAffected)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pZ, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));

    m_builder->SetElement(ElementType::Wedge, vector3i_t(2,0,1), Directions::pX, true);
    ASSERT_EQ(Directions::pX, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    ASSERT_EQ(Directions::pX | Directions::nX, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::nZ, true);
    ASSERT_EQ(Directions::pX | Directions::nX | Directions::nZ, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::pZ, true);
    ASSERT_EQ(Directions::pX | Directions::nX | Directions::nZ | Directions::pZ, el->neighbourhood);
}

TEST_F(BuildingBerthTest, RotatedNeighboursNotAffected)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pZ, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));

    m_builder->SetElement(ElementType::Wedge, vector3i_t(2,0,1), Directions::nX, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::pX, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pZ, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::nZ, true);
    ASSERT_EQ(0, el->neighbourhood);
}

TEST_F(BuildingBerthTest, Generated_NoWedgeMorfing)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::pZ, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,1));
    ASSERT_EQ(ElementType::Wedge, el->construction->primitiveUID);
}

TEST_F(BuildingBerthTest, Generated_OuterWedgeAngle)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::pZ, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
}

TEST_F(BuildingBerthTest, Generated_PiramidTop_1)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nZ, true);

    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(1,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(0,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
}

TEST_F(BuildingBerthTest, Generated_PiramidTop_2)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pX, true);

    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(1,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(0,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
}

TEST_F(BuildingBerthTest, Generated_PiramidTop_3)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nZ, true);

    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(1,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(0,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
}

TEST_F(BuildingBerthTest, Generated_PiramidTop_4)
{
    const size_t size = 6;
    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(i,0,0), Directions::nZ, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(i,0,size-1), Directions::pZ, true);
    }

    for (size_t i = 0; i < size; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,i), Directions::nX, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(size-1,0,i), Directions::pX, true);
    }

    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,size - 1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(size - 1,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(size - 1,0,size - 1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(0,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
}

TEST_F(BuildingBerthTest, Generated_PiramidTop_5)
{
    const size_t size = 3;
    for (size_t i = 0; i < size; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,i), Directions::nX, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(size-1,0,i), Directions::pX, true);
    }

    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(i,0,0), Directions::nZ, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(i,0,size-1), Directions::pZ, true);
    }

    Element *el = m_builder->GetCore().GetElement(vector3i_t(0,0,size - 1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(size - 1,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(size - 1,0,size - 1));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
    el = m_builder->GetCore().GetElement(vector3i_t(0,0,0));
    ASSERT_EQ(ElementType::WedgeOutCorner, el->construction->primitiveUID);
}
// eof