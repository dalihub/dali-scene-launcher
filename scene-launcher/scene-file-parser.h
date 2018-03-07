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

#ifndef DALI_SCENE_LAUNCHER_FILE_PARSER_H
#define DALI_SCENE_LAUNCHER_FILE_PARSER_H

//INTERNAL INCLUDES
#include "asset.h"

namespace SceneLauncher
{

class FileParser
{
public:

  /**
   * @brief Constructor
   *
   * Initialise variables
   */
  FileParser();

  /**
   * @brief Destructor
   *
   */
  ~FileParser();

  /**
   * @brief Parse the selected folder and sets the model file.
   *
   * The model file can be retrieved by calling GetModelFile()
   *
   * @param[in] modelDirUrl, path to folder.
   */
  void ReadModelFolder( const char* const modelDirUrl );

  /**
   * @brief Retrieves the current asset.
   *
   * @return An asset
   */
  Asset& GetAsset();

  /**
   * @return The current model file.
   */
  const std::string& GetModelFile() const;

  /**
   * @brief Parse the selected folder and sets the skin file
   *
   * The skin file can be retrieved by calling GetSkinFile()
   *
   * @param[in] skinUrl, path to folder.
   */
  void ReadSkinFolder( const char* const skinUrl );

  /**
   * @return The current skin file.
   */
  const std::string& GetSkinFile() const;

private:
  Asset mAsset;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_FILE_PARSER_H
