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

#define CHECK_PRIMITIVE_TEST(Fixture, Type)                                                             \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                       \
{                                                                                                       \
    const ElementDescription& desc = IObjectLibrary::instance()->GetElementDescription(ET_##Type);      \
    ASSERT_EQ(ET_##Type, desc.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;       \
    EXPECT_EQ(desc.TLF.x, 0);                                                                           \
    EXPECT_EQ(desc.TLF.y, 0);                                                                           \
    EXPECT_EQ(desc.TLF.z, 0);                                                                           \
    EXPECT_EQ(desc.Dimentions.x, 1);                                                                    \
    EXPECT_EQ(desc.Dimentions.y, 1);                                                                    \
    EXPECT_EQ(desc.Dimentions.z, 1);                                                                    \
    EXPECT_EQ(desc.Direction.x, 1);                                                                     \
    EXPECT_EQ(desc.Direction.y, 0);                                                                     \
    EXPECT_EQ(desc.Direction.z, 0);                                                                     \
}

CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Space);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cube);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Wedge);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Ledder);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cilinder);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Sphere);

// eof