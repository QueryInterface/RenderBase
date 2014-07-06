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
    enum class LayoutType
    {
        Triangle,
        Strip,
        Fan,
        Points
    };

    template <typename Type>
    struct ElementDesc
    {
        std::vector<Type>   Data;
        uint32_t            ElementSize;
    };

    struct PositionsDesc : public ElementDesc<float>
    {
        LayoutType  LayoutType;
    };

    typedef ElementDesc<float>      TexCoordDesc;
    typedef ElementDesc<float>      NormalsDesc;
    typedef ElementDesc<uint32_t>   IndicesDesc;

    struct Shape
    {
        PositionsDesc   Positions;
        TexCoordDesc    TexCoords;
        NormalsDesc     Normals;
        IndicesDesc     Indices;
    };

    struct Desc
    {
        std::vector<Shape> Shapes;
    };

    virtual const Desc& GetDesc() const = 0;
};

struct ITexture : public IResource 
{
    // tbd
};

struct IScript : public IResource 
{
};
