#pragma once
#include "common.h"
#include <vector>

struct IMesh;
struct ITexture;
struct IScript;

typedef std::shared_ptr<IMesh> IMeshPtr;
typedef std::shared_ptr<ITexture> ITexturePtr;
typedef std::shared_ptr<IScript> IScriptPtr;

struct IResource
{
};

// resource interfaces
struct IMesh 
    : public IResource
    , public IHandle
    , public IClonable<IMeshPtr> 
{
    struct LayoutItem
    {
        unsigned int    size;
        unsigned int    stride;
        unsigned int    offset;
    };

    struct MeshComponent
    {
        const float*          geometry;
        const unsigned int*   indices;  // might be NULL
        unsigned int          count;
    };

    struct GeometryDesc
    {
        std::vector<LayoutItem>     layout;
        std::vector<MeshComponent>  groups;
    };

    virtual void GetGeometryDesc(unsigned int flags, GeometryDesc& out_descriptor) const = 0;
};

struct ITexture : public IResource 
{
    // tbd
};

struct IScript : public IResource 
{
};
