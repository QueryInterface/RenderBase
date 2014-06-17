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

    void checkMesh(size_t refCount, vector3f_t refmin, vector3f_t refmax, std::string fileName = "")
    {
        IMesh::GeometryDesc desc;
        m_builder->GetHull().GetGeometryDesc(0, desc);

        size_t verticesTotal = 0;
        for (auto group : desc.groups)
        {
            verticesTotal += group.count;
        }

        vector3f_t maximum(0,0,0);
        vector3f_t minimum(500,500,500);

        for (size_t j = 0; j < desc.layout[0].itemsCount; j += 3)
        {
            vector3f_t current(&desc.layout[0].items[j]);
            maximum.x = (max(maximum.x, current.x));
            maximum.y = (max(maximum.y, current.y));
            maximum.z = (max(maximum.z, current.z));

            minimum.x = (min(minimum.x, current.x));
            minimum.y = (min(minimum.y, current.y));
            minimum.z = (min(minimum.z, current.z));
        }

        EXPECT_EQ(refCount, verticesTotal);

        ASSERT_FLOAT_EQ(refmax.x, maximum.x);
        ASSERT_FLOAT_EQ(refmax.y, maximum.y);
        ASSERT_FLOAT_EQ(refmax.z, maximum.z);

        ASSERT_FLOAT_EQ(refmin.x, minimum.x);
        ASSERT_FLOAT_EQ(refmin.y, minimum.y);
        ASSERT_FLOAT_EQ(refmin.z, minimum.z);

        if (fileName.size())
        {
            exportMesh(desc, fileName);
        }
    }

    void exportMesh(const IMesh::GeometryDesc& desc, std::string fileName)
    {
        FILE *f = nullptr;
        fopen_s(&f, fileName.c_str(), "w");
        if (nullptr == f)
            return;

        // save vertices
        for (size_t j = 0; j < desc.layout[0].itemsCount; j += 3)
        {
            vector3f_t current(&desc.layout[0].items[j]);
            fprintf(f, "v %.3f %.3f %.3f\n", current.x, current.y, current.z);
        }

        //save indices
        for (size_t j = 0; j < (desc.layout[0].itemsCount / desc.layout[0].itemSize); j += 3)
        {
            vector3f_t current(&desc.layout[0].items[j]);
            
            fprintf(f, "f %u %u %u\n", j + 1, j + 2, j + 3);
        }
        fclose(f);
    }

    std::unique_ptr<BuildingBerth> m_builder;
};

TEST_F(BuildingBerthTest, AddSpace)
{
    ASSERT_FALSE(m_builder->SetElement(ElementType::Space, vector3i_t(0,0,0), Directions::pY));
}

TEST_F(BuildingBerthTest, AddPrimitive)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pY));

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(1,1,1), desc.RBB);
}

TEST_F(BuildingBerthTest, AddPrimitiveInNonZeroPosition)
{
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(2,5,3), Directions::pY));

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

    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pY));
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(1,2,1), Directions::pY));
    ASSERT_TRUE(m_builder->SetElement(ElementType::Cube, vector3i_t(2,0,2), Directions::pY));

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(3,3,3), desc.RBB);
}

TEST_F(BuildingBerthTest, AddBigPrimitive3X3X1)
{
    m_builder->SetElement(ElementType::CilindricPlatform, vector3i_t(1,0,1), Directions::pY);

    const ConstructionDescription desc = m_builder->GetCore().ConstructionDesc();
    EXPECT_EQ(vector3i_t(3,1,3), (desc.RBB));
}

TEST_F(BuildingBerthTest, ConstructPillar)
{
    for (size_t i = 0; i < 100; ++i)
    {
        m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,i), Directions::pY);
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
                m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pY);
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
                 m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pY);
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
                    m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pY);
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
                    m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pY);
            }
        }
    }
    ASSERT_NO_FATAL_FAILURE ( m_builder->GetCore().IterrateObject([&](size_t x, size_t y, size_t z, Element&){
        ASSERT_EQ(1, ((x+y+z) % 2));
    }));
}

TEST_F(BuildingBerthTest, ElementNeighborhood)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,1,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pY, true);
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
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pY, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));

    m_builder->SetElement(ElementType::Cube, vector3i_t(2,0,1), Directions::pY, true);
    ASSERT_EQ(Directions::pX, el->neighbourhood);

    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,1), Directions::pY, true);
    ASSERT_EQ(Directions::pX | Directions::nX, el->neighbourhood);

    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,0), Directions::pY, true);
    ASSERT_EQ(Directions::pX | Directions::nX | Directions::nZ, el->neighbourhood);

    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,2), Directions::pY, true);
    ASSERT_EQ(Directions::pX | Directions::nX | Directions::nZ | Directions::pZ, el->neighbourhood);
}

TEST_F(BuildingBerthTest, CubeToWedgeNeighbors)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pY, true);
    Element *el = m_builder->GetCore().GetElement(vector3i_t(1,0,1));

    m_builder->SetElement(ElementType::Wedge, vector3i_t(2,0,1), Directions::pY, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::pY, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pY, true);
    ASSERT_EQ(0, el->neighbourhood);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::pY, true);
    ASSERT_EQ(Directions::pZ, el->neighbourhood);

    IMesh::GeometryDesc desc;
    m_builder->GetHull().GetGeometryDesc(0, desc);
    exportMesh(desc, "c:\\tmp\\wedges.obj");
}

TEST_F(BuildingBerthTest, SingleElementMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pY, true);
    ASSERT_NO_FATAL_FAILURE(checkMesh(36, vector3f_t(0,0,0), vector3f_t(1,1,1), "c:\\tmp\\cube.obj"));
}

TEST_F(BuildingBerthTest, ElementsMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,4,0), Directions::pY, true);

    ASSERT_NO_FATAL_FAILURE(checkMesh(36 * 3, vector3f_t(0,0,0), vector3f_t(1,5,1), "c:\\tmp\\cube_pillar.obj"));
}

TEST_F(BuildingBerthTest, SinglePillarMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,1,0), Directions::pY, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pY, true);

    ASSERT_NO_FATAL_FAILURE(checkMesh(84, vector3f_t(0,0,0), vector3f_t(1,3,1), "c:\\tmp\\united_pillar.obj"));
}

TEST_F(BuildingBerthTest, CubeMesh)
{
    const size_t cubeScales = 3;
    for (size_t x = 0; x < cubeScales; ++x)
        for (size_t y = 0; y < cubeScales; ++y)
            for (size_t z = 0; z < cubeScales; ++z)
            {
                m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pY, true);
            }
    const float BBox = (float)cubeScales;
    ASSERT_NO_FATAL_FAILURE(checkMesh(cubeScales*cubeScales*6*6, vector3f_t(0,0,0), vector3f_t(BBox, BBox, BBox), "c:\\tmp\\HugeCube.obj"));
}

TEST_F(BuildingBerthTest, SpongeMesh)
{
    const size_t cubeScales = 6;
    for (size_t x = 0; x < cubeScales; ++x)
        for (size_t y = 0; y < cubeScales; ++y)
            for (size_t z = 0; z < cubeScales; ++z)
            {
                if ((x+y+z) % 2)
                    m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pY);
            }
    const float BBox = (float)cubeScales;
    //hmmm... 64? * 64?
    ASSERT_NO_FATAL_FAILURE(checkMesh((cubeScales*cubeScales*cubeScales/2)*36, vector3f_t(0,0,0), vector3f_t(BBox, BBox, BBox), "c:\\tmp\\sponge.obj"));
}

TEST_F(BuildingBerthTest, AnyTypeMesh)
{
    const size_t basement = 3;
    for (size_t x = 0; x < basement; ++x)
        for (size_t z = 0; z < basement; ++z)
        {
            if (x != 1 || z != 1)
                m_builder->SetElement(ElementType::Cube, vector3i_t(x,0,z), Directions::pY, true);
        }
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pY, true);
    IMesh::GeometryDesc desc;
    m_builder->GetHull().GetGeometryDesc(0, desc);
    exportMesh(desc, "c:\\tmp\\piramid.obj");
}
// eof