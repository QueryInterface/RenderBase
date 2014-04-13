#include "ConstructionLibrary.h"
#include <gtest/gtest.h>
#include <memory>

using namespace Constructor;

TEST(ConstructionLibraryTest, ConstructionLibraryIsASingletone)
{
    IConstructionLibrary* lib1 = IConstructionLibrary::instance();
    IConstructionLibrary* lib2 = IConstructionLibrary::instance();

    ASSERT_EQ(lib1, lib2) << "Library must be a singletone";
}

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, tlf, brb)                                             \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                       \
{                                                                                                       \
    const ConstructionDescription& desc = IConstructionLibrary::instance()->GetConstructionDescription(ET_##Type);  \
    ASSERT_EQ(ET_##Type, desc.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;       \
    EXPECT_EQ(desc.LFT, tlf);                                                                           \
    EXPECT_EQ(desc.RBB, (brb));                                                                         \
    EXPECT_EQ(desc.direction, ED_pY);

#define END_CHECK_PRIMITIVE_TEST() }


BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Space, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Cube,  Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Wedge, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Ledder, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Cilinder, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, CilindricPlatform, Vector3D(-1, 0, -1), Vector3D(2, 1, 2))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Sphere, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

// eof