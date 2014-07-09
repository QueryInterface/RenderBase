#include <algorithm>
#include "ObjectImpl.h"
#include "HandleImpl.h"

using std::static_pointer_cast;

#define NESTED_LOCK() if (m_nestedCall) return; ++m_nestedCall;
#define NESTED_UNLOCK() --m_nestedCall;

Object::Object(IMeshPtr mesh, ITexturePtr texture)
    : m_mesh(mesh)
    , m_texture(texture)
    , m_position(0., 0., 0.)
    , m_nestedCall(0)
{

}

Object::~Object()
{

}

IObjectPtr Object::Clone() const
{
    CLONE_HANDLE(IObject, Object);
}

void Object::SetPosition(const vector3f_t& pos) 
{
    NESTED_LOCK();
    m_position = pos;
    for (auto c : m_connections)
    {
        c->SetPosition(pos);
    }
    for (auto c : m_connectionsWeak)
    {
        IObjectPtr weakObject = c.lock();
        if (weakObject)
            weakObject->SetPosition(pos);
    }
    NESTED_UNLOCK();
}

void Object::SetPosition(float x, float y, float z)
{
    SetPosition(vector3f_t(x, y, z));
}

void Object::Shift(const vector3f_t& pos)
{
    NESTED_LOCK();
    if (m_nestedCall) return;
    ++m_nestedCall;

    m_position += pos;
    for (auto c : m_connections)
    {
        c->Shift(pos);
    }
    for (auto c : m_connectionsWeak)
    {
        IObjectPtr weakObject = c.lock();
        if (weakObject)
            weakObject->Shift(pos);
    }
    NESTED_UNLOCK();
}

void Object::Shift(float xShift, float yShift, float zShift)
{
    Shift(vector3f_t(xShift, yShift, zShift));
}

vector3f_t Object::GetPosition() const 
{
    return m_position;
}

IMeshPtr Object::GetMesh() const
{
    return m_mesh;
}

ITexturePtr Object::GetTexture() const
{
    return m_texture;
}

void Object::AttachBidirectional(IObjectPtr object) 
{
    object->AttachDirectional(static_pointer_cast<IObject>(shared_from_this()));
    m_connectionsWeak.push_back(object);
}

void Object::AttachDirectional(IObjectPtr object) 
{
    m_connections.push_back(object);
}

uint32_t Object::GetNumAttached() const 
{
    return m_connections.size() + m_connectionsWeak.size();
}

IObjectPtr Object::GetAttached(uint32_t index) const 
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

void Object::Detach() 
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

void Object::Detach(IObjectPtr object) 
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

IObjectPtr IObject::CreateObject(IMeshPtr mesh, ITexturePtr texture)
{
    return static_pointer_cast<IObject>(make_shared_handle<Object>(mesh, texture));
}