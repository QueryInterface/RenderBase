#include "SceneElementImpl.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

SceneElementImpl::SceneElementImpl()
    : m_center(0, 0, 0)
    , m_position(0, 0, 0)
    , m_angle(0, 0, 0)
    , m_scale(1, 1, 1)
    , m_centerUpdateMatrix(nullptr)
    , m_positionUpdateMatrix(nullptr)
    , m_angleUpdateMatrix(nullptr)
    , m_scaleUpdateMatrix(nullptr)
{
}

SceneElementImpl::~SceneElementImpl()
{

}

void SceneElementImpl::SetPositionImpl(const vector3f_t& pos)
{
    vector3f_t delta = pos - m_position;
    m_position = pos;
    *m_positionUpdateMatrix = glm::translate(*m_positionUpdateMatrix, delta);
}

void SceneElementImpl::ShiftImpl(const vector3f_t& shift)
{
    m_position += shift;
    *m_positionUpdateMatrix = glm::translate(*m_positionUpdateMatrix, shift);
}

void SceneElementImpl::FixRotationCenterImpl(const vector3f_t& center)
{
    m_center = center;
}

void SceneElementImpl::SetAngleImpl(const vector3f_t& angles)
{
    vector3f_t delta = angles - m_angle;
    m_angle = angles;
    glm::translate(*m_angleUpdateMatrix, -m_center);
    glm::rotate(*m_angleUpdateMatrix, delta.z, glm::vec3(0.0, 0.0, 1.0));
    glm::rotate(*m_angleUpdateMatrix, delta.y, glm::vec3(0.0, 1.0, 0.0));
    glm::rotate(*m_angleUpdateMatrix, delta.x, glm::vec3(1.0, 0.0, 0.0));
    glm::translate(*m_angleUpdateMatrix, m_center);
}

void SceneElementImpl::RotateImpl(const vector3f_t& angles)
{
    m_angle += angles;
    glm::translate(*m_angleUpdateMatrix, -m_center);
    glm::rotate(*m_angleUpdateMatrix, angles.z, glm::vec3(0.0, 0.0, 1.0));
    glm::rotate(*m_angleUpdateMatrix, angles.y, glm::vec3(0.0, 1.0, 0.0));
    glm::rotate(*m_angleUpdateMatrix, angles.x, glm::vec3(1.0, 0.0, 0.0));
    glm::translate(*m_angleUpdateMatrix, m_center);
}

void SceneElementImpl::RotateImpl(const vector3f_t& center, const vector3f_t& angles)
{
    m_angle += angles;
    glm::translate(*m_angleUpdateMatrix, -center);
    glm::rotate(*m_angleUpdateMatrix, angles.z, glm::vec3(0.0, 0.0, 1.0));
    glm::rotate(*m_angleUpdateMatrix, angles.y, glm::vec3(0.0, 1.0, 0.0));
    glm::rotate(*m_angleUpdateMatrix, angles.x, glm::vec3(1.0, 0.0, 0.0));
    glm::translate(*m_angleUpdateMatrix, center);
}

void SceneElementImpl::SetScaleImpl(const vector3f_t& scales)
{
    vector3f_t delta = scales / m_scale ;
    m_scale = scales;
    *m_scaleUpdateMatrix *= glm::scale(glm::mat4(1.0), delta);
}

void SceneElementImpl::ScaleImpl(const vector3f_t& scales)
{
    m_scale *= scales;
    *m_scaleUpdateMatrix *= glm::scale(glm::mat4(1.0), scales);
}