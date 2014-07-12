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
    m_worldMatrix = glm::translate(glm::mat4(1.0), delta) * m_worldMatrix;
}

void SceneElementImpl::ShiftImpl(const vector3f_t& shift)
{
    m_position += shift;
    m_worldMatrix = glm::translate(glm::mat4(1.0), shift) * m_worldMatrix;
}

void SceneElementImpl::SetCenterImpl(const vector3f_t& center)
{
    m_center = center;
    m_elementMatrix = glm::translate(glm::mat4(1.0), -m_center) * m_elementMatrix;
}

void SceneElementImpl::RotateAroundCenterImpl(const vector3f_t& angles)
{
    m_elementMatrix =   glm::rotate(glm::mat4(1.0), angles.z, glm::vec3(0.0, 0.0, 1.0)) *
                        glm::rotate(glm::mat4(1.0), angles.y, glm::vec3(0.0, 1.0, 0.0)) *
                        glm::rotate(glm::mat4(1.0), angles.x, glm::vec3(1.0, 0.0, 0.0)) * m_elementMatrix;
}

void SceneElementImpl::RotateAroundCenterAxisImpl(const vector3f_t& axis, float angle)
{
    m_elementMatrix = glm::rotate(glm::mat4(1.0), angle, axis) * m_elementMatrix;
}

void SceneElementImpl::RotateAroundPointImpl(const vector3f_t& point, const vector3f_t& angles)
{
    m_worldMatrix = glm::translate(glm::mat4(1.0), point) * 
                    glm::rotate(glm::mat4(1.0), angles.z, glm::vec3(0.0, 0.0, 1.0)) *
                    glm::rotate(glm::mat4(1.0), angles.y, glm::vec3(0.0, 1.0, 0.0)) *
                    glm::rotate(glm::mat4(1.0), angles.x, glm::vec3(1.0, 0.0, 0.0)) *
                    glm::translate(glm::mat4(1.0), -point) * m_worldMatrix;
}

void SceneElementImpl::RotateAroundPointAxisImpl(const vector3f_t& point, const vector3f_t& axis, float angle)
{
    vector3f_t delta = point - m_position;
    m_worldMatrix = glm::translate(glm::mat4(1.0), -delta) * 
                    glm::rotate(glm::mat4(1.0), angle, axis) *
                    glm::translate(glm::mat4(1.0), delta) * m_worldMatrix;
}

void SceneElementImpl::SetScaleImpl(const vector3f_t& scales)
{
    vector3f_t delta = scales / m_scale ;
    m_scale = scales;
    m_elementMatrix = glm::scale(glm::mat4(1.0), delta) * m_elementMatrix;
}

void SceneElementImpl::ScaleImpl(const vector3f_t& scales)
{
    m_scale *= scales;
    m_elementMatrix = glm::scale(glm::mat4(1.0), scales) * m_elementMatrix;
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
