#pragma once
#include <list>
#include <vector>
#include <memory>

namespace Utils
{
    template <class T>
    class range_list
    {
    public:
        range_list(size_t size) : m_size(size) 
        {
        }
        ~range_list() {}

        size_t segments_count() {return m_segments.size();}

        T* get_item_at(size_t index) 
        {
            auto segment = m_segments.begin();
            for (; segment != m_segments.end(); ++segment)
            {
                if (segment->start <= index && (index - segment->start) < segment->items.size())
                {
                    return &segment->items[index - segment->start];
                }
            }
            return nullptr; 
        }

        void insert(size_t index, T value) 
        {
            auto segment = m_segments.begin();
            for (; segment != m_segments.end(); ++segment)
            {
                size_t local_index = index - segment->start;
                if (segment->start <= index && local_index <= segment->items.size())
                {
                    if (local_index == segment->items.size())
                    {
                        segment->items.push_back(value);
                    }
                    else 
                        segment->items[local_index] = value;
                    return;
                }
            }
            segment_desc desc;
            desc.start = index;
            desc.items.push_back(value);
            m_segments.push_back(desc);
        }

    private:
        size_t m_size;
        struct segment_desc
        {
            size_t start;
            std::vector<T> items;
        };

        std::list<segment_desc> m_segments;
    };
}
// eof