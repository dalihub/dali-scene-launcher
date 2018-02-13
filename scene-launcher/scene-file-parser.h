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

//INTERNAL INCLUDES
#include "asset.h"

namespace SceneLauncher
{

// Forward declarations
struct DliCameraParameters;

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
  Asset& GetAsset();

  /**
   * @return The current model file.
   */
  const std::string& GetCurrentModelFile() const;

private:
  Asset mAsset;
};

} // namespace SceneLauncher

#endif // DALI_DEMO_SCENE_FILE_PARSER_H
