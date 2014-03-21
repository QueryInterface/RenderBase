//ET_Space
#define DEFINE_PRIMITIVE(PrimitiveType, x, y, z)                                    \
class class_##PrimitiveType : public IElement                                       \
{                                                                                   \
public:                                                                             \
    ElementDescription m_desc;                                                      \
    const ElementDescription& GetObjectDescription() const {return m_desc;}         \
                                                                                    \
    class_##PrimitiveType()                                                         \
    {                                                                               \
        m_desc.primitiveUID = ET_##PrimitiveType;                                   \
        IObjectLibrary::instance()->RegisterPrimitive(*this);                       \
    }                                                                               \
                                                                                    \
    virtual ~class_##PrimitiveType() {};                                            \
                                                                                    \
    static std::unique_ptr<IElement> self;                                          \
};                                                                                  \
std::unique_ptr<IElement> class_##PrimitiveType::self(new class_##PrimitiveType());

DEFINE_PRIMITIVE(Space, 1, 1, 1);
DEFINE_PRIMITIVE(Cube,  1, 1, 1);
DEFINE_PRIMITIVE(Wedge, 1, 1, 1);
// eof