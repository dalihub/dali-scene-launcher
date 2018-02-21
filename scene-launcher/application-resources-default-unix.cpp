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
    mImpl->mResourcesPath = SCENE_LAUNCHER_RESOURCE_DIR;
  }

  return mImpl->mResourcesPath;
}

const std::string& ApplicationResources::GetImagesPath() const
{
  if( mImpl->mImagesPath.empty() )
  {
    mImpl->mImagesPath = SCENE_LAUNCHER_IMAGE_DIR;
  }

  return mImpl->mImagesPath;
}

const std::string& ApplicationResources::GetModelsPath() const
{
  if( mImpl->mModelsPath.empty() )
  {
    mImpl->mModelsPath = SCENE_LAUNCHER_MODEL_DIR;
  }

  return mImpl->mModelsPath;
}

const std::string& ApplicationResources::GetShadersPath() const
{
  if( mImpl->mShadersPath.empty() )
  {
    mImpl->mShadersPath = SCENE_LAUNCHER_SHADER_DIR;
  }
  return mImpl->mShadersPath;
}

ApplicationResources::ApplicationResources()
: mImpl( new ApplicationResources::Impl() )
{
}

ApplicationResources::~ApplicationResources()
{
  delete mImpl;
}

