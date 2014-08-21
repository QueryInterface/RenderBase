#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, GetSpaceMesh)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Space);

    IMesh::Shape desc;
    MeshProperties prop = {Directions::All, vector3f_t(0,0,0)};
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(0, desc.Positions.Data.size());
    IMeshPtr meshCloneTemp = mesh.Clone();
    ILibraryMesh* meshClone = (ILibraryMesh*)meshCloneTemp.get();
    meshClone->ConstructGeometry(prop, desc);
    ASSERT_EQ(0, desc.Positions.Data.size()) << "incorrect cloned object";
}

TEST(MeshLibraryTest, GetCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Cube);

    IMesh::Shape desc;
    MeshProperties prop = {Directions::All, vector3f_t(0,0,0)};
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(108, desc.Positions.Data.size()) << "incorrect number of vertices";
}

TEST(MeshLibraryTest, GetClonedCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    IMeshPtr meshCloneTemp = lib.GetMesh(ElementType::Cube).Clone();
    ILibraryMesh* meshClone = (ILibraryMesh*)meshCloneTemp.get();
    IMesh::Shape desc;

    MeshProperties prop = {Directions::All, vector3f_t(0,0,0)};
    meshClone->ConstructGeometry(prop, desc);
    ASSERT_EQ(108, desc.Positions.Data.size()) << "incorrect number of vertices";
}

TEST(MeshLibraryTest, GetCubeMeshFace)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Cube);
    IMesh::Shape desc;
    MeshProperties prop = {Directions::pX, vector3f_t(0,0,0)};
    prop.flags = Directions::pX | Directions::nX | Directions::nZ;
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(54, desc.Positions.Data.size());
}


// eof