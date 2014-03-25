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

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, X, Y, Z, DX, DY, DZ)                                  \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                       \
{                                                                                                       \
    const ElementDescription& desc = IObjectLibrary::instance()->GetElementDescription(ET_##Type);      \
    ASSERT_EQ(ET_##Type, desc.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;       \
    EXPECT_EQ(desc.TLF.x, (X));                                                                         \
    EXPECT_EQ(desc.TLF.y, (Y));                                                                         \
    EXPECT_EQ(desc.TLF.z, (Z));                                                                         \
    EXPECT_EQ(desc.Dimentions.x, (DX));                                                                 \
    EXPECT_EQ(desc.Dimentions.y, (DY));                                                                 \
    EXPECT_EQ(desc.Dimentions.z, (DZ));                                                                 \
    EXPECT_EQ(desc.direction, ED_pY);

#define END_CHECK_PRIMITIVE_TEST() }


BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Space, 0, 0, 0, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cube,  0, 0, 0, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Wedge, 0, 0, 0, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Ledder, 0, 0, 0, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cilinder, 0, 0, 0, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, CilindricPlatform, -1, 0, -1, 3, 1, 3)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Sphere, 0, 0, 0, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

// eof