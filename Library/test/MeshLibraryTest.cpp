#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, GetSpaceMesh)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Space);
    IMesh::IndexData_t index;
    mesh.GetIndexData(Directions::All, index);
    ASSERT_EQ(index.size(), 0) << "incorrect number of indices";
    IMeshPtr meshClone = mesh.Clone();
    meshClone->GetIndexData(Directions::All, index);
    ASSERT_EQ(index.size(), index.size()) << "incorrect cloned object";
}

TEST(MeshLibraryTest, GetCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Cube);
    IMesh::IndexData_t index;
    mesh.GetIndexData(Directions::All, index);
    ASSERT_EQ(index.size(), 36) << "incorrect number of indices";
    IMeshPtr meshClone = mesh.Clone();
    meshClone->GetIndexData(Directions::All, index);
    ASSERT_EQ(index.size(), index.size()) << "incorrect cloned object";
}

TEST(MeshLibraryTest, GetCubeMeshFace)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Cube);
    IMesh::IndexData_t index;
    mesh.GetIndexData(Directions::pX, index);
    ASSERT_EQ(index.size(), 6) << "incorrect number of indices for one face";

    mesh.GetIndexData(Directions::pX | Directions::nX | Directions::nZ, index);
    ASSERT_EQ(index.size(), 18) << "incorrect number of indices for set of faces";
}


// eof