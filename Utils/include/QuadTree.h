#pragma once
#include <list>
#include <memory>

namespace Utils
{
    // This quad tree is highly limited by it's usage
    // it works with square areas, the side is pow2

    template <class T>
    class QuadTree
    {
    public:
        QuadTree(size_t squareSide) : m_squareSide(squareSide) {}

        void insert(size_t x, size_t y, T value)
        {
            Node* node      = & m_root;
            size_t width    = m_squareSide;

            while (width > 1)
            {
                width >>= 1;

                // !!(A) convert any non-null number to 1
                size_t index = (!!(x & width)) + ((!!(y & width)) << 1);
                x &= ~width;
                y &= ~width;

                if ( !node->quadNodes[index] )
                {
                    node->quadNodes[index].reset(new Node());
                }
                node = node->quadNodes[index].get();
            }
            node->value = value;
        }

        const T* item_at(size_t x, size_t y)
        {
            Node* node      = & m_root;
            size_t width    = m_squareSide;

            while (width > 1)
            {
                width >>= 1;

                // !!(A) convert any non-null number to 1
                size_t index = (!!(x & width)) + ((!!(y & width)) << 1);
                x &= ~width;
                y &= ~width;

                if ( !node->quadNodes[index] )
                {
                    return nullptr;
                }
                node = node->quadNodes[index].get();
            }
            return &node->value;
        }

        void remove(size_t x, size_t y)
        {
            Node* node      = & m_root;
            size_t width    = m_squareSide;
            Node* lastFull  = & m_root;
            size_t targetIndex    = (!!(x & (width >> 1))) + ((!!(y & (width >> 1))) << 1);

            while (width > 1)
            {
                width >>= 1;

                // !!(A) convert any non-null number to 1
                size_t index = (!!(x & width)) + ((!!(y & width)) << 1);
                x &= ~width;
                y &= ~width;

                if ( !node->quadNodes[index] )
                {
                    return;
                }
                //Get number of non null nodes
                if (((!!node->quadNodes[0]) + (!!node->quadNodes[1]) + (!!node->quadNodes[2]) + (!!node->quadNodes[3])) > 1)
                {
                    lastFull = node;
                    targetIndex = index;
                }
                node = node->quadNodes[index].get();
                
            }
            std::unique_ptr<Node> toRemove(std::move(lastFull->quadNodes[targetIndex]));
            toRemove.reset();
        }
    private:
        struct Node
        {
            // | 0 | 1 |
            // | 2 | 3 |
            std::unique_ptr<Node> quadNodes[4];
            T value;
        };

        size_t m_squareSide;
        Node   m_root;
    };

}
// eof