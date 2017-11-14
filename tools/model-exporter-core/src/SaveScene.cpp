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

#include "SaveScene.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void SaveNodes(Scene3D *scene, ostream &txtFile);
void SaveMeshes(Scene3D *scene, ostream &txtFile, ostream &binFile, std::string fileNameBinPath);
void SaveCameras(Scene3D *scene, ostream &txtFile);
void SaveMaterials(Scene3D *scene, ostream &txtFile);
void SaveEnvironment(Scene3D *scene, ostream &txtFile);
void SaveShaders(Scene3D *scene, ostream &txtFile);
void SaveAnimations(Scene3D *scene, ostream &txtFile);

bool SaveScene(Scene3D *scene,std::string fileNamePath, std::string fileNameBinPath)
{
    struct Files
    {
        std::ofstream dliStream;
        std::ofstream binStream;

        Files( std::string dliName, std::string binName )
        {
            dliStream.open( dliName );
            binStream.open( binName, ios::binary );
        }

        ~Files()
        {
            if( dliStream.is_open() )
            {
                dliStream.close();
            }

            if( binStream.is_open() )
            {
                binStream.close();
            }
        }
    } files( fileNamePath, fileNameBinPath );

    return ConvertScene(scene, fileNamePath, files.dliStream, files.binStream);
}

bool ConvertScene(Scene3D* scene, std::string fileNameBin, std::ostream& outDli, std::ostream& outBin)
{
    // If filenameBin is a path, now is a good time to discard all but the filename & extension -
	// the .bin file that we are going to reference must be in the same directory.
    auto iDirSeparator = std::min( fileNameBin.rfind( '\\' ), fileNameBin.rfind( '/' ) );
    if( iDirSeparator != std::string::npos )
    {
        fileNameBin = fileNameBin.substr( iDirSeparator + 1, fileNameBin.length() - iDirSeparator - 1);
    }

    // Write scene data.
    outDli << "{\n" << "    \"asset\" : { " << "\"version\" : \"1.0\"" << " },\n";
    outDli << "    \"scene\" : 0,\n";
    outDli << "    \"scenes\" : [ { \"nodes\" : [ 0 ] } ],\n";

    //Save Nodes
    outDli << "    \"nodes\" : [\n";
        SaveNodes( scene, outDli );
    outDli << "    ], \n";

    outDli << "    \"meshes\" : [\n";
        SaveMeshes( scene, outDli, outBin, fileNameBin );
    outDli << "    ], \n";

    //Save Cameras
    outDli << "    \"cameras\" : [\n";
        SaveCameras( scene, outDli );
    outDli << "    ], \n";

    //Save Materials
    outDli << "    \"materials\" : [\n";
        SaveMaterials( scene, outDli );
    outDli << "    ],\n";

    //Save Environment
    outDli << "    \"environment\" : [\n";
        SaveEnvironment( scene, outDli );
    outDli << "    ],\n";

    //Save Shaders
    outDli << "    \"shaders\" : [\n";
        SaveShaders( scene, outDli );
    outDli << "    ]";

    //Save Animations
    if(scene->HasAnimations())
    {
        outDli << ",\n    \"animations\" : {\n";
            SaveAnimations( scene, outDli );
        outDli << "\n    }";
    }


    outDli << "\n}\n";
    return true;
}

void SaveNodes(Scene3D *scene, ostream &txtFile)
{
    unsigned int meshIdx = 0;
    for(unsigned int n = 0; n < scene->GetNumNodes(); n++)
    {
        bool cflag = false;
        txtFile << "        {\n";
        Node3D *node = scene->GetNode(n);
        if(!node->m_Name.empty())
        {
            txtFile << "            \"name\": \""<<node->m_Name<<"\"";
            cflag = true;
        }
        if( !node->IsMatrixIdentity() )
        {
            if(cflag)
                txtFile << ",\n";

            txtFile << "            \"matrix\": [";
            for(int i=0;i<15;i++)
            {
                txtFile << node->m_Matrix[i]<<", ";
            }
            txtFile << node->m_Matrix[15]<<"]";
            cflag = true;
        }
        if( node->m_HasMesh)
        {
            if(cflag)
                txtFile << ",\n";

            txtFile << "            \"mesh\": " << meshIdx++;
            cflag = true;
        }
        if(node->m_Children.size()>0)
        {
            if(cflag)
                txtFile << ",\n";

            txtFile << "            \"children\": [ ";

            for(unsigned int i = 1;i < node->m_Children.size(); i++)
            {
                txtFile << node->m_Children[i-1]->index <<", ";
            }
            txtFile << node->m_Children[node->m_Children.size()-1]->index;
            txtFile << " ]";
            cflag = true;
        }
        if(n == scene->GetNumNodes()-1)
        {
            txtFile << "\n        }\n";
        }
        else
        {
            txtFile << "\n        },\n";
        }
    }
}

void SaveMeshes(Scene3D *scene, ostream &txtFile, ostream &binFile, std::string fileNameBin)
{
    unsigned int offset = 0;
    unsigned int length = 0;
    unsigned int meshIdx = 0;
    for(unsigned int n = 0; n < scene->GetNumNodes(); n++)
    {

        Node3D *node = scene->GetNode(n);

        if( node->m_HasMesh)
        {
            if(meshIdx)
                txtFile << "        ,{\n";
            else
                txtFile << "        {\n";
            int attributes = 0;
            attributes |= (node->m_Indices.size()>0)?1:0;
            attributes |= (node->m_Positions.size()>0)?2:0;
            attributes |= (node->m_Normals.size()>0)?4:0;
            attributes |= (node->m_Textures.size()>0)?8:0;
            attributes |= (node->m_Tangents.size()>0)?16:0;
            attributes |= (node->m_Bitangents.size()>0)?32:0;
            txtFile << "            \"uri\": \""<<fileNameBin<<"\",\n";
            txtFile << "            \"attributes\": " << attributes << ",\n";
            txtFile << "            \"primitive\": \"TRIANGLES\"" << ",\n";
            txtFile << "            \"indices\": { ";
            offset += length;
            txtFile << "\"byteOffset\": "<< offset <<", ";
            length = node->m_Indices.size() * sizeof(unsigned short);
            txtFile << "\"byteLength\": "<< length  <<" }";
            binFile.write((char*)node->m_Indices.data(), length );

            txtFile << ",\n            \"positions\": { ";
            offset += length;
            txtFile << "\"byteOffset\": "<< offset <<", ";
            length = node->m_Positions.size() * sizeof(Vector3);
            txtFile << "\"byteLength\": "<< length  <<" }";
            binFile.write((char*)node->m_Positions.data(), length );

            if(node->m_Normals.size())
            {
                txtFile << ",\n            \"normals\": { ";
                offset += length;
                txtFile << "\"byteOffset\": "<< offset <<", ";
                length = node->m_Normals.size() * sizeof(Vector3);
                txtFile << "\"byteLength\": "<< length  <<" }";
                binFile.write((char*)node->m_Normals.data(), length );
            }
            if(node->m_Textures.size())
            {
                txtFile << ",\n            \"textures\": { ";
                offset += length;
                txtFile << "\"byteOffset\": "<< offset <<", ";
                length = node->m_Textures.size() * sizeof(Vector2);
                txtFile << "\"byteLength\": "<< length  <<" }";
                binFile.write((char*)node->m_Textures.data(), length );
            }

            if(node->m_Tangents.size())
            {
                txtFile << ",\n            \"tangents\": { ";
                offset += length;
                txtFile << "\"byteOffset\": "<< offset <<", ";
                length = node->m_Tangents.size() * sizeof(Vector3);
                txtFile << "\"byteLength\": "<< length  <<" }";
                binFile.write((char*)node->m_Tangents.data(), length );
            }

            if(node->m_Bitangents.size())
            {
                txtFile << ",\n            \"bitangents\": { ";
                offset += length;
                txtFile << "\"byteOffset\": "<< offset <<", ";
                length = node->m_Bitangents.size() * sizeof(Vector3);
                txtFile << "\"byteLength\": "<< length  <<" }";
                binFile.write((char*)node->m_Bitangents.data(), length );
            }

            txtFile << "\n        }\n";
            meshIdx++;
        }
    }
}

void SaveCameras( Scene3D *scene, ostream &txtFile )
{
    if( !scene->GetNumCameras() )
    {
        txtFile << "        {\n";
        txtFile << "            \"fov\": 60.0,\n";
        txtFile << "            \"near\": 0.1,\n";
        txtFile << "            \"far\": 1000.0,\n";
        txtFile << "            \"matrix\": [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 3.5, 1.0]\n";
        txtFile << "        }\n";
    }
    for( unsigned int i = 0; i < scene->GetNumCameras(); i++ )
    {
        Camera3D *cam = scene->GetCamera( i );
        txtFile << "        {\n";
        txtFile << "            \"fov\": " << cam->fov << ",\n";
        txtFile << "            \"near\": " << cam->near << ",\n";
        txtFile << "            \"far\": " << cam->far << ",\n";
        txtFile << "            \"matrix\": [";

        for(int i=0;i<15;i++)
        {
            txtFile << cam->m_Matrix[i]<<", ";
        }
        txtFile << cam->m_Matrix[15]<<"]\n";

        if( i == scene->GetNumCameras() - 1)
        {
            txtFile << "        }\n";
        }
        else
        {
            txtFile << "        },\n";
        }
    }
}

void SaveMaterials( Scene3D *scene, ostream &txtFile )
{
    txtFile << "        {\n";
    txtFile << "            \"texture1\": \"scenes/Basic_albedo_metallic.png\",\n";
    txtFile << "            \"texture2\": \"scenes/Basic_normal_roughness.png\",\n";
    txtFile << "            \"environment\": 1\n";
    txtFile << "        }\n";
}

void SaveEnvironment( Scene3D *scene, ostream &txtFile )
{
    txtFile << "        {\n";
    txtFile << "        },\n";
    txtFile << "        {\n";
    txtFile << "            \"cubeSpecular\": \"scenes/EnvironmentTest_Radiance.ktx\",\n";
    txtFile << "            \"cubeDiffuse\": \"scenes/EnvironmentTest_Irradiance.ktx\"\n";
    txtFile << "        }\n";
}

void SaveShaders( Scene3D *scene, ostream &txtFile )
{
    txtFile << "        {\n";
    txtFile << "            \"vertex\": \"scenes/default_pbr_shader.vsh\",\n";
    txtFile << "            \"fragment\": \"scenes/default_pbr_shader.fsh\",\n";
    txtFile << "            \"uCubeMatrix\": [ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ],\n";
    txtFile << "            \"maxLOD\": 8\n";
    txtFile << "        }\n";
}

void SaveAnimations( Scene3D *scene, ostream &txtFile )
{
    for(unsigned int a = 0; a < scene->GetNumAnimations(); a++ )
    {
        Animation3D *animation = scene->GetAnimation( a );
        bool keyComaFlag = false;
        if( !a )
        {
           txtFile << "      \"" << animation->Name << "\" : [ \n";
        }
        else
        {
           txtFile << ",\n      \"" << animation->Name << "\" : [ \n";
        }
        for(unsigned int n = 0; n < animation->AnimNodesList.size(); n++ )
        {
            NodeAnimation3D nodeAnim = animation->AnimNodesList[n];

            if(nodeAnim.Rotations.size())
            {
                if(keyComaFlag)
                {
                    txtFile << ",\n";
                }
                keyComaFlag = true;
                txtFile << "        {\n";
                txtFile << "          \"properties\": [{\n";
                txtFile << "            \"actor\": \""<< nodeAnim.NodeName <<"\",\n";
                txtFile << "            \"property\": \"orientation\",\n";
                txtFile << "            \"timePeriod\": { \"delay\": 0.0, \"duration\": " << animation->Duration * animation->TicksPerSecond<<" },\n";
                txtFile << "            \"keyFrames\": [\n";
                for(unsigned int k = 0; k < nodeAnim.Rotations.size(); k++ )
                {
                    NodeKey nkey = nodeAnim.Rotations[k];
                    if(k)
                        txtFile << ",\n";
                    txtFile << "              { \"progress\": "<< nkey.time/animation->Duration << ", \"value\": ["<<nkey.v[0]<<", "<<nkey.v[1]<<", "<<nkey.v[2]<<", "<<nkey.v[3]<< "]}";
                }
                txtFile << "\n            ]\n";
                txtFile << "          }],\n          \"loop\": true\n";
                txtFile << "        }";
            }
            if(nodeAnim.Positions.size())
            {
                if(keyComaFlag)
                {
                    txtFile << ",\n";
                }

                keyComaFlag = true;
                txtFile << "        {\n";
                txtFile << "          \"properties\": [{\n";
                txtFile << "            \"actor\": \""<< nodeAnim.NodeName <<"\",\n";
                txtFile << "            \"property\": \"position\",\n";
                txtFile << "            \"timePeriod\": { \"delay\": 0.0, \"duration\": " << animation->Duration * animation->TicksPerSecond<<" },\n";
                txtFile << "            \"keyFrames\": [\n";
                for(unsigned int k = 0; k < nodeAnim.Positions.size(); k++ )
                {
                    NodeKey nkey = nodeAnim.Positions[k];
                    if(k)
                        txtFile << ",\n";
                    txtFile << "              { \"progress\": "<< nkey.time/animation->Duration << ", \"value\": ["<<nkey.v[0]<<", "<<nkey.v[1]<<", " << nkey.v[2] <<"]}";
                }
                txtFile << "\n            ]\n";
                txtFile << "          }],\n          \"loop\": true\n";
                txtFile << "        }";
            }
            if(nodeAnim.Scales.size())
            {
                if(keyComaFlag)
                {
                    txtFile << ",\n";
                }

                keyComaFlag = true;
                txtFile << "        {\n";
                txtFile << "          \"properties\": [{\n";
                txtFile << "            \"actor\": \""<< nodeAnim.NodeName <<"\",\n";
                txtFile << "            \"property\": \"scale\",\n";
                txtFile << "            \"timePeriod\": { \"delay\": 0.0, \"duration\": " << animation->Duration * animation->TicksPerSecond<<" },\n";
                txtFile << "            \"keyFrames\": [\n";
                for(unsigned int k = 0; k < nodeAnim.Scales.size(); k++ )
                {
                    NodeKey nkey = nodeAnim.Scales[k];
                    if(k)
                        txtFile << ",\n";
                    txtFile << "              { \"progress\": "<< nkey.time/animation->Duration << ", \"value\": ["<<nkey.v[0]<<", "<<nkey.v[1]<<", " << nkey.v[2] <<"]}";
                }
                txtFile << "\n            ]\n";
                txtFile << "          }],\n          \"loop\": true\n";
                txtFile << "        }";
            }
        }

        txtFile << "\n      ]";
    }
}