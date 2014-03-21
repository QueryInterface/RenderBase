#include "ObjectLibrary.h"
#include <gtest/gtest.h>
#include <memory>

using namespace Constructor;

TEST(ObjectLibraryTest, ObjectLibraryIsASingletone)
{
    IObjectLibrary* lib1 = IObjectLibrary::instance();
    IObjectLibrary* lib2 = IObjectLibrary::instance();

    ASSERT_EQ(lib1, lib2) << "Library must be a sinletone";
}

TEST(ObjectLibraryTest, CanGetDescriptionOfSpaceElementType)
{
    const ElementDescription& space = IObjectLibrary::instance()->GetElementDescription(ET_Space);

    ASSERT_EQ(ET_Space, space.primitiveUID) << "incompatible primitive type";
}

// eof