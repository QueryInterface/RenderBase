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
    TEST(Fixture, CanGetDescriptionOf##Type##ElementType)                                               \
{                                                                                                       \
    const ElementDescription& space = IObjectLibrary::instance()->GetElementDescription(ET_##Type);     \
    ASSERT_EQ(ET_##Type, space.primitiveUID) << "incorrect primitive type expected: ET_" << #Type;      \
}

CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Space);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Cube);
CHECK_PRIMITIVE_TEST(ObjectLibraryTest, Wedge);

// eof