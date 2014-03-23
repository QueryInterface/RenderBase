
//define standard primitive class.

#define BEGIN_PRIMITIVE_DEFINITION(PrimitiveType, x, y, z)                          \
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
    }

#define END_PRIMITIVE_DEFINITION(PrimitiveType)                                     \
                                                                                    \
    virtual ~class_##PrimitiveType() {};                                            \
                                                                                    \
    static std::unique_ptr<IElement> self;                                          \
};                                                                                  \
std::unique_ptr<IElement> class_##PrimitiveType::self(new class_##PrimitiveType());

BEGIN_PRIMITIVE_DEFINITION(Space,               1, 1, 1);
END_PRIMITIVE_DEFINITION(Space);

BEGIN_PRIMITIVE_DEFINITION(Cube,                1, 1, 1);
END_PRIMITIVE_DEFINITION(Cube);

BEGIN_PRIMITIVE_DEFINITION(Wedge,               1, 1, 1);
END_PRIMITIVE_DEFINITION(Wedge);

BEGIN_PRIMITIVE_DEFINITION(Ledder,              1, 1, 1);
END_PRIMITIVE_DEFINITION(Ledder);

BEGIN_PRIMITIVE_DEFINITION(Cilinder,            1, 1, 1);
END_PRIMITIVE_DEFINITION(Cilinder);

BEGIN_PRIMITIVE_DEFINITION(CilindricPlatform,   3, 3, 1);
END_PRIMITIVE_DEFINITION(CilindricPlatform);

BEGIN_PRIMITIVE_DEFINITION(Sphere,              1, 1, 1);
END_PRIMITIVE_DEFINITION(Sphere);
// eof