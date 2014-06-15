#pragma once
#include "Object.h"
#include <vector>

using std::enable_shared_from_this;
using std::weak_ptr;
using std::vector;

class BasicObject 
    : public IObject
    , public enable_shared_from_this<BasicObject> 
{
public:
    BasicObject(IMeshPtr mesh, ITexturePtr texture);
    virtual ~BasicObject();

    IObjectPtr              Clone() const override;

    virtual void            SetPosition(const vector3f_t& pos) override;
    virtual void            SetPosition(float x, float y, float z) override;

    virtual void            Shift(const vector3f_t& shift) override;
    virtual void            Shift(float xShift, float yShift, float zShift) override;

    virtual vector3f_t        GetPosition() const override;

    virtual void            AttachBidirectional(IObjectPtr object) override;
    virtual void            AttachDirectional(IObjectPtr object) override;
    virtual uint32_t        GetNumAttached() const override;
    virtual IObjectPtr      GetAttached(uint32_t index) const override;
    virtual void            Detach() override;
    virtual void            Detach(IObjectPtr object) override;
private:
    IMeshPtr                    m_mesh;
    ITexturePtr                 m_texture;
    vector3f_t                    m_position;
    vector<IObjectPtr>          m_connections;
    vector< weak_ptr<IObject> > m_connectionsWeak;
    uint32_t                    m_nestedCall;
};