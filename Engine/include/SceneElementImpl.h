#pragma once
#include "MathBasics.h"
#include "Engine.h"
#include <map>

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

    const vector3f_t& GetPositionImpl(CoordType type);
    vector3f_t  GetPositionImpl(CoordType type, const vector3f_t& initPosition);
    vector3f_t  GetDirectionImpl(CoordType type, const vector3f_t& initDirection);
    const vector3f_t& GetScaleImpl(CoordType type);

    // Functions to manipulate with internal state of SceneElementImpl

    // Sets starting position of object without matrix modifications
    // Should be used really carefully and only once during derivered class constructor
    void        SetPositionInit(CoordType type, const vector3f_t& initPosition);
    // SceneElementImpl will update registered vectors whenever its internal state has changed.
    // Value of 'v' during registration is taken as initial for multiplication my internal matrices
    void        EnableVectorUpdate(CoordType type, vector3f_t* v);
    // Disable vector update
    void        DisableVectorUpdate(CoordType type, vector3f_t* v);
private:
    // Updates internal position variables
    void        updateState();

    glm::mat4&  getMatrix(CoordType type) const;
    glm::quat&  getQuaternion(CoordType type) const;

    vector4f_t  m_localPositionInit;
    vector4f_t  m_globalPositionInit;

    vector3f_t  m_localPosition;
    vector3f_t  m_globalPosition;
    vector3f_t  m_localScale;
    vector3f_t  m_globalScale;

    mutable glm::mat4   m_localMatrix;
    mutable glm::mat4   m_globalMatrix;

    mutable glm::quat   m_localQ;
    mutable glm::quat   m_globalQ;

    bool                m_changed;

    std::map<vector3f_t*, vector4f_t> m_localUpdateVectors;     // pair: update vector <-> initial value
    std::map<vector3f_t*, vector4f_t> m_globalUpdateVectors;    // pair: update vector <-> initial value
};

#define setposition_impl            virtual void SetPosition(CoordType type, const vector3f_t& pos) override {SetPositionImpl(type, pos);}
#define setscale_impl               virtual void SetScale(CoordType type, const vector3f_t& scales) override {SetScaleImpl(type, scales);}
#define shift_impl                  virtual void Shift(CoordType type, const vector3f_t& shift) override {ShiftImpl(type, shift);}
#define rotate_impl                 virtual void Rotate(CoordType type, const vector3f_t& angles) override {RotateImpl(type, angles);}
#define rotate2_impl                virtual void Rotate(CoordType type, const quat& q) override {RotateImpl(type, q);}

#define getposition_impl            virtual const vector3f_t& GetPosition(CoordType type) override {return GetPositionImpl(type);} 
#define getposition2_impl           virtual       vector3f_t  GetPosition(CoordType type, const vector3f_t& initPosition) override {return GetPositionImpl(type, initPosition);} 
#define getdirection_impl           virtual       vector3f_t  GetDirection(CoordType type, const vector3f_t& initDirection) override {return GetDirectionImpl(type, initDirection);}
#define getscale_impl               virtual const vector3f_t& GetScale(CoordType type) override {return GetScaleImpl(type);}

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
