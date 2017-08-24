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

#ifndef DALI_DEMO_SCENE_FILE_PARSER_H
#define DALI_DEMO_SCENE_FILE_PARSER_H

#include <dali-toolkit/devel-api/builder/json-parser.h>

using namespace Dali;
using namespace Dali::Toolkit;


namespace PbrDemo
{

class ModelPbr;

struct Asset
{
  Asset()
  : hourActors(),
    minActors(),
    secActors(),
    name(),
    model(),
    albedoMetalness(),
    normalRoughness(),
    vertexShader(),
    fragmentShader(),
    cubeSpecular(),
    cubeDiffuse(),
    modelScaleFactor( Vector3::ONE ),
    cameraPosition( Vector3( 0.f, 0.f, 1.5f ) ),
    cameraFov( 60.f ),
    cameraNear( 0.1f ),
    cameraFar( 1000.f ),
    MaxLOD( 8 ),
    objModel(true)
  {}

  ~Asset()
  {}

  std::vector<std::string> hourActors;
  std::vector<std::string> minActors;
  std::vector<std::string> secActors;
  std::string name;
  std::string model;
  std::string albedoMetalness;
  std::string normalRoughness;
  std::string vertexShader;
  std::string fragmentShader;
  std::string cubeSpecular;
  std::string cubeDiffuse;
  Vector3 modelScaleFactor;
  Vector3 cameraPosition;
  float cameraFov;
  float cameraNear;
  float cameraFar;
  float MaxLOD;
  bool objModel;
};

class SceneFileParser
{

private:

  /**
   * @brief Insensitive case compare function.
   *
   * @param[in] a, compare string
   * @param[in] b, compare string
   * @return true if strings are equal
   */
  static bool CaseInsensitiveCharacterCompare( unsigned char a, unsigned char b );

  /**
   * @brief return true if the lower cased ASCII strings are equal.
   *
   * @param[in] a, compare string
   * @param[in] b, compare string
   * @return true if strings are equal
   */
  bool CaseInsensitiveStringCompare( const std::string& a, const std::string& b );

  /**
   * @brief get a vector3 from parsed node
   *
   * @param[in] node, parsed node
   * @param[out] vector3, vector read.
   */
  void ParseVector3( const TreeNode& node, Vector3& vector3 );

  /**
   * @brief From a list of files, get the assets from the corresponding index file.
   *
   * @param[in] index, index of file
   */
  void ParseModelFile( unsigned int index );


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
   * @brief Parse the selected folder and load the first file
   *
   * @param[out] modelDirUrl, path to folder.
   */
  void ReadPbrModelFolder( const char* const modelDirUrl );

  /**
   * @return The current model file.
   */
  std::string GetCurrentModelFile() const;

private:
  unsigned int mModelFileIndex;
  unsigned int mViewModel;
  std::vector<std::string> mPbrModelFiles;
  std::vector<Asset> mAssets;
};

} // namespace PbrDemo

#endif /* DALI_DEMO_EXAMPLES_SCENE_LAUNCHER_SCENE_FILE_PARSER_H_ */
