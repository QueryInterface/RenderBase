#include "OvermindImpl.h"
#include <gtest/gtest.h>
#include <memory>

using namespace OvermindImpl;

class OvermindTest : public ::testing::Test
{
public:

    void SetUp()
    {
        m_builder.reset(new Overmind);
    }
    void TearDown()
    {
        m_builder.reset();
    }
protected:
    std::unique_ptr<Overmind> m_builder;
};

TEST_F(OvermindTest, AddSpace)
{
    //ASSERT_FALSE(m_builder->SetElement(ElementType::Space, vector3i_t(0,0,0), Directions::pZ));
}

// eof