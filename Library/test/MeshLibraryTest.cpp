#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, GetCubeMesh)
{
    ILibrary& lib = *ILibrary::library();
    const Mesh& mesh = lib.GetMesh(ElementType::Cube, Directions::All);
    ASSERT_EQ(mesh.indices.size(), 12) << "incorrect number of indices";
}


// eof