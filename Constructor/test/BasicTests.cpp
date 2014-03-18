#include "ModelConstructor.h"
#include <gtest/gtest.h>
#include <memory>

using namespace Constructor;

class BasicConstructorTest : public ::testing::Test
{
public:
    BasicConstructorTest(){};
protected:
    virtual void SetUp() 
    {
        m_Constructor.reset(new ModelConstructor);
    };

    virtual void TearDown() 
    {
        m_Constructor.reset(0);
        m_vertices.clear();
    };

protected:
	std::unique_ptr<ModelConstructor> m_Constructor;
    VertexList m_vertices;
};

TEST_F(BasicConstructorTest, EmptyModel)
{
    ASSERT_EQ(0, m_Constructor->GetModel(m_vertices));
    ASSERT_EQ(0, m_vertices.size());
}

TEST_F(BasicConstructorTest, Space_Nothing_Added)
{
    ASSERT_FALSE(m_Constructor->SetPrimitive(PT_Space, 0,0,0));
    ASSERT_EQ(0, m_Constructor->GetModel(m_vertices));
    ASSERT_EQ(0, m_vertices.size());
}

// eof