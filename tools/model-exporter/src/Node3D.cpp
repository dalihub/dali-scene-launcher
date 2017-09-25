/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

#include "Node3D.h"

Node3D::Node3D(Node3D *eParent, bool hasMesh)
{
    m_Parent = eParent;
    m_HasMesh = hasMesh;
    index = -1;
}

Node3D::~Node3D()
{
    //dtor
}


void Node3D::SetMatrix(aiMatrix4x4 eMat)
{
    m_Matrix[0] = eMat.a1;
    m_Matrix[1] = eMat.b1;
    m_Matrix[2] = eMat.c1;
    m_Matrix[3] = eMat.d1;

    m_Matrix[4] = eMat.a2;
    m_Matrix[5] = eMat.b2;
    m_Matrix[6] = eMat.c2;
    m_Matrix[7] = eMat.d2;

    m_Matrix[8] = eMat.a3;
    m_Matrix[9] = eMat.b3;
    m_Matrix[10] = eMat.c3;
    m_Matrix[11] = eMat.d3;

    m_Matrix[12] = eMat.a4;
    m_Matrix[13] = eMat.b4;
    m_Matrix[14] = eMat.c4;
    m_Matrix[15] = eMat.d4;

}

bool Node3D::IsMatrixIdentity()
{
    for(int i = 0; i < 16; i++)
    {
        if( m_Matrix[i] == 1.0f)
        {
            if(i!=0 && i!=5 &&i!=10 && i!=15)
            {
                return false;
            }
        }else if(m_Matrix[i] != 0.0f)
        {
            return false;
        }
    }
    return true;
}
