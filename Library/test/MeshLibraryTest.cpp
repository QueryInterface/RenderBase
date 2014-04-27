#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, GetSpaceMesh)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Space);
    ASSERT_EQ(mesh.GetIndexData(Directions::All).size(), 0) << "incorrect number of indices";
}

TEST(MeshLibraryTest, GetCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    const IMesh& mesh = lib.GetMesh(ElementType::Cube);
    ASSERT_EQ(mesh.GetIndexData(Directions::All).size(), 36) << "incorrect number of indices";
}


// eof