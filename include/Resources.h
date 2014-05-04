#pragma once
#include "common.h"

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

    typedef std::vector<LayoutItem> LayoutData_t;
    typedef std::vector<float>      VertexData_t;
    typedef std::vector<short>      IndexData_t;

    virtual const LayoutData_t& GetLayout()     const = 0;
    virtual const VertexData_t& GetMeshBuffer() const = 0;
    virtual void GetIndexData(unsigned int, IndexData_t&) const = 0; 

};

struct ITexture : public IResource 
{
    // tbd
};

struct IScript : public IResource 
{
};
