#pragma once
#include "Engine.h"
#include "Resources.h"
#include <vector>
#include "glm/glm.hpp"
#include "SDL_opengles2.h"
#include "SceneElementImpl.h"

using std::enable_shared_from_this;
using std::weak_ptr;
using std::vector;


class Object 
    : public IObject
    , protected SceneElementImpl
    , public enable_shared_from_this<Object> 
{
public:
    struct GLMeshDesc
    {
        GLuint      VertexBuffer;
        GLuint      IndexBuffer;
    };
    typedef std::vector<GLMeshDesc> GLMeshDescs;

    Object(IMeshPtr mesh, ITexturePtr texture);
    virtual ~Object();
    // IClonable
    IObjectPtr Clone() const override;
    // IObject
    scene_elements_functions_impl
    scene_elements_gets_impl
    scale_functions_impl
    getscale_impl
    // Object
    virtual const GLMeshDescs& GetMeshDescs() const {return m_meshDescs;}
    virtual const glm::mat4& GetElementMatrix() const {return m_elementMatrix;}
    virtual const glm::mat4& GetWorldMatrix() const {return m_worldMatrix;}

    virtual IMeshPtr        GetMesh() const {return m_mesh;}
    virtual ITexturePtr     GetTexture() const {return m_texture;}

    virtual void            AttachBidirectional(IObjectPtr object) override;
    virtual void            AttachDirectional(IObjectPtr object) override;
    virtual uint32_t        GetNumAttached() const override;
    virtual IObjectPtr      GetAttached(uint32_t index) const override;
    virtual void            Detach() override;
    virtual void            Detach(IObjectPtr object) override;
private:
    void                    processMesh();

    IMeshPtr                m_mesh;
    ITexturePtr             m_texture;
    uint32_t                m_nestedCall;
    GLMeshDescs             m_meshDescs;
};

typedef std::shared_ptr<Object> ObjectPtr;