#include "BuildingBerth.h"
#include <gtest/gtest.h>
#include <memory>

using namespace ConstructorImpl;

class MeshBuilderTest : public ::testing::Test
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

TEST_F(MeshBuilderTest, SingleElementMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    ASSERT_NO_FATAL_FAILURE(checkMesh(36, vector3f_t(0,0,0), vector3f_t(1,1,1), "c:\\tmp\\cube.obj"));
}

TEST_F(MeshBuilderTest, ElementsMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,4,0), Directions::pZ, true);

    ASSERT_NO_FATAL_FAILURE(checkMesh(36 * 3, vector3f_t(0,0,0), vector3f_t(1,5,1), "c:\\tmp\\cube_pillar.obj"));
}

TEST_F(MeshBuilderTest, SinglePillarMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,1,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pZ, true);

    ASSERT_NO_FATAL_FAILURE(checkMesh(84, vector3f_t(0,0,0), vector3f_t(1,3,1), "c:\\tmp\\united_pillar.obj"));
}

TEST_F(MeshBuilderTest, CubeMesh)
{
    const size_t cubeScales = 3;
    for (size_t x = 0; x < cubeScales; ++x)
        for (size_t y = 0; y < cubeScales; ++y)
            for (size_t z = 0; z < cubeScales; ++z)
            {
                m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pZ, true);
            }
    const float BBox = (float)cubeScales;
    ASSERT_NO_FATAL_FAILURE(checkMesh(cubeScales*cubeScales*6*6, vector3f_t(0,0,0), vector3f_t(BBox, BBox, BBox), "c:\\tmp\\HugeCube.obj"));
}

TEST_F(MeshBuilderTest, SpongeMesh)
{
    const size_t cubeScales = 6;
    for (size_t x = 0; x < cubeScales; ++x)
        for (size_t y = 0; y < cubeScales; ++y)
            for (size_t z = 0; z < cubeScales; ++z)
            {
                if ((x+y+z) % 2)
                    m_builder->SetElement(ElementType::Cube, vector3i_t(x,y,z), Directions::pZ);
            }
    const float BBox = (float)cubeScales;
    //hmmm... 64? * 64?
    ASSERT_NO_FATAL_FAILURE(checkMesh((cubeScales*cubeScales*cubeScales/2)*36, vector3f_t(0,0,0), vector3f_t(BBox, BBox, BBox), "c:\\tmp\\sponge.obj"));
}

TEST_F(MeshBuilderTest, AnyTypeMesh)
{
    const size_t basement = 3;
    for (size_t x = 0; x < basement; ++x)
        for (size_t z = 0; z < basement; ++z)
        {
            if (x != 1 || z != 1)
                m_builder->SetElement(ElementType::Cube, vector3i_t(x,0,z), Directions::pZ, true);
        }
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pZ, true);
    IMesh::GeometryDesc desc;
    m_builder->GetHull().GetGeometryDesc(0, desc);
    exportMesh(desc, "c:\\tmp\\piramid.obj");
}
// eof