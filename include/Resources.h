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
    enum class LayoutType : unsigned int
    {
        Vertices = 0,
        Normals,
        Texcoord0,
        Texcoord1,
        Tangent,
        Binormal,
    };

    struct LayoutItem
    {
        LayoutType                  layoutType;
        size_t                      itemSize;
        size_t                      itemsCount;
        float*                      items;
    };

    struct IndexGroup
    {
        const uint32_t*         indices;  // might be NULL
        uint32_t                count;
    };

    struct GeometryDesc
    {
        std::vector<LayoutItem>     layout;
        std::vector<IndexGroup>     groups; // can be empty
    };

    virtual void GetGeometryDesc(GeometryDesc& out_descriptor) const = 0;
};

struct ITexture : public IResource 
{
    // tbd
};

struct IScript : public IResource 
{
};
