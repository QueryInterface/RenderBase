#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, GetSpaceMesh)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Space);

    IMesh::GeometryDesc desc;
    mesh.GetGeometryDesc(desc);
    ASSERT_EQ(0, desc.groups.size());
    IMeshPtr meshClone = mesh.Clone();
    meshClone->GetGeometryDesc(desc);
    ASSERT_EQ(0, desc.groups.size()) << "incorrect cloned object";
}

TEST(MeshLibraryTest, GetCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Cube);

    IMesh::GeometryDesc desc;
    mesh.GetGeometryDesc(desc);
    ASSERT_EQ(6, desc.groups.size()) << "incorrect number of indices";
    size_t verticesTotal = 0;
    for (size_t i = 0; i < desc.groups.size(); ++i)
        verticesTotal += desc.groups[i].count;
    ASSERT_EQ(36, verticesTotal);
}

TEST(MeshLibraryTest, GetClonedCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    IMeshPtr meshClone = lib.GetMesh(ElementType::Cube).Clone();
    IMesh::GeometryDesc desc;

    meshClone->GetGeometryDesc(desc);
    ASSERT_EQ(6, desc.groups.size()) << "incorrect number of indices";
    size_t verticesTotal = 0;
    for (size_t i = 0; i < desc.groups.size(); ++i)
        verticesTotal += desc.groups[i].count;
    ASSERT_EQ(36, verticesTotal);
}

TEST(MeshLibraryTest, GetCubeMeshFace)
{
    ILibrary& lib = *ILibrary::library();
    const ILibraryMesh& mesh = lib.GetMesh(ElementType::Cube);
    IMesh::GeometryDesc desc;
    mesh.GetGeometryDesc(Directions::pX, desc);
    ASSERT_EQ(1, desc.groups.size()) << "incorrect number of faces returned";
    size_t verticesTotal = 0;
    for (size_t i = 0; i < desc.groups.size(); ++i)
        verticesTotal += desc.groups[i].count;
    ASSERT_EQ(6, verticesTotal);

    mesh.GetGeometryDesc(Directions::pX | Directions::nX | Directions::nZ, desc);
    ASSERT_EQ(3, desc.groups.size()) << "incorrect number of faces returned";
    verticesTotal = 0;
    for (size_t i = 0; i < desc.groups.size(); ++i)
        verticesTotal += desc.groups[i].count;
    ASSERT_EQ(18, verticesTotal);
}


// eof