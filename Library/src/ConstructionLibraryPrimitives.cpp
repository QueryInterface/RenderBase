
//define standard primitive class.

#define BEGIN_PRIMITIVE_DEFINITION(PrimitiveType, BBOX_LFT, BBOX_RBB)                   \
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
        m_desc.RBB = (BBOX_RBB);

#define BEGIN_NEIGHBORS_LIST(count)                                                     \
        m_desc.neighborsCount = (count);                                                \
        const unsigned int neighbors[(count)] = {

#define END_NEIGHBORS_LIST()                                                            \
        };                                                                              \
        m_desc.neighborRelations.assign(neighbors, neighbors + m_desc.neighborsCount);

#define BEGIN_NEIGHBORS_DIRECTIONS(count)                                               \
        const Vector3D neighborDirections[(count)] = {

#define END_NEIGHBORS_DIRECTIONS()                                                      \
        };                                                                              \
        m_desc.neighborDirections.assign(neighborDirections, neighborDirections + m_desc.neighborsCount);

#define END_PRIMITIVE_DEFINITION(PrimitiveType)                                         \
        ILibrary::library()->RegisterConstruction(*this);                               \
    }                                                                                   \
    virtual ~class_##PrimitiveType() {};                                                \
    static std::unique_ptr<IConstructable> self;                                        \
};                                                                                      \
std::unique_ptr<IConstructable> class_##PrimitiveType::self(new class_##PrimitiveType());

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_PRIMITIVE_DEFINITION(Space,               Vector3D(0, 0, 0),      Vector3D(1, 1, 1))
END_PRIMITIVE_DEFINITION(Space);

BEGIN_PRIMITIVE_DEFINITION(Cube,                Vector3D(0, 0, 0),      Vector3D(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
        Influences::FULLY_COVERED, Influences::FULLY_COVERED,
        Influences::FULLY_COVERED, Influences::FULLY_COVERED,
        Influences::FULLY_COVERED, Influences::FULLY_COVERED,
    END_NEIGHBORS_LIST();
    BEGIN_NEIGHBORS_DIRECTIONS(6)
        Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
    END_NEIGHBORS_DIRECTIONS()
END_PRIMITIVE_DEFINITION(Cube);

BEGIN_PRIMITIVE_DEFINITION(Wedge,               Vector3D(0, 0, 0),      Vector3D(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
                           5, Influences::NOT_AFFECTED, Influences::FULLY_COVERED,
                           5, Influences::FULLY_COVERED, Influences::NOT_AFFECTED
    END_NEIGHBORS_LIST()
    BEGIN_NEIGHBORS_DIRECTIONS(6)
        Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
    END_NEIGHBORS_DIRECTIONS()
END_PRIMITIVE_DEFINITION(Wedge);

BEGIN_PRIMITIVE_DEFINITION(Ledder,              Vector3D(0, 0, 0),      Vector3D(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
                           6, Influences::NOT_AFFECTED, Influences::NOT_AFFECTED, 
                           6, Influences::FULLY_COVERED, Influences::FULLY_COVERED
    END_NEIGHBORS_LIST()
    BEGIN_NEIGHBORS_DIRECTIONS(6)
        Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
    END_NEIGHBORS_DIRECTIONS()
END_PRIMITIVE_DEFINITION(Ledder);

BEGIN_PRIMITIVE_DEFINITION(Cilinder,            Vector3D(0, 0, 0),      Vector3D(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED, 8, 
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED, 8
    END_NEIGHBORS_LIST()
    BEGIN_NEIGHBORS_DIRECTIONS(6)
        Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
    END_NEIGHBORS_DIRECTIONS()
END_PRIMITIVE_DEFINITION(Cilinder);

BEGIN_PRIMITIVE_DEFINITION(CilindricPlatform,   Vector3D(-1, 0, -1),    Vector3D(2, 1, 2))
    BEGIN_NEIGHBORS_LIST(6)
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED
    END_NEIGHBORS_LIST()
    BEGIN_NEIGHBORS_DIRECTIONS(6)
        Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
    END_NEIGHBORS_DIRECTIONS()
END_PRIMITIVE_DEFINITION(CilindricPlatform);

BEGIN_PRIMITIVE_DEFINITION(Sphere,              Vector3D(0, 0, 0),      Vector3D(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED,
                           Influences::FULLY_COVERED, Influences::FULLY_COVERED
    END_NEIGHBORS_LIST()
    BEGIN_NEIGHBORS_DIRECTIONS(6)
        Vector3D(1,0,0), Vector3D(0,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), Vector3D(0,-1,0), Vector3D(0,0,-1)
    END_NEIGHBORS_DIRECTIONS()
END_PRIMITIVE_DEFINITION(Sphere);
// eof