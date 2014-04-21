#pragma once
#include "Object.h"
#include <vector>

using std::enable_shared_from_this;
using std::weak_ptr;
using std::vector;

class ComplexObject 
    : public IObject
    , private enable_shared_from_this<ComplexObject> 
{
public:
    ComplexObject();
    virtual ~ComplexObject();
    virtual void            SetPosition(Vector3<float> pos) override;
    virtual Vector3<float>  GetPosition() const override;

    virtual void            AttachBidirectional(IObjectPtr object) override;
    virtual void            AttachDirectional(IObjectPtr object) override;
    virtual uint32_t        GetNumAttached() const override;
    virtual IObjectPtr      GetAttached(uint32_t index) const override;
    virtual void            Detach() override;
    virtual void            Detach(IObjectPtr object) override;
private:
    Vector3<float>              m_position;
    vector<IObjectPtr>          m_connections;
    vector< weak_ptr<IObject> > m_connectionsWeak;
};