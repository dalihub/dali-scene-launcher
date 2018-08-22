#ifndef NODE3D_H
#define NODE3D_H

/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "Vector3.h"
#include "Vector2.h"

#include <assimp/scene.h>

#include <vector>

using namespace std;

class Node3D
{
    public:
        Node3D(Node3D *eParent, bool hasMesh);
        virtual ~Node3D();
        bool IsMatrixIdentity();

        void SetMatrix(aiMatrix4x4 eMat);
        unsigned int index;
        string m_Name;
        Node3D *m_Parent;
        bool m_HasMesh;
        float m_Matrix[16];

        vector<Node3D*> m_Children;
///////////////////////////////////////////////////
        vector<Vector3> m_Positions;
        vector<Vector3> m_Normals;
        vector<Vector3> m_Tangents;
        vector<Vector2> m_Textures;
        vector<unsigned short> m_Indices;
    protected:

    private:
};

#endif // NODE3D_H
