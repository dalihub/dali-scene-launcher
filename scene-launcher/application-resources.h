#ifndef DALI_SCENE_LAUNCHER_APPLICATION_RESOURCES_H
#define DALI_SCENE_LAUNCHER_APPLICATION_RESOURCES_H

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

// EXTERNAL INCLUDES
#include <string>

class ApplicationResources
{
public:

  static ApplicationResources& Get();

  const std::string& GetResourcesPath() const;
  const std::string& GetImagesPath() const;
  const std::string& GetModelsPath() const;
  const std::string& GetShadersPath() const;
  const std::string& GetLuaScriptsPath() const;
  const std::string& GetSkinsPath() const;

private:
  ApplicationResources();
  ApplicationResources(ApplicationResources const& other); //= delete;
  ~ApplicationResources();

  struct Impl;
  Impl* mImpl;
};

#endif // DALI_SCENE_LAUNCHER_APPLICATION_RESOURCES_H
