
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
        const NeighborDesc neighbors[(count)] = {

#define END_NEIGHBORS_LIST(count)                                                       \
        };                                                                              \
        m_desc.neighbors.assign(neighbors, neighbors + count);

#define END_PRIMITIVE_DEFINITION(PrimitiveType)                                         \
        ILibrary::library()->RegisterConstruction(*this);                               \
    }                                                                                   \
    virtual ~class_##PrimitiveType() {};                                                \
    static std::unique_ptr<IConstructable> self;                                        \
};                                                                                      \
std::unique_ptr<IConstructable> class_##PrimitiveType::self(new class_##PrimitiveType());

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_PRIMITIVE_DEFINITION(Space,               vector3i_t(0, 0, 0),      vector3i_t(1, 1, 1))
END_PRIMITIVE_DEFINITION(Space);

BEGIN_PRIMITIVE_DEFINITION(Cube,                vector3i_t(0, 0, 0),      vector3i_t(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(1,0,0),  Directions::pX), 
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,1,0),  Directions::pY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,1),  Directions::pZ),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(-1,0,0), Directions::nX),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,-1,0), Directions::nY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,-1), Directions::nZ),
    END_NEIGHBORS_LIST(6);
END_PRIMITIVE_DEFINITION(Cube);

BEGIN_PRIMITIVE_DEFINITION(Wedge,               vector3i_t(0, 0, 0),      vector3i_t(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
        NeighborDesc(5, vector3i_t(1,0,0),                          Directions::pX), 
        NeighborDesc(Influences::NOT_AFFECTED, vector3i_t(0,1,0),   Directions::pY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,1),  Directions::pZ),
        NeighborDesc(5, vector3i_t(-1,0,0),                         Directions::nX),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,-1,0), Directions::nY),
        NeighborDesc(Influences::NOT_AFFECTED, vector3i_t(0,0,-1),  Directions::nZ),
    END_NEIGHBORS_LIST(6);
END_PRIMITIVE_DEFINITION(Wedge);

BEGIN_PRIMITIVE_DEFINITION(Ledder,              vector3i_t(0, 0, 0),      vector3i_t(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
        NeighborDesc(6, vector3i_t(1,0,0),                          Directions::pX), 
        NeighborDesc(Influences::NOT_AFFECTED, vector3i_t(0,1,0),   Directions::pY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,1),  Directions::pZ),
        NeighborDesc(6, vector3i_t(-1,0,0),                         Directions::nX),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,-1,0), Directions::nY),
        NeighborDesc(Influences::NOT_AFFECTED, vector3i_t(0,0,-1),  Directions::nZ),
    END_NEIGHBORS_LIST(6);
END_PRIMITIVE_DEFINITION(Ledder);

BEGIN_PRIMITIVE_DEFINITION(Cilinder,            vector3i_t(0, 0, 0),      vector3i_t(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(1,0,0),  Directions::pX), 
        NeighborDesc(8, vector3i_t(0,1,0),                          Directions::pY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,1),  Directions::pZ),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(-1,0,0), Directions::nX),
        NeighborDesc(8, vector3i_t(0,-1,0),                         Directions::nY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,-1), Directions::nZ),
    END_NEIGHBORS_LIST(6);
END_PRIMITIVE_DEFINITION(Cilinder);

BEGIN_PRIMITIVE_DEFINITION(CilindricPlatform,   vector3i_t(-1, 0, -1),    vector3i_t(2, 1, 2))
    BEGIN_NEIGHBORS_LIST(6)
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(1,0,0),  Directions::pX), 
        NeighborDesc(8, vector3i_t(0,1,0),                          Directions::pY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,1),  Directions::pZ),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(-1,0,0), Directions::nX),
        NeighborDesc(8, vector3i_t(0,-1,0),                         Directions::nY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,-1), Directions::nZ),
    END_NEIGHBORS_LIST(6);
END_PRIMITIVE_DEFINITION(CilindricPlatform);

BEGIN_PRIMITIVE_DEFINITION(Sphere,              vector3i_t(0, 0, 0),      vector3i_t(1, 1, 1))
    BEGIN_NEIGHBORS_LIST(6)
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(1,0,0),  Directions::pX),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,1,0),  Directions::pY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,1),  Directions::pZ),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(-1,0,0), Directions::nX),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,-1,0), Directions::nY),
        NeighborDesc(Influences::FULLY_COVERED, vector3i_t(0,0,-1), Directions::nZ),
    END_NEIGHBORS_LIST(6);
END_PRIMITIVE_DEFINITION(Sphere);
// eof