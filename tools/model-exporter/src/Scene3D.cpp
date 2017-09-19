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

#include "Scene3D.h"

Scene3D::Scene3D()
{
    index = 0;
}

Scene3D::~Scene3D()
{
    for(unsigned int i=0; i < m_nodes.size(); i++)
    {
        delete m_nodes[i];
    }
    m_nodes.clear();
}

void Scene3D::AddNode(Node3D *enode)
{
    enode->index = index++;
    m_nodes.push_back(enode);

}

unsigned int Scene3D::GetNumNodes()
{
    return m_nodes.size();
}

Node3D *Scene3D::GetNode(unsigned int idx)
{
    return m_nodes[idx];
}

void Scene3D::AddCamera(Camera3D &ecam)
{
    m_cameras.push_back(ecam);
}

unsigned int Scene3D::GetNumCameras()
{
    return m_cameras.size();
}

Camera3D* Scene3D::GetCamera(unsigned int idx)
{
    return &m_cameras[idx];
}

void Scene3D::AddAnimation(Animation3D &eanim)
{
    m_animations.push_back(eanim);
}
unsigned int Scene3D::GetNumAnimations()
{
    return m_animations.size();
}

Animation3D* Scene3D::GetAnimation(unsigned int idx)
{
    return &m_animations[idx];
}
bool Scene3D::HasAnimations()
{
    bool anim = false;
    for( unsigned int i = 0; i < m_animations.size(); i++ )
    {
        anim |= m_animations[i].HasAnimations();
    }
    return anim;
}
