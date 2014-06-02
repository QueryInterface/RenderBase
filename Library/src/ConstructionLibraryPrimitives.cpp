
//define standard primitive class.

#define BEGIN_PRIMITIVE_DEFINITION(PrimitiveType, BBOX_LFT, BBOX_RBB, INFL_PX, INFL_PY, INFL_PZ, INFL_NX, INFL_NY, INFL_NZ)   \
class class_##PrimitiveType : public IConstructable                                     \
{                                                                                       \
public:                                                                                 \
    ConstructionDescription m_desc;                                                     \
    const ConstructionDescription& ConstructionDesc() const {return m_desc;}            \
                                                                                        \
    class_##PrimitiveType()                                                             \
    {                                                                                   \
        m_desc.primitiveUID = ElementType::##PrimitiveType;                             \
        m_desc.LFT = (BBOX_LFT);                                                        \
        m_desc.RBB = (BBOX_RBB);                                                        \
        m_desc.neighborsCount = 6;                                                      \
        m_desc.neighborRelations[0] = INFL_PX;                                          \
        m_desc.neighborRelations[1] = INFL_PY;                                          \
        m_desc.neighborRelations[2] = INFL_PZ;                                          \
        m_desc.neighborRelations[3] = INFL_NX;                                          \
        m_desc.neighborRelations[4] = INFL_NY;                                          \
        m_desc.neighborRelations[5] = INFL_NZ;                                          \
        ILibrary::library()->RegisterConstruction(*this);                               \
    }

#define END_PRIMITIVE_DEFINITION(PrimitiveType)                                         \
    virtual ~class_##PrimitiveType() {};                                                \
    static std::unique_ptr<IConstructable> self;                                        \
};                                                                                      \
std::unique_ptr<IConstructable> class_##PrimitiveType::self(new class_##PrimitiveType());

BEGIN_PRIMITIVE_DEFINITION(Space,               Vector3D(0, 0, 0),      Vector3D(1, 1, 1),
                           Influences::NOT_AFFECTED, Influences::NOT_AFFECTED,
                           Influences::NOT_AFFECTED, Influences::NOT_AFFECTED,
                           Influences::NOT_AFFECTED, Influences::NOT_AFFECTED);
END_PRIMITIVE_DEFINITION(Space);

BEGIN_PRIMITIVE_DEFINITION(Cube,                Vector3D(0, 0, 0),      Vector3D(1, 1, 1),
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED);
END_PRIMITIVE_DEFINITION(Cube);

BEGIN_PRIMITIVE_DEFINITION(Wedge,               Vector3D(0, 0, 0),      Vector3D(1, 1, 1),
                           5, Influences::NOT_AFFECTED, Influences::FULLY_COVERED,
                           5, Influences::FULLY_COVERED, Influences::NOT_AFFECTED);
END_PRIMITIVE_DEFINITION(Wedge);

BEGIN_PRIMITIVE_DEFINITION(Ledder,              Vector3D(0, 0, 0),      Vector3D(1, 1, 1),
                           6, Influences::NOT_AFFECTED, Influences::NOT_AFFECTED, 
                           6, Influences::FULLY_COVERED, Influences::FULLY_COVERED);
END_PRIMITIVE_DEFINITION(Ledder);

BEGIN_PRIMITIVE_DEFINITION(Cilinder,            Vector3D(0, 0, 0),      Vector3D(1, 1, 1),
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED, 8, 
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED, 8);
END_PRIMITIVE_DEFINITION(Cilinder);

BEGIN_PRIMITIVE_DEFINITION(CilindricPlatform,   Vector3D(-1, 0, -1),    Vector3D(2, 1, 2),
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED);
END_PRIMITIVE_DEFINITION(CilindricPlatform);

BEGIN_PRIMITIVE_DEFINITION(Sphere,              Vector3D(0, 0, 0),      Vector3D(1, 1, 1),
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED);
END_PRIMITIVE_DEFINITION(Sphere);
// eof