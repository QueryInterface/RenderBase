
//define standard primitive class.

#define BEGIN_PRIMITIVE_DEFINITION(PrimitiveType, BBOX_TLF, BBOX_BRB)               \
class class_##PrimitiveType : public IElement                                       \
{                                                                                   \
public:                                                                             \
    ElementDescription m_desc;                                                      \
    const ElementDescription& GetObjectDescription() const {return m_desc;}         \
                                                                                    \
    class_##PrimitiveType()                                                         \
    {                                                                               \
        m_desc.primitiveUID = ET_##PrimitiveType;                                   \
        m_desc.TLF = (BBOX_TLF);                                                    \
        m_desc.BRB = (BBOX_BRB);                                                    \
        IObjectLibrary::instance()->RegisterPrimitive(*this);                       \
    }

#define END_PRIMITIVE_DEFINITION(PrimitiveType)                                     \
                                                                                    \
    virtual ~class_##PrimitiveType() {};                                            \
    static std::unique_ptr<IElement> self;                                          \
};                                                                                  \
std::unique_ptr<IElement> class_##PrimitiveType::self(new class_##PrimitiveType());

BEGIN_PRIMITIVE_DEFINITION(Space,               Vector3D(0, 0, 0),      Vector3D(1, 1, 1));
END_PRIMITIVE_DEFINITION(Space);

BEGIN_PRIMITIVE_DEFINITION(Cube,                Vector3D(0, 0, 0),      Vector3D(1, 1, 1));
END_PRIMITIVE_DEFINITION(Cube);

BEGIN_PRIMITIVE_DEFINITION(Wedge,               Vector3D(0, 0, 0),      Vector3D(1, 1, 1));
END_PRIMITIVE_DEFINITION(Wedge);

BEGIN_PRIMITIVE_DEFINITION(Ledder,              Vector3D(0, 0, 0),      Vector3D(1, 1, 1));
END_PRIMITIVE_DEFINITION(Ledder);

BEGIN_PRIMITIVE_DEFINITION(Cilinder,            Vector3D(0, 0, 0),      Vector3D(1, 1, 1));
END_PRIMITIVE_DEFINITION(Cilinder);

BEGIN_PRIMITIVE_DEFINITION(CilindricPlatform,   Vector3D(-1, 0, -1),    Vector3D(2, 1, 2));
END_PRIMITIVE_DEFINITION(CilindricPlatform);

BEGIN_PRIMITIVE_DEFINITION(Sphere,              Vector3D(0, 0, 0),      Vector3D(1, 1, 1));
END_PRIMITIVE_DEFINITION(Sphere);
// eof