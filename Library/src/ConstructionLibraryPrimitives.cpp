
//define standard primitive class.

#define BEGIN_PRIMITIVE_DEFINITION(PrimitiveType, BBOX_LFT, BBOX_RBB)               \
class class_##PrimitiveType : public IConstructable                                 \
{                                                                                   \
public:                                                                             \
    ConstructionDescription m_desc;                                                 \
    const ConstructionDescription& ConstructionDesc() const {return m_desc;}        \
                                                                                    \
    class_##PrimitiveType()                                                         \
    {                                                                               \
        m_desc.primitiveUID = ElementType::##PrimitiveType;                         \
        m_desc.LFT = (BBOX_LFT);                                                    \
        m_desc.RBB = (BBOX_RBB);                                                    \
        ILibrary::library()->RegisterConstruction(*this);                           \
    }

#define END_PRIMITIVE_DEFINITION(PrimitiveType)                                     \
    virtual ~class_##PrimitiveType() {};                                            \
    static std::unique_ptr<IConstructable> self;                                    \
};                                                                                  \
std::unique_ptr<IConstructable> class_##PrimitiveType::self(new class_##PrimitiveType());

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