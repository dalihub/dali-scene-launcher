#ifndef MESH_H
#define MESH_H

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <vector>

class Mesh
{
public:
    std::vector<Vector3> m_Positions;
    std::vector<Vector3> m_Normals;
    std::vector<Vector3> m_Tangents;
    std::vector<Vector2> m_Textures;
    std::vector<unsigned short> m_Indices;

    // These buffers will be empty when there's no skinning information.
    std::vector<Vector4> m_BoneIds; // indices into the nodes array of the scene; -1s are invalid and should be accompanied by a weight of .0.
    std::vector<Vector4> m_BoneWeights;
};

#endif //MESH_H
