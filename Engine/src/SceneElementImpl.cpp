#include "SceneElementImpl.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

SceneElementImpl::SceneElementImpl()
    : m_center(0, 0, 0)
    , m_position(0, 0, 0)
    , m_scale(1, 1, 1)
    , m_elementMatrix(glm::mat4(1.0))
    , m_worldMatrix(glm::mat4(1.0))
{
}

SceneElementImpl::~SceneElementImpl()
{

}

void SceneElementImpl::SetPositionImpl(const vector3f_t& pos)
{
    vector3f_t delta = pos - m_position;
    m_position = pos;
    glm::translate(m_worldMatrix, delta);
}

void SceneElementImpl::ShiftImpl(const vector3f_t& shift)
{
    m_position += shift;
    glm::translate(m_worldMatrix, shift);
}

void SceneElementImpl::SetCenterImpl(const vector3f_t& center)
{
    m_center = center;
    glm::translate(m_elementMatrix, -m_center);
}

void SceneElementImpl::RotateAroundCenterImpl(const vector3f_t& angles)
{
    glm::rotate(m_elementMatrix, angles.z, glm::vec3(0.0, 0.0, 1.0));
    glm::rotate(m_elementMatrix, angles.y, glm::vec3(0.0, 1.0, 0.0));
    glm::rotate(m_elementMatrix, angles.x, glm::vec3(1.0, 0.0, 0.0));
}

void SceneElementImpl::RotateAroundCenterAxisImpl(const vector3f_t& axis, float angle)
{
    glm::rotate(m_elementMatrix, angle, axis);
}

void SceneElementImpl::RotateAroundPointImpl(const vector3f_t& point, const vector3f_t& angles)
{
    vector3f_t delta = point - m_position;
    glm::translate(m_worldMatrix, delta);
    glm::rotate(m_worldMatrix, angles.z, glm::vec3(0.0, 0.0, 1.0));
    glm::rotate(m_worldMatrix, angles.y, glm::vec3(0.0, 1.0, 0.0));
    glm::rotate(m_worldMatrix, angles.x, glm::vec3(1.0, 0.0, 0.0));
    glm::translate(m_worldMatrix, -delta);
}

void SceneElementImpl::RotateAroundPointAxisImpl(const vector3f_t& point, const vector3f_t& axis, float angle)
{
    vector3f_t delta = point - m_position;
    glm::translate(m_worldMatrix, delta);
    glm::rotate(m_worldMatrix, angle, axis);
    glm::translate(m_worldMatrix, -delta);
}

void SceneElementImpl::SetScaleImpl(const vector3f_t& scales)
{
    vector3f_t delta = scales / m_scale ;
    m_scale = scales;
    glm::scale(m_elementMatrix, delta);
}

void SceneElementImpl::ScaleImpl(const vector3f_t& scales)
{
    m_scale *= scales;
    glm::scale(m_elementMatrix, scales);
}

vector3f_t SceneElementImpl::GetCenterImpl() const
{
    return m_center;
}

vector3f_t SceneElementImpl::GetPositionImpl() const
{
    return m_position;
}

vector3f_t SceneElementImpl::GetAngleImpl() const
{
    return glm::eulerAngles(glm::quat(m_worldMatrix * m_elementMatrix));
}

vector3f_t SceneElementImpl::GetScaleImpl() const
{
    return m_scale;
}
