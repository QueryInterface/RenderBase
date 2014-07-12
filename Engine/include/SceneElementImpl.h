#pragma once
#include "MathBasics.h"

class SceneElementImpl
{
public:
    SceneElementImpl();
    // Virtual destructor to prevent direct object creation
    virtual ~SceneElementImpl() = 0;

    void        SetPositionImpl(const vector3f_t& pos);
    void        ShiftImpl(const vector3f_t& shift);

    void        SetCenterImpl(const vector3f_t& center) ;
    void        RotateAroundCenterImpl(const vector3f_t& angles);
    void        RotateAroundCenterAxisImpl(const vector3f_t& axis, float angle);

    void        RotateAroundPointImpl(const vector3f_t& point, const vector3f_t& angles);
    void        RotateAroundPointAxisImpl(const vector3f_t& point, const vector3f_t& axis, float angle);

    void        SetScaleImpl(const vector3f_t& scales);
    void        ScaleImpl(const vector3f_t& scales);

    vector3f_t  GetCenterImpl() const;
    vector3f_t  GetPositionImpl() const;
    vector3f_t  GetAngleImpl() const;
    vector3f_t  GetScaleImpl() const;

protected:
    vector3f_t  m_center;
    vector3f_t  m_position;
    vector3f_t  m_scale;

    glm::mat4   m_elementMatrix;
    glm::mat4   m_worldMatrix;
};

#define setposition_impl            virtual void SetPosition(const vector3f_t& pos) override {SetPositionImpl(pos);}
#define shift_impl                  virtual void Shift(const vector3f_t& shift) override {ShiftImpl(shift);}
#define setcenter_impl              virtual void SetCenter(const vector3f_t& center) override {SetCenterImpl(center);}
#define rotatearoundcenter_impl     virtual void RotateAroundCenter(const vector3f_t& angles) override {RotateAroundCenterImpl(angles);}
#define rotatearoundcenteraxis_impl virtual void RotateAroundCenterAxis(const vector3f_t& axis, float angle) override {RotateAroundCenterAxisImpl(axis, angle);}
#define rotatearoundpoint_impl      virtual void RotateAroundPoint(const vector3f_t& point, const vector3f_t& angles) override {RotateAroundPointImpl(point, angles);}
#define rotatearoundpointaxis_impl  virtual void RotateAroundPointAxis(const vector3f_t& point, const vector3f_t& axis, float angle) override {RotateAroundPointAxisImpl(point, axis, angle);}
#define setscale_impl               virtual void SetScale(const vector3f_t& scales) override {SetScaleImpl(scales);}
#define scale_impl                  virtual void Scale(const vector3f_t& scales) override {ScaleImpl(scales);}

#define getcenter_impl              virtual vector3f_t GetCenter() const override {return GetCenterImpl();} 
#define getposition_impl            virtual vector3f_t GetPosition() const override {return GetPositionImpl();} 
#define getangle_impl               virtual vector3f_t GetAngle() const override {return GetAngleImpl();}
#define getscale_impl               virtual vector3f_t GetScale() const override {return GetScaleImpl();}

#define position_functions_impl \
    setposition_impl \
    shift_impl

#define center_rotate_functions_impl \
    setcenter_impl \
    rotatearoundcenter_impl \
    rotatearoundcenteraxis_impl \

#define rotate_functions_impl \
    rotatearoundpoint_impl \
    rotatearoundpointaxis_impl

#define scale_functions_impl \
    setscale_impl \
    scale_impl

#define scene_elements_functions_impl \
    position_functions_impl \
    center_rotate_functions_impl \
    rotate_functions_impl \

#define scene_elements_gets_impl \
    getcenter_impl \
    getposition_impl \
    getangle_impl
