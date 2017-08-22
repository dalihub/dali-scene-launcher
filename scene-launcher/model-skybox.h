#ifndef DALI_SCENE_LAUNCHER_MODELSKYBOX_H
#define DALI_SCENE_LAUNCHER_MODELSKYBOX_H

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

// EXTERNAL INCLUDES
#include <dali/public-api/actors/actor.h>
#include <dali/public-api/rendering/renderer.h>

using namespace Dali;

class ModelSkybox
{
public:
  /**
   * @brief Constructor.
   * Does nothing.
   */
  ModelSkybox();

  /**
   * @brief Destructor
   * Does nothing.
   */
  ~ModelSkybox();

  /**
   * @brief Initializes an Actor fro Skybox model.
   *
   * @note Call InitTexture() before calling this method.
   *
   * A renderer is created with the geometry and the shader and it's set to the an actor that can be retrieved by calling GetActor().
   */
  void Init();

  /**
   * @brief Initializes the @p mTextureSet member with the needed texture.
   *
   * @param[in] texSkybox
   */
  void InitTexture( Texture texSkybox );

  /**
   * @brief Clears the previously allocated skybox resources.
   */
  void Clear();

  /**
   * @brief Retrieves the actor created by calling the Init() method.
   *
   * @return The Actor for the Physically Based Rendering.
   */
  Actor &GetActor();

private:

  /**
   * @brief Creates a cube geometry including texture coordinates.
   *
   * @return The geometry.
   */
  Geometry CreateGeometry();

  Actor mActor;
  TextureSet mTextureSet;
};

#endif // DALI_SCENE_LAUNCHER_MODELSKYBOX_H

