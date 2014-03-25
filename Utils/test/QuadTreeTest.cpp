#include "QuadTree.h"
#include <gtest/gtest.h>
#include <memory>

using namespace Utils;

class QuadTreeTest : public ::testing::Test
{
public:

    void SetUp()
    {
        m_tree.reset(new QuadTree<int>());
    }
    void TearDown()
    {
        m_tree.reset();
    }
protected:
    std::unique_ptr< QuadTree<int> > m_tree;
};


// eof