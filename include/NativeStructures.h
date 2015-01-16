/////////////////////////////////////////////////////////////////////////////////
//
// C++ representation of Scriptable structures
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <internal/ScriptBase.h>

structure Vector
{
    float x;
    float y;
    float z;
};

structure ObjectProperties
{
    std::string name;
    std::string meshName;
    std::string materialName;
    std::string elementName;
};

structure PlacementParameters
{
    std::string name;
    Vector position;
    int orientation;
    int placeDirection;
};