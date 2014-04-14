#pragma once

#include "common.h"
#include "RenderContext.h"
#include "MathBasics.h"

using std::shared_ptr;
using std::string;
using std::wstring;

struct ISceneElement;
struct ISceneElementHandle;
struct IScene;

struct IMesh;
struct ITexture;
struct IProgram;
struct IScript;
struct IObjectPart;
struct ISimpleObject;
struct IComplexObject;
struct ILight;
struct ICamera;

struct IResourceOverseer;
struct IEngine;

typedef shared_ptr<ISceneElement>       ISceneElementPtr;
typedef shared_ptr<ISceneElementHandle> ISceneElementHandlePtr;
typedef shared_ptr<IScene>              IScenePtr;

typedef shared_ptr<IMesh>               IMeshPtr;
typedef shared_ptr<ITexture>            ITexturePtr;
typedef shared_ptr<IProgram>            IProgramPtr;
typedef shared_ptr<IScript>             IScriptPtr;
typedef shared_ptr<IObjectPart>         IObjectPartPtr;
typedef shared_ptr<ISimpleObject>       ISimpleObjectPtr;
typedef shared_ptr<IComplexObject>      IComplexObjectPtr;
typedef shared_ptr<ILight>              ILightPtr;
typedef shared_ptr<ICamera>             ICameraPtr;

typedef shared_ptr<IResourceOverseer>   IResourceOverseerPtr;
typedef shared_ptr<IEngine>             IEnginePtr;

struct IClonable  
{
    virtual IClonable* Clone() = 0;
};

struct IResource : public IClonable 
{
};

// Specifies elements that could be added to scene graph
// There are a few ways to attach object:
// 1. Bidirectional attachment (rigid connection) - move of one object causes move of another one
// 2. Directiona attachmant - A->B move of object A causes move of object B, move of object B doesn't cause move of object A 
struct ISceneElementHandle
{
    virtual void                AttachBidirectional(ISceneElementHandlePtr sceneElement) = 0;
    virtual void                AttachDirectional(ISceneElementHandlePtr sceneElement)   = 0;
    virtual uint32_t            GetNumAttached() const;
    virtual uint32_t            GetAttached(uint32_t index) const;
    virtual void                Detach();
    virtual void                Detach(ISceneElementHandlePtr sceneElement);
    virtual ISceneElementPtr    GetSceneElement() const;
};

struct ISceneElement 
{
    ISceneElementHandlePtr GetSceneElementHandle();
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

struct IProgram : public IResource 
{
    virtual string  GetVertexShader() const             = 0;
    virtual string  GetPixelShader() const              = 0;
    virtual void    SetVertexShader(const string& path) = 0;
    virtual void    SetPixelShader(const string& path)  = 0;
};

struct IScript : public IResource 
{
};

// object interfaces

struct IObject
    : public IClonable
    , public ISceneElement
{
    // Sets
    virtual void            SetPosition(Vector3<float> pos)     = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const                 = 0;
    virtual IProgramPtr     GetProgram()    = 0;
    virtual IMeshPtr        GetMesh()       = 0;
};

struct IObjectPart : public IObject
{
};

struct ISimpleObject : public IObject
{
};

struct IComplexObject : public IObject
{
    virtual void            AddObjectPart(IObjectPartPtr obj)   = 0;
    virtual size_t          GetObjectPartsCount() const         = 0;
    virtual IObjectPartPtr  GetObjectParts(size_t index) const  = 0;
};

// scene interfaces
struct ILight
    : public IClonable
    , public ISceneElement 
{
    // Sets
    virtual void            SetPosition(Vector3<float> pos) = 0;
    // Gets
    virtual Vector3<float>  GetPosition() const             = 0;    
};

struct ICamera
    : public IClonable
    , public ISceneElement 
{

};

// Base level of all objects
struct IScene
{
    virtual ISceneElementHandlePtr AddSceneElement(ISceneElementPtr obj) = 0;
};

struct IResourceOverseer 
{
    virtual IMeshPtr            CreateMesh(const wstring& path)                 = 0;
    virtual ITexturePtr         CreateTexture(const wstring& path)              = 0;
    virtual IScriptPtr          CreateScript(const wstring& path)               = 0;
    virtual IObjectPartPtr      CreateObjectPart(/*type*/)                      = 0;

    virtual IComplexObject      CreateComplexObject()                           = 0;
    virtual ISimpleObjectPtr    CreateSimpleObject(IMeshPtr mesh, ITexturePtr texture)= 0;
};

struct IEngine : public IHandle 
{
    // Sets
    virtual void                    SetScene(IScenePtr scene)   = 0;
    // Gets
    virtual IResourceOverseerPtr    GetResourceOverseer() const = 0;
    virtual IWindowPtr              GetWindow() const           = 0;
    
    virtual void                    Run()                       = 0;
    static IEnginePtr               Create();
};