#pragma once
#include <list>
#include <memory>

namespace Utils
{
// MACRO!?! ugly, yes, but it help me to save about 5% of performance other than
// encapsulating the code to the function

#define _item_at(on_not_found, finalize)                            \
    Node* node      = & m_root;                                     \
    size_t width    = m_squareSide;                                 \
    while (width > 1)                                               \
    {                                                               \
        width >>= 1;                                                \
        size_t index = (!!(x & width)) + ((!!(y & width)) << 1);    \
        x &= ~width;                                                \
        y &= ~width;                                                \
        if ( !node->quadNodes[index] )                              \
        {                                                           \
            on_not_found;                                           \
        }                                                           \
        node = node->quadNodes[index].get();                        \
    }                                                               \
    finalize;

    // This quad tree is highly limited by it's usage
    // it works with square areas, the side is pow2
    template <class T>
    class QuadTree
    {
    public:
        QuadTree(size_t squareSide) : m_squareSide(squareSide) {}

        void insert(size_t x, size_t y, T value)
        {
            _item_at(node->quadNodes[index].reset(new Node()), node->value = value);
        }

        const T* get_item_at(size_t x, size_t y)
        {
            _item_at(return nullptr, return &node->value);
        }

        T& item(size_t x, size_t y)
        {
            _item_at(node->quadNodes[index].reset(new Node()), return node->value);
        }

        void remove(size_t x, size_t y)
        {
            Node* node      = & m_root;
            size_t width    = m_squareSide;
            Node* lastFull  = & m_root;
            size_t targetIndex    = (!!(x & (m_squareSide >> 1))) + ((!!(y & (m_squareSide >> 1))) << 1);

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
            lastFull->quadNodes[targetIndex].reset();
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