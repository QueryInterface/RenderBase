#include "BuildingBerth.h"
#include <gtest/gtest.h>
#include <memory>

using namespace ConstructorImpl;

#ifndef min 
#define min(a, b) (a)<(b) ? (a) : (b)
#endif

#ifndef max 
#define max(a, b) (a)>(b) ? (a) : (b)
#endif

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
        const IMesh::Desc& desc = m_builder->GetHull().GetDesc();

        auto& positions = desc.Shapes[0].Positions;
        size_t verticesTotal = positions.Data.size();

        vector3f_t maximum(0,0,0);
        vector3f_t minimum(500,500,500);

        for (size_t j = 0; j < verticesTotal; j += 3)
        {
            vector3f_t current(positions.Data[j], positions.Data[j+1], positions.Data[j+2]);
            maximum.x = (max(maximum.x, current.x));
            maximum.y = (max(maximum.y, current.y));
            maximum.z = (max(maximum.z, current.z));

            minimum.x = (min(minimum.x, current.x));
            minimum.y = (min(minimum.y, current.y));
            minimum.z = (min(minimum.z, current.z));
        }

        EXPECT_EQ(refCount, verticesTotal/3);

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

    void exportMesh(const IMesh::Desc& desc, std::string fileName)
    {
        FILE *f = nullptr;
        fopen_s(&f, fileName.c_str(), "w");
        if (nullptr == f)
            return;
        vector3i_t wlh = m_builder->GetCore().ConstructionDesc().RBB - m_builder->GetCore().ConstructionDesc().LFT;
        vector3i_t lft = m_builder->GetCore().ConstructionDesc().LFT;
        // save vertices
        auto& positions = desc.Shapes[0].Positions;
        for (size_t j = 0; j < positions.Data.size(); j += 3)
        {
            vector3f_t current(positions.Data[j], positions.Data[j + 1], positions.Data[j + 2]);
            fprintf(f, "v %.3f %.3f %.3f\n", current.x - lft.x - wlh.x/2.0, current.y - lft.y - wlh.y/2.0, current.z - lft.z - wlh.z/2.0);
        }

        //save indices
        for (size_t j = 0; j < (positions.Data.size() / positions.ElementSize); j += 3)
        {
            fprintf(f, "f %u %u %u\n", j + 1, j + 2, j + 3);
        }
        fclose(f);
    }

    std::unique_ptr<BuildingBerth> m_builder;
};

TEST_F(MeshBuilderTest, DISABLED_SingleElementMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    ASSERT_NO_FATAL_FAILURE(checkMesh(36, vector3f_t(0,0,0), vector3f_t(1,1,1), "c:\\tmp\\cube.obj"));
}

TEST_F(MeshBuilderTest, DISABLED_ElementsMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,4,0), Directions::pZ, true);

    ASSERT_NO_FATAL_FAILURE(checkMesh(36 * 3, vector3f_t(0,0,0), vector3f_t(1,5,1), "c:\\tmp\\cube_pillar.obj"));
}

TEST_F(MeshBuilderTest, DISABLED_SinglePillarMesh)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,0,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,1,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Cube, vector3i_t(0,2,0), Directions::pZ, true);

    ASSERT_NO_FATAL_FAILURE(checkMesh(84, vector3f_t(0,0,0), vector3f_t(1,3,1), "c:\\tmp\\united_pillar.obj"));
}

TEST_F(MeshBuilderTest, DISABLED_CubeMesh)
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

TEST_F(MeshBuilderTest, DISABLED_SpongeMesh)
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

TEST_F(MeshBuilderTest, DISABLED_WedgeCross)
{
    m_builder->SetElement(ElementType::Cube, vector3i_t(1,0,1), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(2,0,1), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::pZ, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\wedge_cross.obj");
}

TEST_F(MeshBuilderTest, DISABLED_Wedges)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pZ, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\wedges.obj");
}

TEST_F(MeshBuilderTest, DISABLED_Pyramid)
{
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(3,0,3), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(1,0,3), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(2,0,3), Directions::pZ, true);
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(3,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(3,0,1), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(3,0,2), Directions::pX, true);
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(0,0,3), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(1,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(2,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(0,0,0), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge,       vector3i_t(0,0,2), Directions::nX, true);
    m_builder->SetElement(ElementType::Cube ,       vector3i_t(1,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Cube ,       vector3i_t(1,0,2), Directions::nX, true);
    m_builder->SetElement(ElementType::Cube ,       vector3i_t(2,0,2), Directions::nX, true);
    m_builder->SetElement(ElementType::Cube ,       vector3i_t(2,0,1), Directions::nX, true);

    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(2,1,2), Directions::pX, true);
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(2,1,1), Directions::nZ, true);
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(1,1,2), Directions::pZ, true);
    m_builder->SetElement(ElementType::WedgeOutCorner, vector3i_t(1,1,1), Directions::nX, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\piramid.obj");
}

TEST_F(MeshBuilderTest, DISABLED_OuterWedgeAngle)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,2), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::nZ, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\outCorner.obj");
}

TEST_F(MeshBuilderTest, DISABLED_InWedgeAngle)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,1), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,2), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,2), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,0), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(1,0,0), Directions::pZ, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\inCorner.obj");
}

TEST_F(MeshBuilderTest, DISABLED_Cocentric_in)
{
    size_t size = 3;
    size_t offset = 2;
    for (size_t i = 0; i < size; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset,0,offset+i), Directions::nX, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+size-1,0,offset+i), Directions::pX, true);
    }

    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+i,0,offset+0), Directions::nZ, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+i,0,offset+size-1), Directions::pZ, true);
    }

    size = 7;

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

    offset = 12;
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset,0,1), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+1,0,0), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+1,0,1), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset,0,0), Directions::nZ, true);

    m_builder->SetElement(ElementType::Wedge, vector3i_t(10,0,0), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(10,0,1), Directions::nZ, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\OuterCenters.obj");
}


TEST_F(MeshBuilderTest, DISABLED_Cocentric_out)
{
    size_t size = 3;
    size_t offset = 2;
    for (size_t i = 0; i < size; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset,0,offset+i), Directions::pX, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+size-1,0,offset+i), Directions::nX, true);
    }

    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+i,0,offset+0), Directions::pZ, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+i,0,offset+size-1), Directions::nZ, true);
    }

    size = 7;

    for (size_t i = 1; i < size - 1; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(i,0,0), Directions::pZ, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(i,0,size-1), Directions::nZ, true);
    }

    for (size_t i = 0; i < size; ++i)
    {
        m_builder->SetElement(ElementType::Wedge, vector3i_t(0,0,i), Directions::pX, true);
        m_builder->SetElement(ElementType::Wedge, vector3i_t(size-1,0,i), Directions::nX, true);
    }

    offset = 12;
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset,0,1), Directions::pX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+1,0,0), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset+1,0,1), Directions::nZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(offset,0,0), Directions::pZ, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\InnerCenters.obj");
}

TEST_F(MeshBuilderTest, DISABLED_WedgeSpikes)
{
    m_builder->SetElement(ElementType::Wedge, vector3i_t(3,0,3), Directions::nX, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(3,0,4), Directions::pZ, true);
    m_builder->SetElement(ElementType::Wedge, vector3i_t(4,0,4), Directions::nX, true);

    const IMesh::Desc& desc = m_builder->GetHull().GetDesc();
    exportMesh(desc, "c:\\tmp\\WedgeSpikes.obj");
}
// eof