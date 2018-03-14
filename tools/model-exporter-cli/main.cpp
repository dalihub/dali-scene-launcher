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

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Scene3D.h"
#include "LoadScene.h"
#include "SaveScene.h"

using namespace std;

int main(int argc, char* argv[])
{
  Assimp::Importer importer;
  const aiScene *scene;
  string fileName;
  if(argc>1)
  {
    fileName = argv[1];
  }

  scene = importer.ReadFile( fileName,
                             aiProcess_CalcTangentSpace |
                             aiProcess_SortByPType );

  if( !scene )
  {
    cout << importer.GetErrorString()<< endl;
    return false;
  }

  Scene3D scene_data;

  GetSceneNodes(scene_data, nullptr, scene, scene->mRootNode);
  GetSceneCameras(scene_data, scene);
  GetSceneLights(scene_data, scene);
  GetAnimations(scene_data, scene);

  string fname("TestWatch_007.dli");
  string fnamebin("TestWatch_007.bin");

  fname = fileName.substr(0, fileName.length()-4) + ".dli";
  fnamebin = fileName.substr(0, fileName.length()-4) + ".bin";
  SaveScene(&scene_data,fname,fnamebin);
  return 0;
}
