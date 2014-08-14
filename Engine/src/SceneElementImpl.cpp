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
    , m_changed(false)
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
    m_changed = true;
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
    m_changed = true;
}

void SceneElementImpl::ShiftImpl(CoordType type, const vector3f_t& shift)
{
    glm::mat4& m = getMatrix(type);

    m = glm::translate(glm::mat4(1.0), shift) * m;
    m_changed = true;
}

void SceneElementImpl::RotateImpl(CoordType type, const vector3f_t& angles)
{
    glm::quat& q = getQuaternion(type);
    q = glm::quat(angles) * q;
    m_changed = true;
}

void SceneElementImpl::RotateImpl(CoordType type, const quat& qt)
{
    glm::quat& q = getQuaternion(type);
    q = qt * q;
    m_changed = true;
}

const vector3f_t& SceneElementImpl::GetPositionImpl(CoordType type)
{
    updateState();
    switch (type)
    {
    case CoordType::Local:
        {
            return m_localPosition;
        } break;
    case CoordType::Global:
        {
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
    m_changed = true;
}


void SceneElementImpl::EnableVectorUpdate(CoordType type, vector3f_t* v)
{
    switch (type)
    {
    case CoordType::Local:
        m_localUpdateVectors[v] = vector4f_t(*v, 1);
        break;
    case CoordType::Global:
        m_globalUpdateVectors[v] = vector4f_t(*v, 1);
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
}

void SceneElementImpl::DisableVectorUpdate(CoordType type, vector3f_t* v)
{
    switch (type)
    {
    case CoordType::Local:
        m_localUpdateVectors.erase(v);
        break;
    case CoordType::Global:
        m_globalUpdateVectors.erase(v);
        break;
    default:
        VE_ERROR(L"Invalid coord type");
    }
}

void SceneElementImpl::updateState()
{
    if (m_changed)
    {
        glm::mat4& ml = getMatrix(CoordType::Local);
        glm::mat4& mg = getMatrix(CoordType::Global);
        m_localPosition = vector3f_t(ml * m_localPositionInit);
        m_globalPosition = vector3f_t(mg * m_globalPositionInit);
        for (auto& v : m_localUpdateVectors)
        {
            vector3f_t* updateVector = v.first;
            const vector4f_t& initValue = v.second;
            *updateVector = vector3f_t(ml * initValue);
        }
        for (auto& v : m_globalUpdateVectors)
        {
            vector3f_t* updateVector = v.first;
            const vector4f_t& initValue = v.second;
            *updateVector = vector3f_t(mg * initValue);
        }
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
