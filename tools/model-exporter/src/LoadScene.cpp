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

#include <iostream>
#include <assimp/scene.h>
#include <math.h>

#include "LoadScene.h"


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

void GetSceneNodes(Scene3D &scene_data, Node3D *parent, const aiScene *scene, aiNode *aNode)
{

    Node3D *pnode;
    if(!aNode->mNumMeshes)
    {
        if(!aNode->mNumChildren)
        {
            string camname;
            camname.assign(aNode->mName.data,aNode->mName.length);
            if( NodeIsCamera( scene, camname ) )
            {
                return;
            }
        }
        pnode = new Node3D(parent, false);
        pnode->m_Name.assign(aNode->mName.data,aNode->mName.length);
        scene_data.AddNode(pnode);
        pnode->SetMatrix(aNode->mTransformation);
        if(parent)
        {
            parent->m_Children.push_back(pnode);
        }
    }


    for( unsigned int n = 0; n < aNode->mNumMeshes; n++ )
    {
        struct aiMesh *mesh = scene->mMeshes[aNode->mMeshes[n]];
        Node3D *node = new Node3D(parent, true);
        node->SetMatrix(aNode->mTransformation);
        scene_data.AddNode(node);
        if(!n)
        {
            pnode = node;
            pnode->m_Name.assign(aNode->mName.data,aNode->mName.length);
            if(parent)
            {
                parent->m_Children.push_back(pnode);
            }
        }
        for(unsigned int f = 0; f < mesh->mNumFaces; f++ )
        {
            struct aiFace *face = &mesh->mFaces[f];
            if(face->mNumIndices != 3) //Is not a triangle
            {
                continue;
            }
            for(unsigned int i = 0; i < face->mNumIndices; i++ )
            {
                unsigned short index = face->mIndices[i];
                node->m_Indices.push_back(index);
            }
        }
        node->m_Positions.assign( (Vector3*) mesh->mVertices, (Vector3*) (mesh->mVertices + mesh->mNumVertices));
        node->m_Normals.assign( (Vector3*) mesh->mNormals, (Vector3*) (mesh->mNormals + mesh->mNumVertices) );
        if(!mesh->HasTextureCoords(0))
        {
            cout << "No textures" << endl;
        }
        else
        {
            if( mesh->mNumUVComponents[0] == 2 )
            {
                for(unsigned int co = 0; co < mesh->mNumVertices; co++)
                {
                    Vector2 uv;
                    uv.x = (mesh->mTextureCoords[0] + co)->x;
                    uv.y = (mesh->mTextureCoords[0] + co)->y;
                    node->m_Textures.push_back(uv);
                }
            }else
            {
                cout << "3D textures coords, not supported" << endl;
                //node->m_Textures3D.assign( (Vector3*) mesh->mTextureCoords[0], (Vector3*) (mesh->mTextureCoords[0] + mesh->mNumVertices) );
            }
        }
        if( !mesh->mTangents)
        {
            cout << "No tangents" << endl;
        }
        else
        {
            node->m_Tangents.assign( (Vector3*) mesh->mTangents, (Vector3*) (mesh->mTangents + mesh->mNumVertices) );
            node->m_Bitangents.assign( (Vector3*) mesh->mBitangents, (Vector3*) (mesh->mBitangents + mesh->mNumVertices) );
        }
    }
    for(unsigned int c = 0; c < aNode->mNumChildren; c++ )
    {
        GetSceneNodes(scene_data, pnode, scene, aNode->mChildren[c]);
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
            //cam.SetMatrix( vlookat, vUp , vPosition );
            scene_data.AddCamera( cam );
        }

    }
}
