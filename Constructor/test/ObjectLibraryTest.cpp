#include "Constructor.h"
#include <gtest/gtest.h>
#include <memory>

#define BEGIN_CHECK_PRIMITIVE_TEST(Fixture, Type, tlf, brb)                                                             \
    TEST(Fixture, DescriptionOf_##Type##_Element)                                                                       \
{                                                                                                                       \
    const ConstructionDescription* desc = Constructor::GetConstructor().GetLibrary().GetConstructionByName(#Type);      \
    ASSERT_EQ(ElementType::##Type, desc->primitiveUID) << "incorrect primitive type expected: ElementType::" << #Type;  \
    EXPECT_EQ(desc->boundingBox.LFT, tlf);                                                                              \
    EXPECT_EQ(desc->boundingBox.RBB, (brb));                                                                            \
    EXPECT_EQ(desc->direction, Directions::pZ);

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

class ObjectLibraryTest : public ::testing::Test
{
public:
    ObjectLibraryTest() : m_constructor(Constructor::GetConstructor()) {};

    void SetUp()
    {
        m_constructor.GetLibrary().Reset();
    }
    void TearDown()
    {
    }

    Constructor& m_constructor;
};

TEST_F(ObjectLibraryTest, Registeration)
{
    std::string testName = "testObject";
    {
        IGameObjectPtr test_obj(new GameObjectBase(testName));
        ASSERT_EQ(Status::OK, m_constructor.GetLibrary().RegisterObject(testName, test_obj));
    }
    const IGameObject* desc = m_constructor.GetLibrary().GetObjectByName(testName);
    ASSERT_TRUE(desc != nullptr);
    ASSERT_STREQ(testName.c_str(), desc->GetName().c_str());
}

TEST_F(ObjectLibraryTest, RegisterationWithElement)
{
    std::string testName = "testObject";
    {
        IGameObject::ObjectProperties newComplexObject = {testName, "", "", "Cube"};
        IGameObjectPtr test_obj(new GameObjectBase(newComplexObject));
        ASSERT_EQ(Status::OK, m_constructor.GetLibrary().RegisterObject(testName, test_obj));
    }
    const IGameObject* desc = m_constructor.GetLibrary().GetObjectByName(testName);
    ASSERT_TRUE(desc != nullptr);
    ASSERT_STREQ(testName.c_str(), desc->GetName().c_str());
}

TEST_F(ObjectLibraryTest, Reset)
{
    std::string testName = "testObject";
    {
        IGameObjectPtr test_obj(new GameObjectBase(testName));
        m_constructor.GetLibrary().RegisterObject(testName, test_obj);
    }
    const IGameObject* desc = m_constructor.GetLibrary().GetObjectByName(testName);
    ASSERT_EQ(Status::OK, m_constructor.GetLibrary().CheckObjectStatus(testName));
    ASSERT_TRUE(desc != nullptr);
    m_constructor.GetLibrary().Reset();

    ASSERT_EQ(Status::ResourceNotFound, m_constructor.GetLibrary().CheckObjectStatus(testName));
    desc = m_constructor.GetLibrary().GetObjectByName(testName);
    ASSERT_TRUE(desc == nullptr);
}

TEST_F(ObjectLibraryTest, DoubleRegistration)
{
    std::string testName = "testObject";

    IGameObjectPtr test_obj(new GameObjectBase(testName));
    m_constructor.GetLibrary().RegisterObject(testName, test_obj);

    ASSERT_EQ(Status::AlreadyExists, m_constructor.GetLibrary().RegisterObject(testName, test_obj));
}

TEST_F(ObjectLibraryTest, PendingRegistration)
{
    std::string testName = "testObject";

    IGameObject::ObjectProperties newComplexObject = {testName, "SomeMesh", "SomeMaterial", "SomeElement"};
    IGameObjectPtr test_obj(new GameObjectBase(newComplexObject));
    m_constructor.GetLibrary().RegisterObject(testName, test_obj);

    ASSERT_EQ(Status::Pending, m_constructor.GetLibrary().RegisterObject(testName, test_obj));
    ASSERT_EQ(Status::Pending, m_constructor.GetLibrary().CheckObjectStatus(testName));
    ASSERT_TRUE(nullptr == m_constructor.GetLibrary().GetObjectByName(testName));
}

TEST_F(ObjectLibraryTest, PendingRegistrationMovesOnline)
{
    std::string testName = "testObject";

    IGameObject::ObjectProperties newComplexObject = {testName, "", "", "SomeElement"};
    IGameObjectPtr test_obj(new GameObjectBase(newComplexObject));
    m_constructor.GetLibrary().RegisterObject(testName, test_obj);

    ASSERT_EQ(Status::Pending, m_constructor.GetLibrary().RegisterObject(testName, test_obj));
    ASSERT_EQ(Status::Pending, m_constructor.GetLibrary().CheckObjectStatus(testName));
}
// eof