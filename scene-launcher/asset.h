#ifndef DALI_SCENE_LAUNCHER_ASSET_H
#define DALI_SCENE_LAUNCHER_ASSET_H


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

//EXTERNAL INCLUDES
#include <dali/public-api/math/matrix.h>
#include <dali/public-api/math/vector3.h>
#include <string>

using namespace Dali;

namespace SceneLauncher
{

struct CameraParameters
{
  CameraParameters()
  : cameraMatrix( Matrix::IDENTITY ),
    cameraOrthographicSize( -1.f, 1.f, 1.f, -1.f ),
    cameraFov( 60.f ),
    cameraNear( 0.1f ),
    cameraFar( 1000.f ),
    enablePerspective( true )
  {}

  ~CameraParameters()
  {}

  Matrix cameraMatrix;
  Vector4 cameraOrthographicSize;
  float cameraFov;
  float cameraNear;
  float cameraFar;
  bool enablePerspective;
};

struct LightParameters
{
  Matrix transform;
};

struct Script
{
  std::string url;
};

struct Asset
{
  Asset()
  : camera(),
    name(),
    dliPath(),
    albedoMetalness(),
    normalRoughness(),
    vertexShader(),
    fragmentShader(),
    cubeSpecular(),
    cubeDiffuse(),
    modelScaleFactor( Vector3::ONE ),
    MaxLOD( 8.f )
  {}

  ~Asset()
  {}

  CameraParameters camera;
  std::string name;
  std::string dliPath;
  std::string albedoMetalness;
  std::string normalRoughness;
  std::string vertexShader;
  std::string fragmentShader;
  std::string cubeSpecular;
  std::string cubeDiffuse;
  Vector3 modelScaleFactor;
  float MaxLOD;
  std::vector<Script> scripts;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_ASSET_H
