#ifndef DALI_SCENE_LAUNCHER_MODEL_PBR_H
#define DALI_SCENE_LAUNCHER_MODEL_PBR_H

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
#include <dali/public-api/animation/animation.h>
#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/rendering/shader.h>
#include <dali/public-api/rendering/texture-set.h>

// INTERNAL INCLUDES
#include "utils.h"

using namespace Dali;

namespace SceneLauncher
{

// Forward declarations
struct Asset;

class ModelPbr
{
public:
  enum CloneOptions: ::CloneOptions::Type
  {
    GET_SKYBOX_TEXTURE = ::CloneOptions::FIRST_USER_OPTION
  };

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
   * @param[in,out] asset Asset with the url of model. This function sets in the asset the camera parameters.
   * @param[in] position The position of the actor.
   * @param[out] animations Vector of array of animations from file.
   * @param[out] animationsName Vector of animations names to be loaded.
   */
  void Init( Asset& asset, const Vector3& position, std::vector<std::vector<Animation>>& animations, std::vector<std::string>& animationsName );

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
  bool GetUniform( const std::string& property, Property::Value& value, int shaderIndex );

  /**
   * @brief Set Shader uniform in all shaders it has this uniform name
   *
   * @param[in] property Uniform name.
   * @param[in] value To be set in the shaders.
   */
  void SetShaderUniform( const std::string& property, const Property::Value& value );

  /**
   * @brief Set Shader uniform in all shaders it has this uniform name with animation
   *
   * @param[in] property Uniform name.
   * @param[in] value To be set in the shaders.
   * @param[in] alpha Animation function.
   * @param[in] etime Animation period.
   */
  void SetShaderAnimationUniform( const std::string& property, const Property::Value& value, AlphaFunction alpha, TimePeriod etime );

  /**
   * @brief Retrieves the Skybox Texture from the file format
   *
   * @return Skybox texture.
   */
  Texture GetSkyboxTexture();

  /**
   * @brief Clone the actors and their renderers. Geometry and Shader references will be shared.
   * Optionally, the vector of shader references and the skybox texture may also be shared.
   * @param other The ModelPbr instance to duplicate into.
   * @param cloneOptions A bitmask constructed from any combination of ::CloneOptions::Values and
   *    ModelPbr::CloneOptions values.
   */
  void Duplicate(ModelPbr& other, ::CloneOptions::Type cloneOptions = ::CloneOptions::NONE) const;

  /**
   * @brief Adds the given texture to each actor's renderer's texture set.
   * @param texture Texture to attach.
   * @param sampler Sampler to associate with @a texture.
   */
  void AttachTexture(Texture texture, Sampler sampler);

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
  static Actor CreateNode( Shader shader, int blend, TextureSet textureSet, Geometry geometry, const Vector3& actorSize, const std::string& name );

private:

  std::vector<Shader> mShaderArray;
  Actor mActor;
  Texture mSkyboxTexture;
  static int mOrderIdx;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_MODEL_PBR_H

