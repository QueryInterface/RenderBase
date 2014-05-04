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

TEST_F(ObjectTest, Position)
{
    ASSERT_EQ(m_object0->GetPosition(), Vector3<float>(0, 0, 0));
    m_object0->SetPosition(Vector3<float>(1, 2, 3));
    ASSERT_EQ(m_object0->GetPosition(), Vector3<float>(1, 2, 3));
}

TEST_F(ObjectTest, Clone)
{
    m_object0->SetPosition(Vector3<float>(2, 3, 4));
    IObjectPtr clonedObj = m_object0->Clone();
    ASSERT_EQ(clonedObj->GetPosition(), m_object0->GetPosition());
}

TEST_F(ObjectTest, AttachBidirectional)
{
    // Place object in one point
    m_object0->SetPosition(Vector3<float>(0, 0, 0));
    m_object1->SetPosition(Vector3<float>(0, 0, 0));
    m_object0->AttachBidirectional(m_object1);
    // Move object 0
    m_object0->SetPosition(Vector3<float>(1, 2, 3));
    ASSERT_EQ(m_object0->GetPosition(), Vector3<float>(1, 2, 3));
    ASSERT_EQ(m_object1->GetPosition(), m_object0->GetPosition());
    // Move object 1
    m_object1->SetPosition(Vector3<float>(5, 6, 7));
    ASSERT_EQ(m_object1->GetPosition(), Vector3<float>(5, 6, 7));
    ASSERT_EQ(m_object0->GetPosition(), m_object1->GetPosition());
    // Detach objects
    m_object1->Detach(m_object0);
    m_object0->SetPosition(Vector3<float>(1, 2, 3));
    ASSERT_EQ(m_object0->GetPosition(), Vector3<float>(1, 2, 3));
    //ASSERT_EQ(m_object1->GetPosition(), Vector3<float>(5, 6, 7));
}

