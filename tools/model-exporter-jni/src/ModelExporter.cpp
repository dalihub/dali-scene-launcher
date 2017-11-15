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
#include <iostream>
#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Scene3D.h"
#include "LoadScene.h"
#include "SaveScene.h"

using namespace std;

namespace {

std::string s_errorMessage;
std::string s_dliPath;

}

jint Java_com_samsung_dali_modelExporter_ModelExporter_nativeExport(JNIEnv* env, jclass clazz, jstring inputFile, jstring outputFile)
{
  s_dliPath = std::string();

  Assimp::Importer s_importer;

  char const* inputFileChars = env->GetStringUTFChars(inputFile, nullptr);
  string fileName(inputFileChars, env->GetStringUTFLength(inputFile));

  env->ReleaseStringUTFChars(inputFile, inputFileChars);

  const aiScene* scene = s_importer.ReadFile( fileName,
      aiProcess_CalcTangentSpace |
      aiProcess_SortByPType );

  if( !scene )
  {
    s_errorMessage = s_importer.GetErrorString();
    return 1;
  }

  Scene3D scene_data;

  GetSceneNodes(scene_data, nullptr, scene, scene->mRootNode);
  GetSceneCameras(scene_data, scene);
  GetAnimations(scene_data, scene);

  // output file -- default to the name of the input file, in case output file name wasn't supplied.
  // note: we could end up with ownership.
  char const* outputFileChars = fileName.c_str();
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
  printf("Writing to: %s\n", outputFileChars);
  fflush(stdout);

  string outNameDli(outputFileChars, outputFileNameLen);
  string outNameBin(outNameDli + ".bin");
  outNameDli += ".dli";

  if(outputFile)
  {
    env->ReleaseStringUTFChars(outputFile, outputFileChars);
  }

  SaveScene(&scene_data, outNameDli, outNameBin);

  s_dliPath = outNameDli;

  return 0;
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
