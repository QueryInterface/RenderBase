#pragma once

#include "common.h"
#include "RenderContext.h"
using std::shared_ptr;
using std::string;
using std::wstring;

struct IResource;
struct IMesh;
struct ITexture;
struct IProgram;
struct IScript;
struct IObject;
struct ICompositeObject;
struct ILight;
struct IScene;
struct IResourceOverseer;
struct IEngine;

typedef shared_ptr<IMesh>               IMeshPtr;
typedef shared_ptr<ITexture>            ITexturePtr;
typedef shared_ptr<IProgram>            IProgramPtr;
typedef shared_ptr<IScript>             IScriptPtr;
typedef shared_ptr<IObject>             IObjectPtr;
typedef shared_ptr<ICompositeObject>    ICompositeObjectPtr;
typedef shared_ptr<IScene>              IScenePtr;
typedef shared_ptr<ILight>              ILightPtr;
typedef shared_ptr<IResourceOverseer>   IResourceOverseerPtr;
typedef shared_ptr<IEngine>             IEnginePtr;

struct IResource : public IHandle {
};

struct IMesh : public IResource {
};

struct ITexture : public IResource {
};

struct IProgram : public IResource {
    virtual string  GetVertexShader() const             = 0;
    virtual string  GetPixelShader() const              = 0;
    virtual void    SetVertexShader(const string& path) = 0;
    virtual void    SetPixelShader(const string& path)  = 0;
};

struct IScript : public IResource {
};

struct IObject : public IHandle {
    // Sets
    virtual void            SetPosition(Vector3<float> pos) = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const             = 0;
    virtual IProgramPtr     GetProgram()                    = 0;
};

struct ICompositeObject : IObject {
    virtual void AddObject(IObjectPtr obj) = 0;
};

struct ILight : public IHandle {
    // Sets
    virtual void            SetPosition(Vector3<float> pos) = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const             = 0;    
};

struct IScene : public IHandle {
    virtual void AddObject(IObjectPtr obj) = 0;
    virtual void AddLight(ILightPrt light) = 0;
};

struct IResourceOverseer {
    virtual IMeshPtr        CreateMesh(const wstring& path)                 = 0;
    virtual ITexturePtr     CreateTexture(const wstring& path)              = 0;
    virtual IScriptPtr      CreateScript(const wstring& path)               = 0;

    virtual IObjectPtr      CreateObject(IMeshPtr mesh, ITexturePtr texture)= 0;
};

struct IEngine : public IHandle {
    // Sets
    virtual void                    SetScene(IScenePtr scene)   = 0;
    // Gets
    virtual IResourceOverseerPtr    GetResourceOverseer() const = 0;
    virtual IWindowPtr              GetWindow() const           = 0;
    
    virtual void                    Run()                       = 0;
    static IEnginePtr               Create();
};