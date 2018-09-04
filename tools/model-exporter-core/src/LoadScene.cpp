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

#include "LoadScene.h"
#include "Mesh.h"

#include <assimp/scene.h>

#include <iostream>
#include <math.h>
#include <algorithm>
#include <cassert>

using namespace std;

bool NodeIsCamera( const aiScene *scene, std::string eName )
{

    for( unsigned int n = 0; n < scene->mNumCameras; n++ )
    {
        const aiCamera *acam = scene->mCameras[n];
        string camname;
        camname.assign(acam->mName.data,acam->mName.length);
        if( eName == camname )
            return true;
    }
    return false;
}

void SetNodeMeshAndUpdateIds(unsigned int id, Node3D& node, MeshIds& meshIds)
{
    node.m_MeshId = id;

    auto iInsert = std::lower_bound(meshIds.begin(), meshIds.end(), id);
    if (iInsert == meshIds.end() || *iInsert != id)
    {
        meshIds.insert(iInsert, id);
    }
}

void GetSceneNodes(Scene3D &scene_data, MeshIds& meshIds, Node3D *parent, const aiScene *scene, const aiNode *aNode)
{
    if(0 == aNode->mNumMeshes && 0 == aNode->mNumChildren)
    {
        string camname(aNode->mName.data, aNode->mName.length);
        if( NodeIsCamera( scene, camname ) )
        {
            return;
        }
    }

    Node3D *pnode = new Node3D(parent);
    pnode->m_Name.assign(aNode->mName.data,aNode->mName.length);
    pnode->SetMatrix(aNode->mTransformation);
    scene_data.AddNode(pnode);

    if (aNode->mNumMeshes > 0)
    {
        auto meshId = aNode->mMeshes[0];
        SetNodeMeshAndUpdateIds(meshId, *pnode, meshIds);

        // Create an anonymous node each for the rest of the meshes, with the same transform.
        for (unsigned int i = 1; i < aNode->mNumMeshes; ++i)
        {
            Node3D* node = new Node3D(parent);
            node->SetMatrix(aNode->mTransformation);

            SetNodeMeshAndUpdateIds(aNode->mMeshes[i], *node, meshIds);

            scene_data.AddNode(node);
        }
    }

    for(unsigned int c = 0; c < aNode->mNumChildren; c++ )
    {
        GetSceneNodes(scene_data, meshIds, pnode, scene, aNode->mChildren[c]);
    }
}

void PackSceneNodeMeshIds(Scene3D& scene_data, const MeshIds& meshIds)
{
    for (unsigned int i = 0; i < scene_data.GetNumNodes(); ++i)
    {
        auto node = scene_data.GetNode(i);
        if (node->HasMesh())
        {
            auto iFind = std::lower_bound(meshIds.begin(), meshIds.end(), node->m_MeshId);
            assert(iFind != meshIds.end());
            node->m_MeshId = std::distance(meshIds.begin(), iFind);
        }
    }
}

void GetSceneMeshes(Scene3D& scene_data, const MeshIds& meshIds, const aiScene* scene)
{
    for (auto& i : meshIds)
    {
        struct aiMesh *mesh = scene->mMeshes[i];

        Mesh* pmesh = new Mesh();
        pmesh->m_Indices.reserve(mesh->mNumFaces * 3);
        for (unsigned int f = 0; f < mesh->mNumFaces; f++)
        {
            struct aiFace *face = &mesh->mFaces[f];

            assert(face->mNumIndices == 3);    // aiProcess_Triangulate
            for (unsigned int i = 0; i < face->mNumIndices; i++)
            {
                unsigned short index = face->mIndices[i];
                pmesh->m_Indices.push_back(index);
            }
        }

        pmesh->m_Positions.assign((Vector3*)mesh->mVertices, (Vector3*)(mesh->mVertices + mesh->mNumVertices));
        pmesh->m_Normals.assign((Vector3*)mesh->mNormals, (Vector3*)(mesh->mNormals + mesh->mNumVertices));
        if (!mesh->HasTextureCoords(0))
        {
            cout << "No textures" << endl;
        }
        else
        {
            if (mesh->mNumUVComponents[0] == 2)
            {
                pmesh->m_Textures.reserve(mesh->mNumVertices);
                for (unsigned int co = 0; co < mesh->mNumVertices; co++)
                {
                    Vector2 uv;
                    uv.x = mesh->mTextureCoords[0][co].x;
                    uv.y = mesh->mTextureCoords[0][co].y;
                    pmesh->m_Textures.push_back(uv);
                }
            }
            else
            {
                cout << "3D textures coords, not supported" << endl;
                //node->m_Textures3D.assign( (Vector3*) mesh->mTextureCoords[0], (Vector3*) (mesh->mTextureCoords[0] + mesh->mNumVertices) );
            }
        }

        if (!mesh->mTangents)
        {
            cout << "No tangents" << endl;
        }
        else
        {
            pmesh->m_Tangents.assign((Vector3*)mesh->mTangents, (Vector3*)(mesh->mTangents + mesh->mNumVertices));
        }

        if (0 != mesh->mNumBones)   // Get skinning data
        {
            pmesh->m_BoneIds.resize(pmesh->m_Positions.size(), Vector4 { .0, .0, .0, .0 });
            pmesh->m_BoneWeights.resize(pmesh->m_Positions.size(), Vector4 { .0, .0, .0, .0 });

            std::vector<int> nextBone;
            nextBone.resize(pmesh->m_Positions.size(), 0);

            auto iBone = mesh->mBones;
            for (auto endBones = iBone + mesh->mNumBones; iBone != endBones; ++iBone)
            {
                auto bone = *iBone;
                if (bone->mNumWeights > 0)
                {
                    auto boneNode = scene_data.FindNodeNamed(bone->mName.C_Str());
                    if (!boneNode)
                    {
                        cout << "ERROR: Bone '" << bone->mName.C_Str() << "' of mesh '" << mesh->mName.C_Str() <<
                            "' references invalid joint '" << bone->mName.C_Str() << "'." << endl;
                        return;
                    }
                    assert (boneNode != nullptr);

                    auto iWeight = bone->mWeights;
                    for (auto endWeights = iWeight + bone->mNumWeights; iWeight != endWeights; ++iWeight)
                    {
                        int iNextBone = nextBone[iWeight->mVertexId];
                        if (iNextBone < 4)
                        {
                            pmesh->m_BoneIds[iWeight->mVertexId].data[iNextBone] = boneNode->m_Index;
                            pmesh->m_BoneWeights[iWeight->mVertexId].data[iNextBone] = iWeight->mWeight;
                            ++nextBone[iWeight->mVertexId];
                        }
                        else
                        {
                            cout << "WARNING: Vertex " << iWeight->mVertexId << " of mesh '" << mesh->mName.C_Str() <<
                                "' exceeds the number of supported weights." << endl;
                        }
                    }
                }
            }
        }

        scene_data.AddMesh(pmesh);
    }
}

aiNode *GetCameraNode( const aiScene *scene, aiNode *aNode )
{
    if(!aNode->mNumMeshes)
    {
        if(!aNode->mNumChildren)
        {
            string camname;
            camname.assign(aNode->mName.data,aNode->mName.length);
            if( NodeIsCamera( scene, camname ) )
            {
                return aNode;
            }
        }
    }

    for(unsigned int c = 0; c < aNode->mNumChildren; c++ )
    {
        aiNode *returnNode = GetCameraNode( scene, aNode->mChildren[c]);
        if(returnNode)
            return returnNode;
    }
    return nullptr;
}

void GetSceneCameras( Scene3D &scene_data, const aiScene *scene )
{
    for( unsigned int n = 0; n < scene->mNumCameras; n++ )
    {
        const aiCamera *acam = scene->mCameras[n];
        Camera3D cam;
        cam.far = acam->mClipPlaneFar;
        cam.near = acam->mClipPlaneNear;
        cam.fov = acam->mHorizontalFOV *180.0f / 3.14159265358979323846;
        Vector3 vlookat;
        Vector3 vUp;
        Vector3 vPosition;
        vlookat.x = acam->mLookAt.x;
        vlookat.y = acam->mLookAt.y;
        vlookat.z = acam->mLookAt.z;
        vUp.x = acam->mUp.x;
        vUp.y = acam->mUp.y;
        vUp.z = acam->mUp.z;
        vPosition.x = acam->mPosition.x;
        vPosition.y = acam->mPosition.y;
        vPosition.z = acam->mPosition.z;
        aiNode *pnode = GetCameraNode( scene, scene->mRootNode );
        if( pnode )
        {
            cam.SetMatrix( pnode->mTransformation );
            cam.MultLookAtMatrix( vlookat, vUp , vPosition );
            scene_data.AddCamera( cam );
        }
    }
}

aiNode* FindLeafNamed(const string& eName, aiNode* node)
{
    if(node->mNumMeshes == 0 &&
        node->mNumChildren == 0 &&
        node->mName.length == eName.size() &&
        strncmp(node->mName.data, eName.c_str(), node->mName.length) == 0)
    {
        return node;
    }
    else
    {
        aiNode* result = nullptr;
        auto iEnd = node->mChildren + node->mNumChildren;
        for (auto i = node->mChildren; i != iEnd; ++i)
        {
            if (auto n = FindLeafNamed(eName, *i))
            {
                result = n;
                break;
            }
        }
        return result;
    }
}

void GetSceneLights( Scene3D& scene_data, const aiScene* scene)
{
    for( unsigned int n = 0; n < scene->mNumLights; ++n)
    {
        const aiLight* aLight = scene->mLights[n];
        string name(aLight->mName.data, aLight->mName.length);

        Light light;
        aiNode* node = FindLeafNamed( name, scene->mRootNode );
        if (node)
        {
            light.SetMatrix(node->mTransformation);
            light.SetDiffuseColor(aLight->mColorDiffuse);
            scene_data.AddLight(light);
        }
    }
}

void GetAnimations( Scene3D &scene_data, const aiScene *scene )
{
    if(!scene->HasAnimations())
    {
        return;
    }

    for( unsigned int a = 0; a < scene->mNumAnimations; a++)
    {
        float timeFactor = 1.0f;
        aiAnimation *animation = scene->mAnimations[a];
        if( !animation->mNumChannels )
        {
            continue;
        }
        Animation3D dataAnim;
        timeFactor = dataAnim.TicksPerSecond;

        dataAnim.Duration = animation->mDuration;
        dataAnim.TicksPerSecond = animation->mTicksPerSecond;


        dataAnim.Name.assign(animation->mName.data, animation->mName.length);


         for( unsigned int ch = 0; ch < animation->mNumChannels; ch++)
         {
            bool diffFlag = false;
            aiQuaternion cquat;
            aiVector3D cvec;
            aiNodeAnim* nAnim = animation->mChannels[ch];
            NodeAnimation3D nodeAnim;
            nodeAnim.NodeName.assign( nAnim->mNodeName.data, nAnim->mNodeName.length );

            for( unsigned int k = 0; k < nAnim->mNumRotationKeys; k++ )
            {
                aiQuatKey qkey = nAnim->mRotationKeys[k];
                NodeKey nkey;
                nkey.time = qkey.mTime * timeFactor;
                nkey.v[0] = qkey.mValue.x;
                nkey.v[1] = qkey.mValue.y;
                nkey.v[2] = qkey.mValue.z;
                nkey.v[3] = qkey.mValue.w;
                nodeAnim.Rotations.push_back( nkey );
                if(!k)
                {
                  cquat =  qkey.mValue;
                }
                else if(cquat != qkey.mValue)
                {
                    diffFlag = true;
                }
            }
            if( !diffFlag )
            {
                nodeAnim.Rotations.clear();
            }
            diffFlag = false;
            for( unsigned int k = 0; k < nAnim->mNumPositionKeys; k++ )
            {
                aiVectorKey vkey = nAnim->mPositionKeys[k];
                NodeKey nkey;
                nkey.time = vkey.mTime * timeFactor;
                nkey.v[0] = vkey.mValue[0];
                nkey.v[1] = vkey.mValue[1];
                nkey.v[2] = vkey.mValue[2];
                nodeAnim.Positions.push_back( nkey );
                if(!k)
                {
                  cvec =  vkey.mValue;
                }
                else if(cvec != vkey.mValue)
                {
                    diffFlag = true;
                }
            }
            if( !diffFlag )
            {
                nodeAnim.Positions.clear();
            }
            diffFlag = false;
            for( unsigned int k = 0; k < nAnim->mNumScalingKeys; k++ )
            {
                aiVectorKey vkey = nAnim->mScalingKeys[k];
                NodeKey nkey;
                nkey.time = vkey.mTime * timeFactor;
                nkey.v[0] = vkey.mValue[0];
                nkey.v[1] = vkey.mValue[1];
                nkey.v[2] = vkey.mValue[2];
                nodeAnim.Scales.push_back( nkey );
                if(!k)
                {
                  cvec =  vkey.mValue;
                }
                else if(cvec != vkey.mValue)
                {
                    diffFlag = true;
                }
            }
            if( !diffFlag )
            {
                nodeAnim.Scales.clear();
            }

            if(nodeAnim.Rotations.size() + nodeAnim.Positions.size() + nodeAnim.Scales.size())
            {
                dataAnim.AnimNodesList.push_back(nodeAnim);
            }
         }
        scene_data.AddAnimation(dataAnim);
    }
}
