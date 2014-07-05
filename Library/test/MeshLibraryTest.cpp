#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, GetSpaceMesh)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Space);

    GeometryMesh desc;
    MeshProperties prop = {Directions::All, vector3f_t(0,0,0)};
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(0, desc.vertices.size());
    IMeshPtr meshCloneTemp = mesh.Clone();
    ILibraryMesh* meshClone = (ILibraryMesh*)meshCloneTemp.get();
    meshClone->ConstructGeometry(prop, desc);
    ASSERT_EQ(0, desc.vertices.size()) << "incorrect cloned object";
}

TEST(MeshLibraryTest, GetCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Cube);

    GeometryMesh desc;
    MeshProperties prop = {Directions::All, vector3f_t(0,0,0)};
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(6, desc.vertices.size()) << "incorrect number of indices";
    ASSERT_EQ(36, desc.vertices.size());
}

TEST(MeshLibraryTest, GetClonedCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    IMeshPtr meshCloneTemp = lib.GetMesh(ElementType::Cube).Clone();
    ILibraryMesh* meshClone = (ILibraryMesh*)meshCloneTemp.get();
    GeometryMesh desc;

    MeshProperties prop = {Directions::All, vector3f_t(0,0,0)};
    meshClone->ConstructGeometry(prop, desc);
    ASSERT_EQ(6, desc.vertices.size()) << "incorrect number of indices";
    ASSERT_EQ(36, desc.vertices.size());
}

TEST(MeshLibraryTest, GetCubeMeshFace)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Cube);
    GeometryMesh desc;
    MeshProperties prop = {Directions::pX, vector3f_t(0,0,0)};
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(6, desc.vertices.size()) << "incorrect number of faces returned";

    prop.flags = Directions::pX | Directions::nX | Directions::nZ;
    mesh.ConstructGeometry(prop, desc);
    ASSERT_EQ(18, desc.vertices.size());
}


// eof