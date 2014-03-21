
//define standard primitive class.

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
        m_desc.TLF = Vector3D(0,0,0);                                               \
        m_desc.Dimentions = Vector3D((x), (y), (z));                                \
        IObjectLibrary::instance()->RegisterPrimitive(*this);                       \
    }                                                                               \
                                                                                    \
    virtual ~class_##PrimitiveType() {};                                            \
                                                                                    \
    static std::unique_ptr<IElement> self;                                          \
};                                                                                  \
std::unique_ptr<IElement> class_##PrimitiveType::self(new class_##PrimitiveType());

DEFINE_PRIMITIVE(Space,     1, 1, 1);
DEFINE_PRIMITIVE(Cube,      1, 1, 1);
DEFINE_PRIMITIVE(Wedge,     1, 1, 1);
DEFINE_PRIMITIVE(Ledder,    1, 1, 1);
DEFINE_PRIMITIVE(Cilinder,  1, 1, 1);
DEFINE_PRIMITIVE(Sphere,    1, 1, 1);

// eof