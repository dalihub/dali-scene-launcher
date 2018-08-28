#ifndef MESH_H
#define MESH_H

#include "Vector2.h"
#include "Vector3.h"
#include <vector>

class Mesh
{
public:
    std::vector<Vector3> m_Positions;
    std::vector<Vector3> m_Normals;
    std::vector<Vector3> m_Tangents;
    std::vector<Vector2> m_Textures;
    std::vector<unsigned short> m_Indices;
};

#endif //MESH_H
