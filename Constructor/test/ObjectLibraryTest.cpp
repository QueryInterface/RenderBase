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

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, X, Y, Z)                                              \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                       \
{                                                                                                       \
    const ElementDescription& desc = IObjectLibrary::instance()->GetElementDescription(ET_##Type);      \
    ASSERT_EQ(ET_##Type, desc.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;       \
    EXPECT_EQ(desc.TLF.x, 0);                                                                           \
    EXPECT_EQ(desc.TLF.y, 0);                                                                           \
    EXPECT_EQ(desc.TLF.z, 0);                                                                           \
    EXPECT_EQ(desc.Dimentions.x, (X));                                                                  \
    EXPECT_EQ(desc.Dimentions.y, (Y));                                                                  \
    EXPECT_EQ(desc.Dimentions.z, (Z));                                                                  \
    EXPECT_EQ(desc.Direction.x, 1);                                                                     \
    EXPECT_EQ(desc.Direction.y, 0);                                                                     \
    EXPECT_EQ(desc.Direction.z, 0);

#define END_CHECK_PRIMITIVE_TEST() }


BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Space, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cube, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Wedge, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Ledder, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cilinder, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, CilindricPlatform, 3, 3, 1)
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Sphere, 1, 1, 1)
END_CHECK_PRIMITIVE_TEST();

// eof