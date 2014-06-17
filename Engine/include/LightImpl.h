#pragma once
#include "Engine.h"

class Light : public ILight
{
public:
    Light();
    virtual ~Light();
    virtual ILightPtr       Clone() const;
    // Sets
    virtual void            SetPosition(vector3f_t pos);
    virtual void            SetPosition(float x, float y, float z);
    // Gets
    virtual vector3f_t        GetPosition() const;    
private:
    vector3f_t m_position;
};