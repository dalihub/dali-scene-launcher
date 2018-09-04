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

#include "SaveScene.h"
#include "Mesh.h"
#include "JsonWriter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

template <typename T>
void WriteArrayData(const T* data, unsigned int n, JsonWriter& writer)
{
  const T* end = data + n;
  while (data != end)
  {
    writer.WriteValue(nullptr, *data);
    ++data;
  }
}

template <unsigned int N, typename T>
void WriteArrayData(const T(&data)[N], JsonWriter& writer)
{
    WriteArrayData(data, N, writer);
}

void SaveNodes(Scene3D *scene, JsonWriter& outDli);
void SaveMeshes(Scene3D *scene, JsonWriter& outDli, ostream &outBin,
    std::string fileNameBinPath);
void SaveCameras(Scene3D *scene, JsonWriter& outDli);
void SaveLights(Scene3D *scene, JsonWriter& outDli);
void SaveMaterials(Scene3D *scene, JsonWriter& outDli);
void SaveEnvironment(Scene3D *scene, JsonWriter& outDli);
void SaveShaders(Scene3D *scene, JsonWriter& outDli);
void SaveAnimations(Scene3D *scene, JsonWriter& outDli);

bool SaveScene(Scene3D *scene, std::string fileNamePath,
    std::string fileNameBinPath)
{
  struct Files
  {
    std::ofstream dliStream;
    std::ofstream binStream;

    Files(std::string dliName, std::string binName)
    {
      dliStream.open(dliName);
      binStream.open(binName, ios::binary);
    }

    ~Files()
    {
      if (dliStream.is_open())
      {
        dliStream.close();
      }

      if (binStream.is_open())
      {
        binStream.close();
      }
    }
  } files(fileNamePath, fileNameBinPath);

  return ConvertScene(scene, fileNameBinPath, files.dliStream, files.binStream);
}

bool ConvertScene(Scene3D* scene, std::string fileNameBin, std::ostream& outDli,
    std::ostream& outBin)
{
  // If filenameBin is a path, now is a good time to discard all but the filename & extension -
  // the .bin file that we are going to reference must be in the same directory as the .dli.
  auto iDirSeparator = std::min(fileNameBin.rfind('\\'),
      fileNameBin.rfind('/'));
  if (iDirSeparator != std::string::npos)
  {
    fileNameBin = fileNameBin.substr(iDirSeparator + 1,
        fileNameBin.length() - iDirSeparator - 1);
  }

  // Write scene data.
  JsonWriter writer(outDli, "  ");
  writer.WriteObject(nullptr);

  writer.WriteObject("asset", true);
    writer.WriteValue("version", "1.0");
  writer.CloseScope();

  writer.WriteValue("scene", 0);

  writer.WriteArray("scenes", true);
    writer.WriteObject(nullptr, true);
      writer.WriteArray("nodes", true);
        writer.WriteValue(nullptr, 0);
      writer.CloseScope();
    writer.CloseScope();
  writer.CloseScope();

  //Save Nodes
  writer.WriteArray("nodes");
  SaveNodes(scene, writer);
  writer.CloseScope();

  writer.WriteArray("meshes");
  SaveMeshes(scene, writer, outBin, fileNameBin);
  writer.CloseScope();

  //Save Cameras
  writer.WriteArray("cameras");
  SaveCameras(scene, writer);
  writer.CloseScope();

  //Save Lights
  writer.WriteArray("lights");
  SaveLights(scene, writer);
  writer.CloseScope();

  //Save Materials
  writer.WriteArray("materials");
  SaveMaterials(scene, writer);
  writer.CloseScope();

  //Save Environment
  writer.WriteArray("environment");
  SaveEnvironment(scene, writer);
  writer.CloseScope();

  //Save Shaders
  writer.WriteArray("shaders");
  SaveShaders(scene, writer);
  writer.CloseScope();

  //Save Animations
  if (scene->HasAnimations())
  {
    writer.WriteArray("animations");
    SaveAnimations(scene, writer);
    writer.CloseScope();
  }

  writer.CloseScope();
  outDli << std::endl;
  return true;
}

void SaveNodes(Scene3D *scene, JsonWriter& outDli)
{
  for (unsigned int n = 0; n < scene->GetNumNodes(); n++)
  {
    outDli.WriteObject(nullptr);
    Node3D *node = scene->GetNode(n);
    if (!node->m_Name.empty())
    {
      outDli.WriteValue("name", node->m_Name.c_str());
    }
    if (!node->IsMatrixIdentity())
    {
      outDli.WriteArray("matrix", true);
      for (int i = 0; i < 16; i++)
      {
        outDli.WriteValue(nullptr, node->m_Matrix[i]);
      }
      outDli.CloseScope();
    }
    if (node->HasMesh())
    {
      outDli.WriteValue("mesh", node->m_MeshId);
    }
    if (node->m_Children.size() > 0)
    {
      outDli.WriteArray("children", true);

      for (auto i: node->m_Children)
      {
        outDli.WriteValue(nullptr, i->m_Index);
      }
      outDli.CloseScope();
    }

    outDli.CloseScope();
  }
}

void WriteBufferInternal(const char* name, unsigned int offset, unsigned int size, JsonWriter& writer)
{
  writer.WriteObject(name, true);
  writer.WriteValue("byteOffset", offset);
  writer.WriteValue("byteLength", size);
  writer.CloseScope();
}

template <typename T>
void WriteBuffer(const char* name, unsigned int offset, unsigned int numElems, JsonWriter& writer, unsigned int& outLength)
{
  outLength = numElems * sizeof(T);
  WriteBufferInternal(name, offset, outLength, writer);
}

void SaveMeshes(Scene3D *scene, JsonWriter& outDli, ostream &outBin,
    std::string fileNameBin)
{
  unsigned int offset = 0;
  unsigned int length = 0;
  for (unsigned int m = 0; m < scene->GetNumMeshes(); m++)
  {
    const Mesh* mesh = scene->GetMesh(m);

    outDli.WriteObject(nullptr);
    int attributes = 0;
    attributes |= (mesh->m_Indices.size() > 0) ? 1 : 0;
    attributes |= (mesh->m_Positions.size() > 0) ? 2 : 0;
    attributes |= (mesh->m_Normals.size() > 0) ? 4 : 0;
    attributes |= (mesh->m_Textures.size() > 0) ? 8 : 0;
    attributes |= (mesh->m_Tangents.size() > 0) ? 16 : 0;

    outDli.WriteValue("uri", fileNameBin.c_str());
    outDli.WriteValue("attributes", attributes);
    outDli.WriteValue("primitive", "TRIANGLES");

    offset += length;
    WriteBuffer<unsigned short>("indices", offset, mesh->m_Indices.size(), outDli, length);

    outBin.write((char*) mesh->m_Indices.data(), length);

    offset += length;
    WriteBuffer<Vector3>("positions", offset, mesh->m_Positions.size(), outDli, length);

    outBin.write((char*) mesh->m_Positions.data(), length);

    if (mesh->m_Normals.size())
    {
      offset += length;
      WriteBuffer<Vector3>("normals", offset, mesh->m_Normals.size(), outDli, length);

      outBin.write((char*) mesh->m_Normals.data(), length);
    }

    if (mesh->m_Textures.size())
    {
      offset += length;
      WriteBuffer<Vector2>("textures", offset, mesh->m_Textures.size(), outDli, length);

      outBin.write((char*) mesh->m_Textures.data(), length);
    }

    if (mesh->m_Tangents.size())
    {
      offset += length;
      WriteBuffer<Vector3>("tangents", offset, mesh->m_Tangents.size(), outDli, length);

      outBin.write((char*) mesh->m_Tangents.data(), length);
    }

      // write weights
      if (!mesh->m_BoneIds.size())
      {
        offset += length;
        WriteBuffer<Vector4>("bone_ids", offset, mesh->m_BoneIds.size(), outDli, length);

        outBin.write(reinterpret_cast<const char*>(mesh->m_BoneIds.data()), length);

        offset += length;
        WriteBuffer<Vector4>("bone_weights", offset, mesh->m_BoneWeights.size(), outDli, length);

        outBin.write(reinterpret_cast<const char*>(mesh->m_BoneWeights.data()), length);
      }
    outDli.CloseScope();
  }
}

void SaveCameras(Scene3D *scene, JsonWriter& outDli)
{
  if (!scene->GetNumCameras())
  {
    outDli.WriteObject(nullptr);
    outDli.WriteValue("fov", 60.0);
    outDli.WriteValue("near", 0.1);
    outDli.WriteValue("far", 1000.0);

    outDli.WriteArray("matrix", true);
    const double matrix[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 3.5, 1.0 };
    WriteArrayData(matrix, outDli);
    outDli.CloseScope();

    outDli.CloseScope();
  }

  for (unsigned int i = 0; i < scene->GetNumCameras(); i++)
  {
    Camera3D *cam = scene->GetCamera(i);
    outDli.WriteObject(nullptr);
    outDli.WriteValue("fov", cam->fov);
    outDli.WriteValue("near", cam->near);
    outDli.WriteValue("far", cam->far);

    outDli.WriteArray("matrix", true);
    WriteArrayData(cam->m_Matrix, outDli);
    outDli.CloseScope();

    outDli.CloseScope();
  }
}

void SaveLights(Scene3D* scene, JsonWriter& outDli)
{
  for (unsigned int i = 0; i < scene->GetNumLights(); ++i)
  {
    const Light* light = scene->GetLight(i);

    outDli.WriteObject(nullptr);
    outDli.WriteArray("matrix", true);
    WriteArrayData(light->m_Matrix, outDli);
    outDli.CloseScope();

    outDli.WriteArray("color", true);
    WriteArrayData(light->m_DiffuseColor.data, outDli);
    outDli.CloseScope();

    outDli.CloseScope();
  }
}

void SaveMaterials(Scene3D *scene, JsonWriter& outDli)
{
  outDli.WriteObject(nullptr);
  outDli.WriteValue("texture1", "Basic_albedo_metallic.png");
  outDli.WriteValue("texture2", "Basic_normal_roughness.png");
  outDli.WriteValue("mipmap", true);
  outDli.WriteValue("environment", 1);
  outDli.CloseScope();
}

void SaveEnvironment(Scene3D *scene, JsonWriter& outDli)
{
  outDli.WriteObject(nullptr);
  outDli.CloseScope();
  outDli.WriteObject(nullptr);
  outDli.WriteValue("cubeSpecular", "EnvironmentTest_Radiance.ktx");
  outDli.WriteValue("cubeDiffuse", "EnvironmentTest_Irradiance.ktx");
  outDli.CloseScope();
}

void SaveShaders(Scene3D *scene, JsonWriter& outDli)
{
  outDli.WriteObject(nullptr);
  outDli.WriteValue("vertex", "default_pbr_shader.vsh");
  outDli.WriteValue("fragment", "default_pbr_shader.fsh");
  outDli.WriteArray("uCubeMatrix", true);
  const double matrix[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
  WriteArrayData(matrix, outDli);
  outDli.CloseScope();
  outDli.WriteValue("uMaxLOD", 8);
  outDli.CloseScope();
}

void SaveAnimations(Scene3D *scene, JsonWriter& outDli)
{
  for (unsigned int a = 0; a < scene->GetNumAnimations(); a++)
  {
    Animation3D *animation = scene->GetAnimation(a);
    outDli.WriteObject(nullptr);
    outDli.WriteValue("name", animation->Name.c_str());
    outDli.WriteArray("animation");

    for (unsigned int n = 0; n < animation->AnimNodesList.size(); n++)
    {
      NodeAnimation3D nodeAnim = animation->AnimNodesList[n];

      if (nodeAnim.Rotations.size())
      {
        outDli.WriteObject(nullptr);
        outDli.WriteArray("properties", true);
        outDli.WriteObject(nullptr);
        outDli.WriteValue("actor", nodeAnim.NodeName.c_str());
        outDli.WriteValue("property", "orientation");

        outDli.WriteObject("timePeriod", true);
        outDli.WriteValue("delay", 0.0);
        outDli.WriteValue("duration", animation->Duration * animation->TicksPerSecond);
        outDli.CloseScope();

        outDli.WriteArray("keyFrames");
        for (unsigned int k = 0; k < nodeAnim.Rotations.size(); k++)
        {
          const NodeKey& nkey = nodeAnim.Rotations[k];
          outDli.WriteObject(nullptr, true);
          outDli.WriteValue("progress", nkey.time / animation->Duration);
          outDli.WriteArray("value", true);
          WriteArrayData(nkey.v, outDli);
          outDli.CloseScope();
          outDli.CloseScope();
        }

        outDli.CloseScope();
        outDli.CloseScope();
        outDli.CloseScope();
        outDli.WriteValue("loop", true);
        outDli.CloseScope();
      }

      if (nodeAnim.Positions.size())
      {
        outDli.WriteObject(nullptr);
        outDli.WriteArray("properties", true);
        outDli.WriteObject(nullptr);
        outDli.WriteValue("actor", nodeAnim.NodeName.c_str());
        outDli.WriteValue("property", "position");

        outDli.WriteObject("timePeriod", true);
        outDli.WriteValue("delay", 0.0);
        outDli.WriteValue("duration", animation->Duration * animation->TicksPerSecond);
        outDli.CloseScope();

        outDli.WriteArray("keyFrames");
        for (unsigned int k = 0; k < nodeAnim.Positions.size(); k++)
        {
          const NodeKey& nkey = nodeAnim.Positions[k];
          outDli.WriteObject(nullptr, true);
          outDli.WriteValue("progress", nkey.time / animation->Duration);
          outDli.WriteArray("value", true);
          WriteArrayData(nkey.v, 3, outDli);
          outDli.CloseScope();
          outDli.CloseScope();
        }

        outDli.CloseScope();
        outDli.CloseScope();
        outDli.CloseScope();
        outDli.WriteValue("loop", true);
        outDli.CloseScope();
      }

      if (nodeAnim.Scales.size())
      {
        outDli.WriteObject(nullptr);
        outDli.WriteArray("properties", true);
        outDli.WriteObject(nullptr);
        outDli.WriteValue("actor", nodeAnim.NodeName.c_str());
        outDli.WriteValue("property", "scale");

        outDli.WriteObject("timePeriod", true);
        outDli.WriteValue("delay", 0.0);
        outDli.WriteValue("duration", animation->Duration * animation->TicksPerSecond);
        outDli.CloseScope();

        outDli.WriteArray("keyFrames");
        for (unsigned int k = 0; k < nodeAnim.Scales.size(); k++)
        {
          NodeKey nkey = nodeAnim.Scales[k];
          outDli.WriteObject(nullptr, true);
          outDli.WriteValue("progress", nkey.time / animation->Duration);
          outDli.WriteArray("value", true);
          WriteArrayData(nkey.v, 3, outDli);
          outDli.CloseScope();
          outDli.CloseScope();
        }

        outDli.CloseScope();
        outDli.CloseScope();
        outDli.CloseScope();
        outDli.WriteValue("loop", true);
        outDli.CloseScope();
      }
    }

    outDli.CloseScope();
    outDli.CloseScope();
  }
}
