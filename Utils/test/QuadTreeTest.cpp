#include "QuadTree.h"
#include <gtest/gtest.h>
#include <memory>
#include <map>

using namespace Utils;

class QuadTreeTest : public ::testing::Test
{
public:

    void SetUp()
    {
        m_tree.reset(new QuadTree<int>(512));
    }
    void TearDown()
    {
        m_tree.reset();
    }
protected:
    std::unique_ptr< QuadTree<int> > m_tree;
};

TEST_F(QuadTreeTest, EmptyItem)
{
    ASSERT_TRUE( nullptr == m_tree->item_at(254, 400) );
}

TEST_F(QuadTreeTest, Insert)
{
    ASSERT_NO_THROW( m_tree->insert(254, 400, 10) );
}

TEST_F(QuadTreeTest, InsertedItem)
{
    ASSERT_NO_THROW( m_tree->insert(254, 400, 10) );
    ASSERT_TRUE( nullptr == m_tree->item_at(400, 254) );
    ASSERT_EQ(10, *m_tree->item_at(254, 400));
}

TEST_F(QuadTreeTest, RewriteItem)
{
    m_tree->insert(254, 400, 10);
    ASSERT_EQ(10, *m_tree->item_at(254, 400));
    ASSERT_EQ(10, *m_tree->item_at(254, 400));
    m_tree->insert(254, 400, 50);
    ASSERT_EQ(50, *m_tree->item_at(254, 400));
}

TEST_F(QuadTreeTest, RemoveItem)
{
    m_tree->insert(254, 400, 10);
    ASSERT_EQ(10, *m_tree->item_at(254, 400));
    ASSERT_NO_THROW( m_tree->remove(254, 400) );
    ASSERT_TRUE( nullptr == m_tree->item_at(254, 400) );
}

TEST_F(QuadTreeTest, RemoveItems)
{
    m_tree->insert(254, 400, 10);
    m_tree->insert(254, 401, 15);
    ASSERT_NO_THROW( m_tree->remove(254, 400) );
    ASSERT_TRUE( nullptr == m_tree->item_at(254, 400) );
    ASSERT_TRUE( nullptr != m_tree->item_at(254, 401) );
    ASSERT_EQ( 15, *m_tree->item_at(254, 401) );
}

class QuadTreeBenchmark : public ::testing::Test
{
     
public:

    static void SetUpTestCase()
    {
        m_tree.reset(new QuadTree<int>(512));
        x.resize(iterrations);
        y.resize(iterrations);
    }
    static void TearDownTestCase()
    {
        m_tree.reset();
    }
protected:
    static const size_t iterrations = 1000000;
    static std::vector<size_t> x;
    static std::vector<size_t> y;
    static std::map<int, int> v;
    static std::unique_ptr< QuadTree<int> > m_tree;
};

std::vector<size_t> QuadTreeBenchmark::x;
std::vector<size_t> QuadTreeBenchmark::y;
std::map<int, int> QuadTreeBenchmark::v;
std::unique_ptr< QuadTree<int> > QuadTreeBenchmark::m_tree;

TEST_F(QuadTreeBenchmark, InsertPerformance)
{
    for (unsigned int i = 0; i < iterrations; ++i)
    {
        size_t X= rand() % 512;
        size_t Y = rand() % 512;
        int V = rand() % 512;
        x[i] = X;
        y[i] = Y;
        m_tree->insert(X, Y, V);
    }
}

TEST_F(QuadTreeBenchmark, AccessPerformance)
{
    for (unsigned int i = 0; i < iterrations; ++i)
    {
        m_tree->item_at(x[i], y[i]);
    }
}

TEST_F(QuadTreeBenchmark, RemovePerformance)
{
    for (unsigned int i = 0; i < iterrations; ++i)
    {
        m_tree->remove(x[i], y[i]);
    }
}

TEST_F(QuadTreeBenchmark, QuadTreeCorrectness)
{
    size_t targetX = rand() % 512;
    size_t targetY = rand() % 512;
    int targetValue = rand() % 512;
    m_tree->insert(targetX, targetY, targetValue);

    for (unsigned int i = 0; i < iterrations; ++i)
    {
        size_t X= rand() % 512;
        size_t Y = rand() % 512;
        int V = rand() % 512;
        if (X == targetX && Y == targetY)
        {
            targetValue = V;
        }
        m_tree->insert(X, Y, V);
        ASSERT_EQ(V, *m_tree->item_at(X, Y));
    }

    ASSERT_EQ(targetValue, *m_tree->item_at(targetX, targetY));
}
// eof