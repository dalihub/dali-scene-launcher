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

void SaveNodes(Scene3D *scene, ofstream &txtFile);
void SaveMeshes(Scene3D *scene, ofstream &txtFile,ofstream &binFile, std::string fileNameBinPath);
void SaveCameras(Scene3D *scene, ofstream &txtFile);
void SaveMaterials(Scene3D *scene, ofstream &txtFile);
void SaveEnvironment(Scene3D *scene, ofstream &txtFile);
void SaveShaders(Scene3D *scene, ofstream &txtFile);

bool SaveScene(Scene3D *scene,std::string fileNamePath, std::string fileNameBinPath)
{
    ofstream txtFile;
    ofstream binFile;
    txtFile.open( fileNamePath );
    binFile.open( fileNameBinPath, ios::binary );
    std::string fileName = fileNamePath.substr( fileNamePath.rfind('\\') + 1, fileNamePath.length() - fileNamePath.rfind('\\') - 1 );
    std::string fileNameBin = fileNameBinPath.substr( fileNameBinPath.rfind('\\') + 1, fileNameBinPath.length() - fileNameBinPath.rfind('\\') - 1);
    txtFile << "{\n" << "    \"asset\" : { " << "\"version\" : \"1.0\"" << " },\n";
    txtFile << "    \"scene\" : 0,\n";
    txtFile << "    \"scenes\" : [ { \"nodes\" : [ 0 ] } ],\n";

    //Save Nodes
    txtFile << "    \"nodes\" : [ \n";
        SaveNodes( scene, txtFile );
    txtFile << "    ], \n";

    txtFile << "    \"meshes\" : [ \n";
        SaveMeshes( scene, txtFile, binFile, fileNameBin );
    txtFile << "    ], \n";

    //Save Cameras
    txtFile << "    \"cameras\" : [ \n";
        SaveCameras( scene, txtFile );
    txtFile << "    ], \n";

    //Save Materials
    txtFile << "    \"materials\" : [ \n";
        SaveMaterials( scene, txtFile );
    txtFile << "    ], \n";

    //Save Environment
    txtFile << "    \"environment\" : { \n";
        SaveEnvironment( scene, txtFile );
    txtFile << "    }, \n";

    //Save Shaders
    txtFile << "    \"shaders\" : [ \n";
        SaveShaders( scene, txtFile );
    txtFile << "    ] \n";


    txtFile << "}\n";
    txtFile.close();
    binFile.close();
    return true;
}

void SaveNodes(Scene3D *scene, ofstream &txtFile)
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

void SaveMeshes(Scene3D *scene, ofstream &txtFile, ofstream &binFile, std::string fileNameBin)
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
            txtFile << "            \"indices\": { \n";
            offset += length;
            txtFile << "                \"byteOffset\": "<< offset <<",\n";
            length = node->m_Indices.size() * sizeof(unsigned short);
            txtFile << "                \"byteLength\": "<< length  <<"}";
            binFile.write((char*)node->m_Indices.data(), length );

            txtFile << ",\n            \"positions\": { \n";
            offset += length;
            txtFile << "                \"byteOffset\": "<< offset <<",\n";
            length = node->m_Positions.size() * sizeof(Vector3);
            txtFile << "                \"byteLength\": "<< length  <<"}";
            binFile.write((char*)node->m_Positions.data(), length );

            if(node->m_Normals.size())
            {
                txtFile << ",\n            \"normals\": { \n";
                offset += length;
                txtFile << "                \"byteOffset\": "<< offset <<",\n";
                length = node->m_Normals.size() * sizeof(Vector3);
                txtFile << "                \"byteLength\": "<< length  <<"}";
                binFile.write((char*)node->m_Normals.data(), length );
            }
            if(node->m_Textures.size())
            {
                txtFile << ",\n            \"textures\": { \n";
                offset += length;
                txtFile << "                \"byteOffset\": "<< offset <<",\n";
                length = node->m_Textures.size() * sizeof(Vector2);
                txtFile << "                \"byteLength\": "<< length  <<"}";
                binFile.write((char*)node->m_Textures.data(), length );
            }

            if(node->m_Tangents.size())
            {
                txtFile << ",\n            \"tangents\": { \n";
                offset += length;
                txtFile << "                \"byteOffset\": "<< offset <<",\n";
                length = node->m_Tangents.size() * sizeof(Vector3);
                txtFile << "                \"byteLength\": "<< length  <<"}";
                binFile.write((char*)node->m_Tangents.data(), length );
            }

            if(node->m_Bitangents.size())
            {
                txtFile << ",\n            \"bitangents\": { \n";
                offset += length;
                txtFile << "                \"byteOffset\": "<< offset <<",\n";
                length = node->m_Bitangents.size() * sizeof(Vector3);
                txtFile << "                \"byteLength\": "<< length  <<"}";
                binFile.write((char*)node->m_Bitangents.data(), length );
            }

            txtFile << "\n        }\n";
            meshIdx++;
        }


    }
}

void SaveCameras(Scene3D *scene, ofstream &txtFile)
{
    txtFile << "        {\n";
    txtFile << "            \"fov\": 60.0,\n";
    txtFile << "            \"near\": 0.1,\n";
    txtFile << "            \"far\": 1000.0,\n";
    txtFile << "            \"position\": [0.0, 0.0, 3.5]\n";
    txtFile << "        }\n";
}

void SaveMaterials(Scene3D *scene, ofstream &txtFile)
{
    txtFile << "        {\n";
    txtFile << "            \"texture1\": \"\",\n";
    txtFile << "            \"texture2\": \"\"\n";
    txtFile << "        }\n";
}

void SaveEnvironment(Scene3D *scene, ofstream &txtFile)
{
    txtFile << "        \"cubeSpecular\": \"scenes/EnvironmentTest_Radiance.ktx\",\n";
    txtFile << "        \"cubeDiffuse\": \"scenes/EnvironmentTest_Irradiance.ktx\",\n";
    txtFile << "        \"maxLOD\": 8\n";
}

void SaveShaders(Scene3D *scene, ofstream &txtFile)
{
    txtFile << "        {\n";
    txtFile << "            \"vertex\": \"scenes/default_pbr_shader.vsh\",\n";
    txtFile << "            \"fragment\": \"scenes/default_pbr_shader.fsh\"\n";
    txtFile << "        }\n";
}
