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

#include <ModelExporter.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Scene3D.h"
#include "LoadScene.h"
#include "SaveScene.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>

using namespace std;

namespace {

std::string s_errorMessage;
std::string s_dliPath;

}

std::string GetInputName(JNIEnv* env, jstring inputFile)
{
  char const* inputFileChars = env->GetStringUTFChars(inputFile, nullptr);
  string fileName(inputFileChars, env->GetStringUTFLength(inputFile));

  env->ReleaseStringUTFChars(inputFile, inputFileChars);
  return fileName;
}

std::string GetOutputName(JNIEnv* env, jstring outputFile, char const* fallback)
{
  // output file -- default to the name of the input file, in case output file name wasn't supplied.
  // note: we could end up with ownership.
  char const* outputFileChars = fallback;
  size_t outputFileNameLen = 0;
  if(outputFile)
  {
      outputFileChars = env->GetStringUTFChars(outputFile, nullptr);
      outputFileNameLen = env->GetStringUTFLength(outputFile);
  }
  else
  {
      outputFileNameLen = strlen(outputFileChars);
  }

  auto pPeriod = strrchr(outputFileChars, '.');
  auto pSlash = strrchr(outputFileChars, '/');
  auto pBackslash = strrchr(outputFileChars, '\\');
  if (pPeriod != nullptr && std::max(pSlash, pBackslash) < pPeriod)
  {
      outputFileNameLen = pPeriod - outputFileChars;
  }

  string outName(outputFileChars, outputFileNameLen);
  if(outputFile)
  {
      env->ReleaseStringUTFChars(outputFile, outputFileChars);
  }

  return outName;
}

jint Java_com_samsung_dali_modelExporter_ModelExporter_nativeExport(JNIEnv* env, jclass clazz, jstring inputFile, jstring outputFile)
{
  s_dliPath = std::string();

  Assimp::Importer importer;

  string fileName = GetInputName(env, inputFile);
  const aiScene* scene = importer.ReadFile( fileName,
                                              aiProcess_CalcTangentSpace |
											  aiProcess_SortByPType );

  if( !scene )
  {
    s_errorMessage = importer.GetErrorString();
    return 1;
  }

  string outNameDli = GetOutputName(env, outputFile, fileName.c_str());
  string outNameBin = outNameDli + ".bin";
  outNameDli += ".dli";

  Scene3D scene_data;
  GetSceneNodes(scene_data, nullptr, scene, scene->mRootNode);
  GetSceneCameras(scene_data, scene);
  GetAnimations(scene_data, scene);
  SaveScene(&scene_data, outNameDli, outNameBin);

  s_dliPath = outNameDli;

  return 0;
}

jstring Java_com_samsung_dali_modelExporter_ModelExporter_nativeConvert(JNIEnv* env, jclass clazz, jstring inputFile, jstring binaryOutputFile)
{
  s_dliPath = std::string();

  Assimp::Importer importer;

  string fileName = GetInputName(env, inputFile);
  const aiScene* scene = importer.ReadFile( fileName,
                                              aiProcess_CalcTangentSpace |
                                              aiProcess_SortByPType );

  if( !scene )
  {
    s_errorMessage = importer.GetErrorString();
    return nullptr;
  }

  string outNameBin = GetOutputName(env, binaryOutputFile, fileName.c_str()) + ".bin";

  Scene3D scene_data;
  GetSceneNodes(scene_data, nullptr, scene, scene->mRootNode);
  GetSceneCameras(scene_data, scene);
  GetAnimations(scene_data, scene);

  std::ofstream binStream(outNameBin, ios::binary);
  if (binStream.is_open() && binStream.good())
  {
      std::ostringstream dliStream;
      jstring result = nullptr;
      if(ConvertScene(&scene_data, outNameBin, dliStream, binStream))
      {
          result = env->NewStringUTF(dliStream.str().c_str());
      }
      return result;
  }
  else
  {
      s_errorMessage = "Failed to open '" + outNameBin + "' for writing.";
      return nullptr;
  }
}

jstring Java_com_samsung_dali_modelExporter_ModelExporter_nativeGetErrorMessage(JNIEnv* env, jclass clazz)
{
    jstring result = nullptr;
    if(!s_errorMessage.empty())
    {
        result = env->NewStringUTF(s_errorMessage.c_str());
        std::string().swap(s_errorMessage);
    }
    return result;
}

jstring Java_com_samsung_dali_modelExporter_ModelExporter_nativeGetDliPath(JNIEnv* env, jclass clazz)
{
    jstring result = nullptr;
    if(!s_dliPath.empty())
    {
        result = env->NewStringUTF(s_dliPath.c_str());
    }
    return result;
}
