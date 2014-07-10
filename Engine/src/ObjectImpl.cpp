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

void Object::SetCenter(const vector3f_t& center)
{
    SetCenterImpl(m_glDesc.MatrixDescGL, center);
}

void Object::SetCenter(float x, float y, float z)
{
    SetCenterImpl(m_glDesc.MatrixDescGL, vector3f_t(x, y, z));
}

void Object::ShiftCenter(const vector3f_t& shift)
{
    ShiftCenterImpl(m_glDesc.MatrixDescGL, shift);
}

void Object::ShiftCenter(float shiftX, float shiftY, float shiftZ)
{
    ShiftCenterImpl(m_glDesc.MatrixDescGL, vector3f_t(shiftX, shiftY, shiftZ));
}

void Object::SetPosition(const vector3f_t& pos) 
{
    SetPositionImpl(m_glDesc.MatrixDescGL, pos);
    // TODO: implement attahment
}

void Object::SetPosition(float x, float y, float z)
{
    SetPositionImpl(m_glDesc.MatrixDescGL, vector3f_t(x, y, z));
}

void Object::Shift(const vector3f_t& pos)
{
    ShiftImpl(m_glDesc.MatrixDescGL, pos);
    // TODO: implement attahment
}

void Object::Shift(float shiftX, float shiftY, float shiftZ)
{
    ShiftImpl(m_glDesc.MatrixDescGL, vector3f_t(shiftX, shiftY, shiftZ));
}

void Object::SetAngle(const vector3f_t& angles)
{
    SetAngleImpl(m_glDesc.MatrixDescGL, angles);
}

void Object::SetAngle(float angleX, float angleY, float angleZ)
{
    SetAngleImpl(m_glDesc.MatrixDescGL, vector3f_t(angleX, angleY, angleZ));
}

void Object::Rotate(const vector3f_t& angles)
{
    RotateImpl(m_glDesc.MatrixDescGL, angles);
}

void Object::Rotate(float angleX, float angleY, float angleZ)
{
    RotateImpl(m_glDesc.MatrixDescGL, vector3f_t(angleX, angleY, angleZ));
}

void Object::SetScale(const vector3f_t& scales)
{
    SetScaleImpl(m_glDesc.MatrixDescGL, scales);
}

void Object::SetScale(float angleX, float angleY, float angleZ)
{
    SetScaleImpl(m_glDesc.MatrixDescGL, vector3f_t(angleX, angleY, angleZ));
}

void Object::Scale(const vector3f_t& scales)
{
    ScaleImpl(m_glDesc.MatrixDescGL, scales);
}

void Object::Scale(float angleX, float angleY, float angleZ)
{
    ScaleImpl(m_glDesc.MatrixDescGL, vector3f_t(angleX, angleY, angleZ));
}

vector3f_t Object::GetAngle() const
{
    return m_angle;
}

vector3f_t Object::GetPosition() const
{
    return m_position;
}

vector3f_t Object::GetCenter() const
{
    return m_center;
}

vector3f_t Object::GetScale() const
{
    return m_scale;
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