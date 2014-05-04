#include "ObjectImpl.h"
#include <algorithm>

using std::static_pointer_cast;

ComplexObject::ComplexObject()
    : m_position(0., 0., 0.)
{

}

ComplexObject::~ComplexObject()
{

}

IObjectPtr ComplexObject::Clone() const
{
    std::shared_ptr<ComplexObject> obj = std::make_shared<ComplexObject>();
    *obj = *this;
    return std::static_pointer_cast<IObject>(obj);
}

void ComplexObject::SetPosition(Vector3<float> pos) 
{
    m_position = pos;
}

Vector3<float> ComplexObject::GetPosition() const 
{
    return m_position;
}

void ComplexObject::AttachBidirectional(IObjectPtr object) 
{
    object->AttachDirectional(static_pointer_cast<IObject>(shared_from_this()));
    m_connectionsWeak.push_back(object);
}

void ComplexObject::AttachDirectional(IObjectPtr object) 
{
    m_connections.push_back(object);
}

uint32_t ComplexObject::GetNumAttached() const 
{
    return m_connections.size() + m_connectionsWeak.size();
}

IObjectPtr ComplexObject::GetAttached(uint32_t index) const 
{
    size_t size0 = m_connections.size();
    size_t size1 = size0 + m_connectionsWeak.size();
    if (index >= size1)
        return nullptr;
    else if (index > size0)
        return m_connectionsWeak[index % size0].lock();
    else
        return m_connections[index];
}

void ComplexObject::Detach() 
{
    vector<IObjectPtr> temp;
    temp.swap(m_connections);
    vector< weak_ptr<IObject> > tempWeak;
    tempWeak.swap(m_connectionsWeak);
    for (auto obj : tempWeak)
    {
        IObjectPtr obj_ptr = obj.lock();
        if (obj_ptr)
        {
            obj_ptr->Detach(shared_from_this());
        }
    }
}

void ComplexObject::Detach(IObjectPtr object) 
{
    auto elem0 = std::find(m_connections.begin(), m_connections.end(), object);
    if (elem0 != m_connections.end())
    {
        uint32_t index0 = std::distance(m_connections.begin(), elem0);
        m_connections[index0] = m_connections.back();
        m_connections.resize(m_connections.size()-1);
    }

    auto elem1 = std::find_if(m_connectionsWeak.begin(), m_connectionsWeak.end(), [&](weak_ptr<IObject>& k){
        return k.lock() == object;
    });
    if (elem1 != m_connectionsWeak.end())
    {
        uint32_t index1 = std::distance(m_connectionsWeak.begin(), elem1);
        m_connectionsWeak[index1] = m_connectionsWeak.back();
        m_connectionsWeak.resize(m_connectionsWeak.size()-1);
    }
}

