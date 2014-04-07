#include "RangeList.h"
#include <gtest/gtest.h>
#include <memory>
#include <map>

using namespace Utils;

class RangeVectorTest : public ::testing::Test
{
public:
    static const size_t c_size = 512;
    void SetUp()
    {
        m_ranges.reset(new range_list<int>(c_size));
    }
    void TearDown()
    {
        m_ranges.reset();
    }
protected:
    std::unique_ptr< range_list<int> > m_ranges;
};

TEST_F(RangeVectorTest, EmptyItem)
{
    ASSERT_TRUE( nullptr == m_ranges->get_item_at(100) );
}

TEST_F(RangeVectorTest, AddItem)
{
    ASSERT_NO_THROW(m_ranges->insert(100, 10));
    ASSERT_EQ( 10, *m_ranges->get_item_at(100) );
}

TEST_F(RangeVectorTest, AddItems)
{
    ASSERT_NO_THROW(m_ranges->insert(100, 10));
    ASSERT_NO_THROW(m_ranges->insert(300, 30));
    ASSERT_EQ( 10, *m_ranges->get_item_at(100) );
    ASSERT_EQ( 30, *m_ranges->get_item_at(300) );
}

TEST_F(RangeVectorTest, SegmentsCount)
{
    ASSERT_NO_THROW(m_ranges->insert(100, 10));
    ASSERT_NO_THROW(m_ranges->insert(101, 11));
    ASSERT_NO_THROW(m_ranges->insert(300, 30));
    ASSERT_EQ( 10, *m_ranges->get_item_at(100) );
    ASSERT_EQ( 11, *m_ranges->get_item_at(101) );
    ASSERT_EQ( 30, *m_ranges->get_item_at(300) );
    ASSERT_EQ( 2, m_ranges->segments_count() );
}
// eof