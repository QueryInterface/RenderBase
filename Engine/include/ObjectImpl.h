#pragma once
#include "Object.h"
#include <vector>
#include "glm/glm.hpp"
#include "SDL_opengles2.h"

using std::enable_shared_from_this;
using std::weak_ptr;
using std::vector;


class Object 
    : public IObject
    , public enable_shared_from_this<Object> 
{
public:
    struct GLMeshDesc
    {
        GLuint      VertexBuffer;
        GLuint      IndexBuffer;
    };

    struct GLDesc
    {
        std::vector<GLMeshDesc> MeshDescGL;
        glm::mat4               ObjectMatrix;
        glm::mat4               WorldMatrix;
    };
public:
    Object(IMeshPtr mesh, ITexturePtr texture);
    virtual ~Object();
    // Object
    const GLDesc&           GetGLDesc() const;

    // IObject
    IObjectPtr              Clone() const override;

    virtual void            SetCenter(const vector3f_t& m_center) override;
    virtual void            SetCenter(float x, float y, float z) override;
    virtual void            ShiftCenter(const vector3f_t& shift) override;
    virtual void            ShiftCenter(float shiftX, float shiftY, float shiftZ) override;

    virtual void            SetPosition(const vector3f_t& pos) override;
    virtual void            SetPosition(float x, float y, float z) override;
    virtual void            Shift(const vector3f_t& shift) override;
    virtual void            Shift(float shiftX, float shiftY, float shiftZ) override;

    virtual void            SetAngle(const vector3f_t& angles) override;
    virtual void            SetAngle(float angleX, float angleY, float angleZ) override;
    virtual void            Rotate(const vector3f_t& angles) override;
    virtual void            Rotate(float angleX, float angleY, float angleZ) override;

    virtual void            SetScale(const vector3f_t& scales) override;
    virtual void            SetScale(float angleX, float angleY, float angleZ) override;
    virtual void            Scale(const vector3f_t& scales) override;
    virtual void            Scale(float angleX, float angleY, float angleZ) override;

    virtual vector3f_t      GetPosition() const override;

    virtual IMeshPtr        GetMesh() const;
    virtual ITexturePtr     GetTexture() const;

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
    GLDesc                  m_glDesc;

    vector3f_t              m_center;
    vector3f_t              m_position;
    vector3f_t              m_angle;
    vector3f_t              m_scale;
};

typedef std::shared_ptr<Object> ObjectPtr;