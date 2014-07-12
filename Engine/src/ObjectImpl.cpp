#include <algorithm>
#include "ObjectImpl.h"
#include "HandleImpl.h"
#include "ErrorHandler.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

using std::static_pointer_cast;

#define NESTED_LOCK() if (m_nestedCall) return; ++m_nestedCall;
#define NESTED_UNLOCK() --m_nestedCall;

Object::Object(IMeshPtr mesh, ITexturePtr texture)
    : m_mesh(mesh)
    , m_texture(texture)
    , m_nestedCall(0)
{
    processMesh();
}

Object::~Object()
{
    for (auto& desc : m_meshDescs)
    {
        if (desc.VertexBuffer)
        {
            glDeleteBuffers(1, &desc.VertexBuffer);
            desc.VertexBuffer = 0;
        }
        if (desc.IndexBuffer)
        {
            glDeleteBuffers(1, &desc.IndexBuffer);
            desc.IndexBuffer = 0;
        }
    }
}

IObjectPtr Object::Clone() const
{
    CLONE_HANDLE(IObject, Object);
}

void Object::AttachBidirectional(IObjectPtr /*object*/) 
{
    // TODO: implement
}

void Object::AttachDirectional(IObjectPtr /*object*/) 
{
    // TODO: implement
}

uint32_t Object::GetNumAttached() const 
{
    // TODO: implement
    return 0;
}

IObjectPtr Object::GetAttached(uint32_t /*index*/) const 
{
    // TODO: implement
    return nullptr;
}

void Object::Detach() 
{
    // TODO: implement
}

void Object::Detach(IObjectPtr /*object*/) 
{
    // TODO: implement
}

IObjectPtr IObject::CreateObject(IMeshPtr mesh, ITexturePtr texture)
{
    return static_pointer_cast<IObject>(make_shared_handle<Object>(mesh, texture));
}

void Object::processMesh()
{
    const IMesh::Desc& meshDesc = GetMesh()->GetDesc();
    if (m_meshDescs.size() != meshDesc.Shapes.size())
        m_meshDescs.resize(meshDesc.Shapes.size());
    for (uint32_t s = 0; s < meshDesc.Shapes.size(); ++s)
    {
        // Delete old buffers
        if (m_meshDescs[s].VertexBuffer)
        {
            glDeleteBuffers(1, &m_meshDescs[s].VertexBuffer);
            m_meshDescs[s].VertexBuffer = 0;
        }
        if (m_meshDescs[s].IndexBuffer)
        {
            glDeleteBuffers(1, &m_meshDescs[s].IndexBuffer);
            m_meshDescs[s].IndexBuffer = 0;
        }
        // Init new data
        auto& shape = meshDesc.Shapes[s];
        uint32_t posDataSize = shape.Positions.Data.size() * sizeof(shape.Positions.Data[0]);
        GL_CALL(glGenBuffers(1, &m_meshDescs[s].VertexBuffer));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_meshDescs[s].VertexBuffer));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, posDataSize, shape.Positions.Data.data(), GL_STATIC_DRAW));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        if (shape.Indices.Data.size())
        {
            uint32_t indDataSize = shape.Indices.Data.size() * sizeof(shape.Indices.Data[0]);
            GL_CALL(glGenBuffers(1, &m_meshDescs[s].IndexBuffer));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshDescs[s].IndexBuffer));
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indDataSize, shape.Indices.Data.data(), GL_STATIC_DRAW));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }
    }
    //// Bind vertex tex coodinates
    //glGenBuffers(1, &g_VBOTexCoords);
    //glBindBuffer(GL_ARRAY_BUFFER, g_VBOTexCoords);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_pTexCoord), g_pTexCoord, GL_STATIC_DRAW);
    //glVertexAttribPointer(g_AtribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}