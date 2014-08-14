#pragma once
#include "MathBasics.h"
#include "Engine.h"

class SceneElementImpl
{
public:
    const glm::mat4&    GetMatrix(CoordType type) const {return getMatrix(type);}
protected:
    SceneElementImpl();
    // Virtual destructor to prevent direct object creation
    virtual ~SceneElementImpl() = 0;

    void        SetPositionImpl(CoordType type, const vector3f_t& pos);
    void        SetScaleImpl(CoordType type, const vector3f_t& scale);
    void        ShiftImpl(CoordType type, const vector3f_t& shift);
    void        RotateImpl(CoordType type, const vector3f_t& angles);
    void        RotateImpl(CoordType type, const quat& q);

    vector3f_t  GetPositionImpl();
    vector3f_t  GetPositionImpl(CoordType type);
    vector3f_t  GetDirectionImpl(CoordType type, const vector3f_t& initDirection);
    vector3f_t  GetScaleImpl(CoordType type);

    bool        HasChanged(bool reset = false);
private:
    glm::mat4&  getMatrix(CoordType type) const;
    glm::quat&  getQuaternion(CoordType type) const;

    vector3f_t  m_localPosition;
    vector3f_t  m_worldPosition;
    vector3f_t  m_localScale;
    vector3f_t  m_worldScale;

    mutable glm::mat4 m_localMatrix;
    mutable glm::mat4 m_worldMatrix;

    mutable glm::quat   m_localQ;
    mutable glm::quat   m_worldQ;

    bool                m_changed;
};

#define setposition_impl            virtual void SetPosition(CoordType type, const vector3f_t& pos) override {SetPositionImpl(type, pos);}
#define setscale_impl               virtual void SetScale(CoordType type, const vector3f_t& scales) override {SetScaleImpl(type, scales);}
#define shift_impl                  virtual void Shift(CoordType type, const vector3f_t& shift) override {ShiftImpl(type, shift);}
#define rotate_impl                 virtual void Rotate(CoordType type, const vector3f_t& angles) override {RotateImpl(type, angles);}
#define rotate2_impl                virtual void Rotate(CoordType type, const quat& q) override {RotateImpl(type, q);}

#define getposition_impl            virtual vector3f_t GetPosition() override {return GetPositionImpl();} 
#define getposition2_impl           virtual vector3f_t GetPosition(CoordType type) override {return GetPositionImpl(type);} 
#define getdirection_impl           virtual vector3f_t GetDirection(CoordType type, const vector3f_t& initDirection) override {return GetDirectionImpl(type, initDirection);}
#define getscale_impl               virtual vector3f_t GetScale(CoordType type) override {return GetScaleImpl(type);}

#define scene_elements_functions_impl \
    setposition_impl \
    setscale_impl \
    shift_impl \
    rotate_impl \
    rotate2_impl

#define scene_elements_gets_impl \
    getposition_impl \
    getposition2_impl \
    getdirection_impl \
    getscale_impl

#define scene_elements_impl \
    scene_elements_functions_impl \
    scene_elements_gets_impl
