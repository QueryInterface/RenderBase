#pragma once

#include "common.h"
#include "RenderContext.h"
using std::shared_ptr;
using std::string;
using std::wstring;

struct IClonable;
struct ISceneElement;
struct IResource;
struct IMesh;
struct ITexture;
struct IProgram;
struct IScript;
struct IObjectPart;
struct IObject;
struct ILight;
struct IScene;
struct IResourceOverseer;
struct IEngine;

typedef shared_ptr<ISceneElement>        ISceneElementPtr;
typedef shared_ptr<IMesh>               IMeshPtr;
typedef shared_ptr<ITexture>            ITexturePtr;
typedef shared_ptr<IProgram>            IProgramPtr;
typedef shared_ptr<IScript>             IScriptPtr;
typedef shared_ptr<IObjectPart>         IObjectPartPtr;
typedef shared_ptr<IObject>             IObjectPtr;
typedef shared_ptr<IScene>              IScenePtr;
typedef shared_ptr<ILight>              ILightPtr;
typedef shared_ptr<IResourceOverseer>   IResourceOverseerPtr;
typedef shared_ptr<IEngine>             IEnginePtr;

struct IClonable  {
    virtual IClonable* Clone() = 0;
};

// Specifies elements that could be added to scene graph
struct ISceneElement {
};

// resource interfaces
struct IResource : public IClonable {
};

struct IMesh : public IResource {
    // tbd
};

struct ITexture : public IResource {
    // tbd
};

struct ISceleton : public IResource {
    // tbd
};

struct IProgram : public IResource {
    virtual string  GetVertexShader() const             = 0;
    virtual string  GetPixelShader() const              = 0;
    virtual void    SetVertexShader(const string& path) = 0;
    virtual void    SetPixelShader(const string& path)  = 0;
};

struct IScript : public IResource {
};

struct ICamera : public ISceneElement {

};

// object interfaces
struct IObjectPart 
    : public IClonable
    , public ISceneElement
{
    virtual IProgramPtr     GetProgram()                    = 0;
    virtual IMeshPtr        GetMesh() = 0;
};

struct IObject 
    : public IClonable
    , public ISceneElement
{
    // Sets
    virtual void            SetPosition(Vector3<float> pos)     = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const                 = 0;
    
    virtual void            AddObjectPart(IObjectPartPtr obj)   = 0;
    virtual size_t          GetObjectPartsCount() const         = 0;
    virtual IObjectPartPtr  GetObjectParts(size_t index) const  = 0;
};

// scene interfaces
struct ILight 
    : public IClonable
    , public ISceneElement {
    // Sets
    virtual void            SetPosition(Vector3<float> pos) = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const             = 0;    
};

struct IScene {
    virtual void AddSceneObject(ISceneElementPtr obj) = 0;
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