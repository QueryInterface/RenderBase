#pragma once
#include "MathBasics.h"

class SceneElementImpl
{
public:
    SceneElementImpl();
    // Virtual destructor to prevent direct object creation
    virtual ~SceneElementImpl() = 0;

    void        SetOnCenterUpdateMatrix(glm::mat4* m);
    void        SetOnPositionUpdateMatrix(glm::mat4* m);
    void        SetOnAngleUpdateMatrix(glm::mat4* m);
    void        SetOnScaleUpdateMatrix(glm::mat4* m);


    void        SetPositionImpl(const vector3f_t& pos);
    void        ShiftImpl(const vector3f_t& shift);

    void        FixRotationCenterImpl(const vector3f_t& center);
    void        SetAngleImpl(const vector3f_t& angles);
    void        RotateImpl(const vector3f_t& angles);

    void        RotateImpl(const vector3f_t& center, const vector3f_t& angles);

    void        SetScaleImpl(const vector3f_t& scales);
    void        ScaleImpl(const vector3f_t& scales);

protected:
    vector3f_t  m_center;
    vector3f_t  m_position;
    vector3f_t  m_angle;
    vector3f_t  m_scale;

    glm::mat4*  m_centerUpdateMatrix;
    glm::mat4*  m_positionUpdateMatrix;
    glm::mat4*  m_angleUpdateMatrix;
    glm::mat4*  m_scaleUpdateMatrix;
};

#define position_update_functions                                                                                                                   \
    virtual void            SetPosition(const vector3f_t& pos) override {SetPositionImpl(pos);}                                                     \
    virtual void            Shift(const vector3f_t& shift) override {ShiftImpl(shift);}                                                             \
    virtual vector3f_t      GetPosition() const override {return m_position;}

#define angle_update_functions                                                                                                                      \
    virtual void            FixRotationCenter(const vector3f_t& center) override {FixRotationCenterImpl(center);}                                   \
    virtual void            SetAngle(const vector3f_t& angles) override {SetAngleImpl(angles);}                                                     \
    virtual void            Rotate(const vector3f_t& angles) override {RotateImpl(angles);}                                                         \
    virtual vector3f_t      GetAngle() const override {return m_angle;}

#define angle_update_functions_ex                                                                                                                   \
    virtual void            Rotate(const vector3f_t& center, const vector3f_t& angles) override {RotateImpl(center, angles);}                       \

#define scale_update_functions                                                                                                                      \
    virtual void            SetScale(const vector3f_t& scales) override {SetScaleImpl(scales);}                                                     \
    virtual void            Scale(const vector3f_t& scales) override {ScaleImpl(scales);}                                                           \
    virtual vector3f_t      GetScale(return m_scale;) const override;

     
