
#include <vector>

namespace Constructor
{

    enum PrimitiveType
    {
        PT_Space   = 0,
        PT_Cube,

        PT_Wedge,
        PT_Ledder,

        PT_Cilinder,
        PT_Sphere,
    };

    enum EdgeType
    {
        ET_Sharp = 0,
        ET_Bevel,       // vertices only
        ET_Chamfer,     // edge
    };

    enum ElementDirection
    {
        ED_pX = 0,
        ED_nX = 1,
        ED_pY = 2,
        ED_nY = 3,
        ED_pZ = 4,
        ED_nZ = 5,

        ED_ALL
    };

/////////////////////////////////////////////////////////////////////
///
/// Model constructor internalstructure
///
/////////////////////////////////////////////////////////////////////
    struct ConstructionElement
    {
        ElementDirection        direction;
        ConstructionElement*    neighbours[ED_ALL];
        PrimitiveType           primitive;

        ConstructionElement() : direction(ED_pX), primitive(PT_Space)
        {}
    };

/////////////////////////////////////////////////////////////////////
///
/// Model comonent arrays; 
/// Constructor output
///
/////////////////////////////////////////////////////////////////////
    struct Vertex
    {
        float x,y,z;
        Vertex() : x(0), y(0), z(0) {}
    };
    typedef std::vector<Vertex> VertexList;

/////////////////////////////////////////////////////////////////////
///
/// Model constructor object
///
/////////////////////////////////////////////////////////////////////
    class ModelConstructor
    {
    public:
        ModelConstructor() {};
        ~ModelConstructor() {};

        int  GetModel(VertexList& vertices);
        bool SetPrimitive(PrimitiveType type, int x, int y, int z, ElementDirection direction = ED_pX);
    private:
    };

}//end  of namespace constructor

// eof