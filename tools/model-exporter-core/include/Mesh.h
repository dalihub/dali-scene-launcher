#ifndef MESH_H
#define MESH_H

#include "Vector2.h"
#include "Vector3.h"
#include <vector>
#include <map>

class Mesh
{
public:
    std::vector<Vector3> m_Positions;
    std::vector<Vector3> m_Normals;
    std::vector<Vector3> m_Tangents;
    std::vector<Vector2> m_Textures;
    std::vector<unsigned short> m_Indices;

    using SkinnedVertex = std::pair<unsigned int, float>;
    std::map<unsigned int, std::vector<SkinnedVertex>> m_SkinnedVertices;
};

#endif //MESH_H
