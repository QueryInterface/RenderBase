#pragma once
#include <set>

template <typename T>
class Pool
{
public:
    Pool() {}
    ~Pool() {}
    void Push(T* object)
    {
        m_objects.insert(object);
    }
    T* Pop()
    {
        if (!m_objects.empty())
        {
            auto iter = m_objects.begin();
            T* object = *iter;
            m_objects.erase(iter);
            return object;
        }
        return nullptr;
    }
    uint32_t Size() const
    {
        return m_objects.size();
    }
private:
    std::set<T*> m_objects;
};