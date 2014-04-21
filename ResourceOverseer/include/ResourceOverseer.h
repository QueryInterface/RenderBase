#pragma once

struct IMesh;
struct ITexture;
struct ISceleton;
struct IScript;
struct IResourceOverseer;

typedef shared_ptr<IMesh>               IMeshPtr;
typedef shared_ptr<ITexture>            ITexturePtr;
typedef shared_ptr<IScript>             IScriptPtr;
typedef shared_ptr<IResourceOverseer>   IResourceOverseerPtr;


struct IResource : public IClonable 
{
};

// resource interfaces
struct IMesh : public IResource 
{
    // tbd
};

struct ITexture : public IResource 
{
    // tbd
};

struct ISceleton : public IResource {
    // tbd
};

struct IScript : public IResource 
{
};

struct IResourceOverseer 
{
    virtual IMeshPtr            CreateMesh(const wstring& path)                 = 0;
    virtual ITexturePtr         CreateTexture(const wstring& path)              = 0;
    virtual IScriptPtr          CreateScript(const wstring& path)               = 0;
};