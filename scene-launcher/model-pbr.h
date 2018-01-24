#ifndef DALI_SCENE_LAUNCHER_MODELPBR_H
#define DALI_SCENE_LAUNCHER_MODELPBR_H

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
#include <dali/public-api/actors/actor.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/rendering/renderer.h>
#include <dali-toolkit/dali-toolkit.h>

// INTERNAL INCLUDES
#include "dli-loader.h"

using namespace Dali;

namespace SceneLauncher
{

class ModelPbr
{
public:
  /**
   * @brief Constructor.
   * Does nothing.
   */
  ModelPbr();

  /**
   * @brief Destructor
   * Does nothing.
   */
  ~ModelPbr();

  /**
   * @brief Initializes an Actor and its geometry for the Physically Based Rendering.
   *
   * @note Call InitTexture() before calling this method.
   *
   * It creates a geometry for the renderer.
   * The @p modelUrl parameter needs to be set with a valid url pointing a file with a @e obj model.
   *
   * A renderer is created with the geometry and the shader and it's set to the an actor that can be retrieved by calling GetActor().
   * The @p position and @p size is set to the actor.
   *
   * @param[in] modelUrl The url of model.
   * @param[in] position The position of the actor.
   * @param[in] size The size of the actor.
   * @param[in] camera pointer to load the camera from file.
   * @param[in] pointer to an array of array of animations from file.
   * @param[in] pointer to an array animations names to be loaded.
   */
  void Init( const std::string& modelUrl, const Vector3& position, const Vector3& size, DliCameraParameters *camera, std::vector<std::vector<Animation>> *animations, std::vector<std::string> *animationsName );

  /**
   * @brief Clears the previously allocated PBR model resources.
   */
  void Clear();

  /**
   * @brief Retrieves the actor created by calling the Init() method.
   *
   * @return The Actor for the Physically Based Rendering.
   */
  Actor& GetActor();

  /**
   * @brief Retrieves the Uniform value
   *
   * @param[in] property Uniform name.
   * @param[out] value The value will be saved in this variable.
   * @param[in] shaderIndex, It is the shader index to get the uniform value,
   *            if shaderIndex is -1, then find the uniform in any shader and
   *            return the first value it finds.
   *
   * @return true if uniforms exist in shader, else return false.
   */
  bool GetUniform(std::string property, Property::Value& value, int shaderIndex );

  /**
   * @brief Set Shader uniform in all shaders it has this uniform name
   *
   * @param[in] property Uniform name.
   * @param[in] value To be set in the shaders.
   */
  void SetShaderUniform( std::string property, const Property::Value& value );

  /**
   * @brief Set Shader uniform in all shaders it has this uniform name with animation
   *
   * @param[in] property Uniform name.
   * @param[in] value To be set in the shaders.
   * @param[in] alpha Animation function.
   * @param[in] etime Animation period.
   */
  void SetShaderAnimationUniform( std::string property, const Property::Value& value, AlphaFunction alpha, TimePeriod etime );

  /**
   * @brief Retrieves the Skybox Texture from the file format
   *
   * @return Skybox texture.
   */
  Texture GetSkyboxTexture();

  /**
   * @brief Create a node with actor, mesh and renderer
   *
   * @param[in] shader used for this node.
   * @param[in] blend rendering mode.
   * @param[in] textureSet textures used for this node.
   * @param[in] geometry.
   * @param[in] actorSize, used to set the boundary box for touch/click events for this node.
   * @param[in] name of the node.
   */
  static Actor CreateNode( Shader shader, int blend, TextureSet textureSet, Geometry geometry, Vector3 actorSize, const std::string& name );

private:

  static int mOrderIdx;
  Actor mActor;
  std::vector<Shader> mShaderArray;
  Texture mSkyboxTexture;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_MODELPBR_H

