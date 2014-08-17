#include "SceneElementImpl.h"
#include "ErrorHandler.h"


SceneElementImpl::SceneElementImpl()
    : m_localPositionInit(0, 0, 0, 1)
    , m_globalPositionInit(0, 0, 0, 1)
    , m_localPosition(0, 0, 0)
    , m_globalPosition(0, 0, 0)
    , m_localScale(1, 1, 1)
    , m_globalScale(1, 1, 1)
    , m_localMatrix(glm::mat4(1.0))
    , m_globalMatrix(glm::mat4(1.0))
    , m_localQ(1, 0, 0, 0)
    , m_globalQ(1, 0, 0, 0)
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
    case CoordType::Global:
        m_globalScale = scale;
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

const vector3f_t& SceneElementImpl::GetPositionImpl(CoordType type)
{
    switch (type)
    {
    case CoordType::Local:
        {
            glm::mat4& ml = getMatrix(CoordType::Local);
            m_localPosition = vector3f_t(ml * m_localPositionInit);
            return m_localPosition;
        } break;
    case CoordType::Global:
        {
            glm::mat4& mg = getMatrix(CoordType::Global);
            m_globalPosition = vector3f_t(mg * m_globalPositionInit);
            return m_globalPosition;
        } break;
    default:
        VE_ERROR(L"Invalid coord type");
        return m_globalPosition;
    }
}

vector3f_t SceneElementImpl::GetPositionImpl(CoordType type, const vector3f_t& initPosition)
{
    vector4f_t v = getMatrix(type) * vector4f_t(initPosition, 1);
    return vector3f_t(v);
}

vector3f_t SceneElementImpl::GetDirectionImpl(CoordType type, const vector3f_t& initDirection)
{
    vector4f_t direction = getMatrix(type) * vector4f_t(initDirection, 0);
    return vector3f_t(direction);
}

const vector3f_t& SceneElementImpl::GetScaleImpl(CoordType type)
{
    switch (type)
    {
    case CoordType::Local:
        return m_localScale;
        break;
    case CoordType::Global:
        return m_globalScale;
        break;
    default:
        VE_ERROR(L"Invalid coord type");
        return m_globalScale;
    }
}

void SceneElementImpl::SetPositionInit(CoordType type, const vector3f_t& initPosition)
{
    switch (type)
    {
    case CoordType::Local:
        m_localPositionInit = vector4f_t(initPosition, 1);
        break;
    case CoordType::Global:
         m_globalPositionInit = vector4f_t(initPosition, 1);
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
}

glm::mat4& SceneElementImpl::getMatrix(CoordType type) const
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
    case CoordType::Global:
        {
            glm::quat& q = getQuaternion(type);

            m_globalMatrix = glm::toMat4(q) * m_globalMatrix;
            q = glm::quat(1, 0, 0, 0);
            return m_globalMatrix;
        }
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
    return m_localMatrix;
}

glm::quat& SceneElementImpl::getQuaternion(CoordType type) const
{
    switch (type)
    {
    case CoordType::Local:
        return m_localQ;
        break;
    case CoordType::Global:
        return m_globalQ;
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
    return m_localQ;
}
