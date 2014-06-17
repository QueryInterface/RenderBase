#include "Library.h"
#include <gtest/gtest.h>
#include <memory>

TEST(ConstructionLibraryTest, ConstructionLibraryIsASingletone)
{
    ILibrary* lib1 = ILibrary::library();
    ILibrary* lib2 = ILibrary::library();

    ASSERT_EQ(lib1, lib2) << "Library must be a singletone";
}

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, tlf, brb)                                                             \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                                       \
{                                                                                                                       \
    const ConstructionDescription& desc = ILibrary::library()->GetConstruction(ElementType::##Type);                    \
    ASSERT_EQ(ElementType::##Type, desc.primitiveUID) << "incorrect primitive type expected: ElementType::" << #Type;   \
    EXPECT_EQ(desc.LFT, tlf);                                                                                           \
    EXPECT_EQ(desc.RBB, (brb));                                                                                         \
    EXPECT_EQ(desc.direction, Directions::pZ);

#define END_CHECK_PRIMITIVE_TEST() }


BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Space, vector3i_t(0, 0, 0), vector3i_t(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Cube,  vector3i_t(0, 0, 0), vector3i_t(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Wedge, vector3i_t(0, 0, 0), vector3i_t(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Ledder, vector3i_t(0, 0, 0), vector3i_t(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Cilinder, vector3i_t(0, 0, 0), vector3i_t(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, CilindricPlatform, vector3i_t(-1, 0, -1), vector3i_t(2, 1, 2))
END_CHECK_PRIMITIVE_TEST();

BEGIN_CHECK_PRIMITIVE_TEST(ConstructionLibraryTest, Sphere, vector3i_t(0, 0, 0), vector3i_t(1, 1, 1))
END_CHECK_PRIMITIVE_TEST();

// eof