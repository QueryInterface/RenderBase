#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(MeshLibraryTest, ConstructionLibraryIsASingletone)
{
    ILibrary* lib1 = ILibrary::library();
    ILibrary* lib2 = ILibrary::library();

    ASSERT_EQ(lib1, lib2) << "Library must be a singletone";
}

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, tlf, brb)                                             \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                       \
{                                                                                                       \
    const ConstructionDescription& desc = ILibrary::library()->GetConstruction(ET_##Type);              \
    ASSERT_EQ(ET_##Type, desc.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;       \
    EXPECT_EQ(desc.LFT, tlf);                                                                           \
    EXPECT_EQ(desc.RBB, (brb));                                                                         \
    EXPECT_EQ(desc.direction, ED_pY);

#define END_CHECK_PRIMITIVE_TEST() }


BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, Space, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, Cube,  Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, Wedge, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, Ledder, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, Cilinder, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, CilindricPlatform, Vector3D(-1, 0, -1), Vector3D(2, 1, 2))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(MeshLibraryTest, Sphere, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

// eof