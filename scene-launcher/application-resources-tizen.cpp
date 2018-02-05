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

// CLASS HEADER
#include "application-resources.h"

// EXTERNAL INCLUDES
#include <app_common.h>

namespace
{
  const std::string IMAGES_PATH("images/");
  const std::string MODELS_PATH("models/");
  const std::string SHADERS_PATH("shaders/");
  const std::string LUA_SCRIPTS_PATH("lua-scripts/");
}

struct ApplicationResources::Impl
{
  Impl()
  {
  }

  ~Impl()
  {
  }

  std::string mResourcesPath;
  std::string mImagesPath;
  std::string mModelsPath;
  std::string mShadersPath;
  std::string mLuaScriptPath;
};

ApplicationResources& ApplicationResources::Get()
{
  static ApplicationResources applicationResources;
  return applicationResources;
}

const std::string& ApplicationResources::GetResourcesPath() const
{
  if( mImpl->mResourcesPath.empty() )
  {
    char* pathBuffer = app_get_resource_path();

    mImpl->mResourcesPath = std::string( pathBuffer );

    free( pathBuffer );
  }

  return mImpl->mResourcesPath;
}

const std::string& ApplicationResources::GetImagesPath() const
{
  if( mImpl->mImagesPath.empty() )
  {
    mImpl->mImagesPath = GetResourcesPath() + IMAGES_PATH;
  }

  return mImpl->mImagesPath;
}

const std::string& ApplicationResources::GetModelsPath() const
{
  if( mImpl->mModelsPath.empty() )
  {
    mImpl->mModelsPath = GetResourcesPath() + MODELS_PATH;
  }

  return mImpl->mModelsPath;
}

const std::string& ApplicationResources::GetShadersPath() const
{
  if( mImpl->mShadersPath.empty() )
  {
    mImpl->mShadersPath = GetResourcesPath() + SHADERS_PATH;
  }
  return mImpl->mShadersPath;
}

const std::string& ApplicationResources::GetLuaScriptsPath() const
{
  if( mImpl->mLuaScriptPath.empty() )
  {
    mImpl->mLuaScriptPath = GetResourcesPath() + LUA_SCRIPTS_PATH;
  }
  return mImpl->mLuaScriptPath;
}

ApplicationResources::ApplicationResources()
: mImpl( new ApplicationResources::Impl() )
{
}

ApplicationResources::~ApplicationResources()
{
  delete mImpl;
}

