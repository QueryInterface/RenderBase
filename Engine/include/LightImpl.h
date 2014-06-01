#pragma once
#include "Engine.h"

class Light : public ILight
{
public:
    Light();
    virtual ~Light();
    virtual ILightPtr       Clone() const;
    // Sets
    virtual void            SetPosition(vector3d pos);
    virtual void            SetPosition(float x, float y, float z);
    // Gets
    virtual vector3d        GetPosition() const;    
private:
    vector3d m_position;
};