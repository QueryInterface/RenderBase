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

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(Vector3D(1,1,1), desc.RBB);
}

TEST_F(BuildingBerthTest, AddPrimitiveInNonZeroPosition)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, Vector3D(2,5,3), Directions::pY));

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
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

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(Vector3D(3,3,3), desc.RBB);
}

TEST_F(BuildingBerthTest, AddBigPrimitive3X3X1)
{
    m_builder->SetElement(ElementType::CilindricPlatform, Vector3D(1,0,1), Directions::pY);

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(Vector3D(3,1,3), (desc.RBB));
}

TEST_F(BuildingBerthTest, ConstructPillar)
{
    for (size_t i = 0; i < 100; ++i)
    {
        m_builder->SetElement(ElementType::Cube, Vector3D(0,0,i), Directions::pY);
    }

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
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

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(Vector3D(cubeScales, cubeScales, cubeScales), (desc.RBB));
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
                 m_builder->SetElement(ElementType::Cube, Vector3D(x,y,z), Directions::pY);
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
                    m_builder->SetElement(ElementType::Cube, Vector3D(x,y,z), Directions::pY);
            }
        }
    }

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(Vector3D(cubeScales, cubeScales, cubeScales), (desc.RBB));
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
                    m_builder->SetElement(ElementType::Cube, Vector3D(x,y,z), Directions::pY);
            }
        }
    }
    ASSERT_NO_FATAL_FAILURE ( m_builder->GetCore().IterrateObject([&](size_t x, size_t y, size_t z, Element&){
        ASSERT_EQ(1, ((x+y+z) % 2));
    }));
}

TEST_F(BuildingBerthTest, ElementNeighbourhood)
{
    m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,1,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,2,0), Directions::pY, true);
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


TEST_F(BuildingBerthTest, SingleElementMesh)
{
    m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY, true);

    IMesh::GeometryDesc desc;
    m_builder->GetHull().GetGeometryDesc(0, desc);

    Vector3<float> maximum(0,0,0);
    Vector3<float> minimum(500,500,500);
    for (auto group : desc.groups)
    {
        for (size_t j = 0; j < group.count; ++j)
        {
            Vector3<float> current(&desc.layout[0].items[group.indices[j] * 3]);
            maximum.x = (max(maximum.x, current.x));
            maximum.y = (max(maximum.y, current.y));
            maximum.z = (max(maximum.z, current.z));

            minimum.x = (min(minimum.x, current.x));
            minimum.y = (min(minimum.y, current.y));
            minimum.z = (min(minimum.z, current.z));
        }
    }
    EXPECT_FLOAT_EQ(1, maximum.x);
    EXPECT_FLOAT_EQ(1, maximum.y);
    EXPECT_FLOAT_EQ(1, maximum.z);

    EXPECT_FLOAT_EQ(0, minimum.x);
    EXPECT_FLOAT_EQ(0, minimum.y);
    EXPECT_FLOAT_EQ(0, minimum.z);
}

TEST_F(BuildingBerthTest, ElementsMesh)
{
    m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,2,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,4,0), Directions::pY, true);

    Vector3<float> maximum(0,0,0);
    Vector3<float> minimum(500,500,500);

    IMesh::GeometryDesc desc;
    m_builder->GetHull().GetGeometryDesc(0, desc);

    size_t verticesTotal = 0;
    for (auto group : desc.groups)
    {
        verticesTotal += group.count;
    }

    for (size_t j = 0; j < desc.layout[0].itemsCount; j += 3)
    {
        Vector3<float> current(&desc.layout[0].items[j]);
        maximum.x = (max(maximum.x, current.x));
        maximum.y = (max(maximum.y, current.y));
        maximum.z = (max(maximum.z, current.z));

        minimum.x = (min(minimum.x, current.x));
        minimum.y = (min(minimum.y, current.y));
        minimum.z = (min(minimum.z, current.z));
    }

    ASSERT_EQ(36 * 3, verticesTotal);

    ASSERT_FLOAT_EQ(1, maximum.x);
    ASSERT_FLOAT_EQ(5, maximum.y);
    ASSERT_FLOAT_EQ(1, maximum.z);

    ASSERT_FLOAT_EQ(0, minimum.x);
    ASSERT_FLOAT_EQ(0, minimum.y);
    ASSERT_FLOAT_EQ(0, minimum.z);
}

TEST_F(BuildingBerthTest, SinglePillarMesh)
{
    m_builder->SetElement(ElementType::Cube, Vector3D(0,0,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,1,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, Vector3D(0,2,0), Directions::pY, true);

    IMesh::GeometryDesc desc;
    m_builder->GetHull().GetGeometryDesc(0, desc);

    size_t verticesTotal = 0;
    for (size_t i = 0; i < desc.groups.size(); ++i)
        verticesTotal += desc.groups[i].count;

    ASSERT_EQ(84, verticesTotal);
}
// eof