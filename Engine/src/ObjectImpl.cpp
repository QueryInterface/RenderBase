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
    , m_position(0, 0, 0)
    , m_angle(0, 0, 0)
    , m_scale(1, 1, 1)
{
    processMesh();
}

Object::~Object()
{
    for (auto& desc : m_glDesc.MeshDescGL)
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

void Object::SetPosition(const vector3f_t& pos) 
{
    vector3f_t delta = pos - m_position;
    m_position = pos;
    m_glDesc.ObjectMatrix = glm::translate(m_glDesc.ObjectMatrix, delta);
    // TODO: implement attahment
}

void Object::SetPosition(float x, float y, float z)
{
    SetPosition(vector3f_t(x, y, z));
}

void Object::Shift(const vector3f_t& pos)
{
    m_position += pos;
    m_glDesc.ObjectMatrix = glm::translate(m_glDesc.ObjectMatrix, pos);
    // TODO: implement attahment
}

void Object::Shift(float shiftX, float shiftY, float shiftZ)
{
    Shift(vector3f_t(shiftX, shiftY, shiftZ));
}

void Object::SetAngle(const vector3f_t& angles)
{
    vector3f_t delta = angles - m_angle;
    m_angle = angles;
    m_glDesc.ObjectMatrix *= glm::rotate(glm::mat4(1.0f), delta.x, glm::vec3(1.0, 0.0, 0.0)) *
					         glm::rotate(glm::mat4(1.0f), delta.y, glm::vec3(0.0, 1.0, 0.0)) *
					         glm::rotate(glm::mat4(1.0f), delta.z, glm::vec3(0.0, 0.0, 1.0));
}

void Object::SetAngle(float angleX, float angleY, float angleZ)
{
    SetAngle(vector3f_t(angleX, angleY, angleZ));
}

void Object::Rotate(const vector3f_t& angles)
{
    m_angle += angles;
    m_glDesc.ObjectMatrix *= glm::rotate(glm::mat4(1.0f), angles.x, glm::vec3(1.0, 0.0, 0.0)) *
					         glm::rotate(glm::mat4(1.0f), angles.y, glm::vec3(0.0, 1.0, 0.0)) *
					         glm::rotate(glm::mat4(1.0f), angles.z, glm::vec3(0.0, 0.0, 1.0));
}

void Object::Rotate(float angleX, float angleY, float angleZ)
{
    Rotate(vector3f_t(angleX, angleY, angleZ));
}

void Object::SetScale(const vector3f_t& scales)
{
    vector3f_t delta = m_scale / scales;
    m_glDesc.ObjectMatrix *= glm::scale(glm::mat4(1.0), delta);
}

void Object::SetScale(float angleX, float angleY, float angleZ)
{
    SetScale(vector3f_t(angleX, angleY, angleZ));
}

void Object::Scale(const vector3f_t& scales)
{
    m_scale *= scales;
    m_glDesc.ObjectMatrix *= glm::scale(glm::mat4(1.0), scales);
}

void Object::Scale(float angleX, float angleY, float angleZ)
{
    Scale(vector3f_t(angleX, angleY, angleZ));
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

const Object::GLDesc& Object::GetGLDesc() const
{
    return m_glDesc;
}

IObjectPtr IObject::CreateObject(IMeshPtr mesh, ITexturePtr texture)
{
    return static_pointer_cast<IObject>(make_shared_handle<Object>(mesh, texture));
}

void Object::processMesh()
{
    const IMesh::Desc& meshDesc = GetMesh()->GetDesc();
    if (m_glDesc.MeshDescGL.size() != meshDesc.Shapes.size())
        m_glDesc.MeshDescGL.resize(meshDesc.Shapes.size());
    for (uint32_t s = 0; s < meshDesc.Shapes.size(); ++s)
    {
        // Delete old buffers
        if (m_glDesc.MeshDescGL[s].VertexBuffer)
        {
            glDeleteBuffers(1, &m_glDesc.MeshDescGL[s].VertexBuffer);
            m_glDesc.MeshDescGL[s].VertexBuffer = 0;
        }
        if (m_glDesc.MeshDescGL[s].IndexBuffer)
        {
            glDeleteBuffers(1, &m_glDesc.MeshDescGL[s].IndexBuffer);
            m_glDesc.MeshDescGL[s].IndexBuffer = 0;
        }
        // Init new data
        auto& shape = meshDesc.Shapes[s];
        uint32_t posDataSize = shape.Positions.Data.size() * sizeof(shape.Positions.Data[0]);
        GL_CALL(glGenBuffers(1, &m_glDesc.MeshDescGL[s].VertexBuffer));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_glDesc.MeshDescGL[s].VertexBuffer));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, posDataSize, shape.Positions.Data.data(), GL_STATIC_DRAW));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        if (shape.Indices.Data.size())
        {
            uint32_t indDataSize = shape.Indices.Data.size() * sizeof(shape.Indices.Data[0]);
            GL_CALL(glGenBuffers(1, &m_glDesc.MeshDescGL[s].IndexBuffer));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glDesc.MeshDescGL[s].IndexBuffer));
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