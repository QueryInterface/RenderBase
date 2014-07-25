#include "SceneElementImpl.h"
#include "ErrorHandler.h"


SceneElementImpl::SceneElementImpl()
    : m_localPosition(0, 0, 0)
    , m_worldPosition(0, 0, 0)
    , m_localScale(1, 1, 1)
    , m_worldScale(1, 1, 1)
    , m_localMatrix(glm::mat4(1.0))
    , m_worldMatrix(glm::mat4(1.0))
    , m_localQ(1, 0, 0, 0)
    , m_worldQ(1, 0, 0, 0)
{
}

SceneElementImpl::~SceneElementImpl()
{

}

void SceneElementImpl::SetPositionImpl(CoordType type, const vector3f_t& pos)
{
    glm::mat4& m = getMatrix(type);

    vector3f_t delta = pos - GetPositionImpl(type);
    m = glm::translate(glm::mat4(1.0), delta) * m;
}

void SceneElementImpl::SetScaleImpl(CoordType type, const vector3f_t& scale)
{
    glm::mat4& m = getMatrix(type);

    vector3f_t delta = scale / GetScaleImpl(type);
    m = glm::scale(glm::mat4(1.0), delta) * m;
    switch (type)
    {
    case CoordType::Local:
        m_localScale = scale;
        break;
    case CoordType::World:
        m_worldScale = scale;
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
}

void SceneElementImpl::ShiftImpl(CoordType type, const vector3f_t& shift)
{
    glm::mat4& m = getMatrix(type);

    m = glm::translate(glm::mat4(1.0), shift) * m;
}

void SceneElementImpl::RotateImpl(CoordType type, const vector3f_t& angles)
{
    glm::quat& q = getQuaternion(type);
    q = glm::quat(angles) * q;
}

void SceneElementImpl::RotateImpl(CoordType type, const quat& qt)
{
    glm::quat& q = getQuaternion(type);
    q = qt * q;
}

vector3f_t SceneElementImpl::GetPositionImpl()
{
    vector4f_t p(0,0,0,1);
    p = getMatrix(CoordType::World) * getMatrix(CoordType::Local) * p;
    return vector3f_t(p.x, p.y, p.z);
}


vector3f_t SceneElementImpl::GetPositionImpl(CoordType type)
{
    vector4f_t p(0,0,0,1);
    p = getMatrix(type) * p;
    return vector3f_t(p.x, p.y, p.z);
}

vector3f_t SceneElementImpl::GetDirectionImpl(CoordType type, const vector3f_t& initDirection)
{
    vector4f_t direction = getMatrix(type) * vector4f_t(initDirection, 0);
    return vector3f_t(direction.x, direction.y, direction.z);
}

vector3f_t SceneElementImpl::GetScaleImpl(CoordType type)
{
    switch (type)
    {
    case CoordType::Local:
        return m_localScale;
        break;
    case CoordType::World:
        return m_worldScale;
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
    return vector3f_t(1,1,1);
}

glm::mat4& SceneElementImpl::getMatrix(CoordType type)
{
    switch (type)
    {
    case CoordType::Local:
        {
            glm::quat& q = getQuaternion(type);

            m_localMatrix = glm::toMat4(q) * m_localMatrix;
            q = glm::quat(1, 0, 0, 0);
            return m_localMatrix;
        }
        break;
    case CoordType::World:
        {
            glm::quat& q = getQuaternion(type);

            m_worldMatrix = glm::toMat4(q) * m_worldMatrix;
            q = glm::quat(1, 0, 0, 0);
            return m_worldMatrix;
        }
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
    return m_localMatrix;
}

glm::quat& SceneElementImpl::getQuaternion(CoordType type)
{
    switch (type)
    {
    case CoordType::Local:
        return m_localQ;
        break;
    case CoordType::World:
        return m_worldQ;
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
    return m_localQ;
}
