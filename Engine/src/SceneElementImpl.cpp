#include "SceneElementImpl.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

SceneElementImpl::SceneElementImpl()
    : m_center(0, 0, 0)
    , m_position(0, 0, 0)
    , m_angle(0, 0, 0)
    , m_scale(1, 1, 1)
{
}

SceneElementImpl::~SceneElementImpl()
{

}

void SceneElementImpl::SetCenterImpl(GLMatrixDesc& out, const vector3f_t& center)
{
    vector3f_t delta = m_center - center;
    m_center = center;
    out.ObjectMatrix =  glm::translate(out.ObjectMatrix, delta);
}

void SceneElementImpl::ShiftCenterImpl(GLMatrixDesc& out, const vector3f_t& shift)
{
    m_center += shift;
    out.ObjectMatrix =  glm::translate(out.ObjectMatrix, shift);
}

void SceneElementImpl::SetPositionImpl(GLMatrixDesc& out, const vector3f_t& pos)
{
    vector3f_t delta = pos - m_position;
    m_position = pos;
    out.WorldMatrix = glm::translate(out.WorldMatrix, delta);
}

void SceneElementImpl::ShiftImpl(GLMatrixDesc& out, const vector3f_t& shift)
{
    m_position += shift;
    out.WorldMatrix = glm::translate(out.WorldMatrix, shift);
}

void SceneElementImpl::SetAngleImpl(GLMatrixDesc& out, const vector3f_t& angles)
{
    vector3f_t delta = angles - m_angle;
    m_angle = angles;
    out.ObjectMatrix = glm::rotate(glm::mat4(1.0f), delta.x, glm::vec3(1.0, 0.0, 0.0))  *
					   glm::rotate(glm::mat4(1.0f), delta.y, glm::vec3(0.0, 1.0, 0.0)) *
					   glm::rotate(glm::mat4(1.0f), delta.z, glm::vec3(0.0, 0.0, 1.0)) * out.ObjectMatrix;
}

void SceneElementImpl::RotateImpl(GLMatrixDesc& out, const vector3f_t& angles)
{
    m_angle += angles;
    out.ObjectMatrix = glm::rotate(glm::mat4(1.0f), angles.x, glm::vec3(1.0, 0.0, 0.0))  *
					   glm::rotate(glm::mat4(1.0f), angles.y, glm::vec3(0.0, 1.0, 0.0)) *
					   glm::rotate(glm::mat4(1.0f), angles.z, glm::vec3(0.0, 0.0, 1.0)) * out.ObjectMatrix;
}

void SceneElementImpl::SetScaleImpl(GLMatrixDesc& out, const vector3f_t& scales)
{
    vector3f_t delta = scales / m_scale ;
    m_scale = scales;
    out.ObjectMatrix *= glm::scale(glm::mat4(1.0), delta);
}

void SceneElementImpl::ScaleImpl(GLMatrixDesc& out, const vector3f_t& scales)
{
    m_scale *= scales;
    out.ObjectMatrix *= glm::scale(glm::mat4(1.0), scales);
}