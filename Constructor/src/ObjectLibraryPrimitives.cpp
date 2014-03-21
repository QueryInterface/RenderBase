//ET_Space
class SpacePrimitive : public IElement
{
public:
    ElementDescription m_desc;
    const ElementDescription& GetObjectDescription() const {return m_desc;}

    SpacePrimitive()
    {
        m_desc.primitiveUID = ET_Space;
        IObjectLibrary::instance()->RegisterElement(*this);
    }

    virtual ~SpacePrimitive() {};

    static std::unique_ptr<IElement> self;
};
std::unique_ptr<IElement> SpacePrimitive::self(new SpacePrimitive());
// eof