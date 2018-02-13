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

#ifndef DALI_DEMO_SCENE_FILE_PARSER_H
#define DALI_DEMO_SCENE_FILE_PARSER_H

//EXTERNAL INCLUDES
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/math/matrix.h>
#include <dali/public-api/math/vector3.h>
#include <string>

using namespace Dali;

namespace SceneLauncher
{

// Forward declarations
struct DliCameraParameters;

struct Asset
{
  Asset()
  : name(),
    model(),
    albedoMetalness(),
    normalRoughness(),
    vertexShader(),
    fragmentShader(),
    cubeSpecular(),
    cubeDiffuse(),
    cameraMatrix( Matrix::IDENTITY ),
    cameraOrthographicSize( -1.0f, 1.0f, 1.0f, -1.0 ),
    modelScaleFactor( Vector3::ONE ),
    cameraFov( 60.f ),
    cameraNear( 0.1f ),
    cameraFar( 1000.f ),
    MaxLOD( 8.f ),
    objModel(true),
    enablePerspective(true)
  {}

  ~Asset()
  {}

  std::string name;
  std::string model;
  std::string albedoMetalness;
  std::string normalRoughness;
  std::string vertexShader;
  std::string fragmentShader;
  std::string cubeSpecular;
  std::string cubeDiffuse;
  Matrix cameraMatrix;
  Vector4 cameraOrthographicSize;
  Vector3 modelScaleFactor;
  float cameraFov;
  float cameraNear;
  float cameraFar;
  float MaxLOD;
  bool objModel;
  bool enablePerspective;
};

class SceneFileParser
{
public:

  /**
   * @brief Constructor
   *
   * Initialise variables
   */
  SceneFileParser();

  /**
   * @brief Destructor
   *
   */
  ~SceneFileParser();

  /**
   * @brief Parse the selected folder and load the first file
   *
   * @param[out] modelDirUrl, path to folder.
   */
  void ReadModelFolder( const char* const modelDirUrl );

  /**
   * @brief Get assets from the next array asset or next files according to the case
   */
  void LoadNextModel();

  /**
   * @brief Retrieves the current asset.
   *
   * @return An asset
   */
  const Asset& GetAsset() const;

  /**
   * @return The current model file.
   */
  const std::string& GetCurrentModelFile() const;

  /**
   * @brief Set camera parameters.
   *
   */
  void SetCameraParameters(const DliCameraParameters& camera);

private:
  std::vector<Asset> mAssets;
  unsigned int mViewModel;
};

} // namespace SceneLauncher

#endif // DALI_DEMO_SCENE_FILE_PARSER_H
