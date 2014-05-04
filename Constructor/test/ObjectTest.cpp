#include <gtest/gtest.h>
#include "ObjectImpl.h"

using namespace std;

class ObjectTest : public ::testing::Test
{
public:

    void SetUp()
    {
        m_object0 = static_pointer_cast<IObject>(make_shared<ComplexObject>());
        m_object1 = static_pointer_cast<IObject>(make_shared<ComplexObject>());
    }
    void TearDown()
    {
        m_object0.reset();
        m_object1.reset();
    }
protected:
    IObjectPtr m_object0;
    IObjectPtr m_object1;
};

TEST_F(ObjectTest, ObjectPosition)
{
    ASSERT_EQ(m_object0->GetPosition(), Vector3<float>(0, 0, 0));
    m_object0->SetPosition(Vector3<float>(1, 2, 3));
    ASSERT_EQ(m_object0->GetPosition(), Vector3<float>(1, 2, 3));
}

TEST_F(ObjectTest, ObjectClone)
{
    m_object0->SetPosition(Vector3<float>(2, 3, 4));
    IObjectPtr clonedObj = m_object0->Clone();
    ASSERT_EQ(clonedObj->GetPosition(), m_object0->GetPosition());
}
