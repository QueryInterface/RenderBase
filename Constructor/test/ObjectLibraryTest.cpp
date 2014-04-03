#include "ObjectLibrary.h"
#include <gtest/gtest.h>
#include <memory>

using namespace Constructor;

TEST(ObjectLibraryTest, ObjectLibraryIsASingletone)
{
    IObjectLibrary* lib1 = IObjectLibrary::instance();
    IObjectLibrary* lib2 = IObjectLibrary::instance();

    ASSERT_EQ(lib1, lib2) << "Library must be a singletone";
}

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, tlf, brb)                                             \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                       \
{                                                                                                       \
    const ElementDescription& desc = IObjectLibrary::instance()->GetElementDescription(ET_##Type);      \
    ASSERT_EQ(ET_##Type, desc.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;       \
    EXPECT_EQ(desc.LFT, tlf);                                                                           \
    EXPECT_EQ(desc.RBB, (brb));                                                                         \
    EXPECT_EQ(desc.direction, ED_pY);

#define END_CHECK_PRIMITIVE_TEST() }


BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Space, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cube,  Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Wedge, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Ledder, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cilinder, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, CilindricPlatform, Vector3D(-1, 0, -1), Vector3D(2, 1, 2))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Sphere, Vector3D(0, 0, 0), Vector3D(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

// eof