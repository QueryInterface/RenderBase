#pragma once
#include "common.h"
#include <vector>

struct IMesh;
struct ITexture;
struct IScript;
struct IGameObject;
struct IConstructable;

typedef std::shared_ptr<IMesh> IMeshPtr;
typedef std::shared_ptr<ITexture> ITexturePtr;
typedef std::shared_ptr<IScript> IScriptPtr;
typedef std::shared_ptr<IConstructable> IConstructablePtr;
typedef std::shared_ptr<IGameObject> IGameObjectPtr;

struct IResource
{
};

// resource interfaces
struct IGameObject
    : public IResource
{
    struct ObjectProperties
    {
        std::string    name;
        std::string    meshName;
        std::string    materialName;
        std::string    elementName;
    };

    struct ObjectResources
    {
        IMeshPtr            mesh;
        IConstructablePtr   construction;
    };

    virtual const ObjectProperties& GetObjectContent()   const = 0;
    virtual const ObjectResources&  GetObjectResources() const = 0;
    // lot's of stuff to do here
    // FIXME: remove
    virtual const std::string& GetName() const = 0;

    virtual ~IGameObject() {};
};

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

    typedef ElementDesc<float>      PositionsDesc;
    typedef ElementDesc<float>      TexCoordDesc;
    typedef ElementDesc<float>      NormalsDesc;
    typedef ElementDesc<uint32_t>   IndicesDesc;

    struct Shape
    {
        PositionsDesc   Positions;
        TexCoordDesc    TexCoords;
        NormalsDesc     Normals;
        IndicesDesc     Indices;
        LayoutType      LayoutType;
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
