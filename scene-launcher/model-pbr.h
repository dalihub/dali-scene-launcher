#ifndef DALI_SCENE_LAUNCHER_MODELPBR_H
#define DALI_SCENE_LAUNCHER_MODELPBR_H

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
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/rendering/renderer.h>
#include <dali-toolkit/dali-toolkit.h>

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
   * @param[in] shader The Physically Based Rendering shader.
   * @param[in] modelUrl The url of model.
   * @param[in] position The position of the actor.
   * @param[in] size The size of the actor.
   */
  void Init( const std::string& modelUrl, const Vector3& position, const Vector3& size, std::vector<Animation> *loadAnimation );

  /**
   * @brief Initializes the @p mTextureSet member with the needed textures for Physically Based Rendering.
   *
   * @param[in] albedoMetalTexture The albedo metal texture.
   * @param[in] normalRoughTexture The normal rough texture.
   * @param[in] diffuseTexture The diffuse cubemap texture.
   * @param[in] specularTexture The specular cubemap texture.
   */
  void InitPbrTexture( Texture albedoMetalTexture, Texture normalRoughTexture, Texture diffuseTexture, Texture specularTexture );

  /**
   * @brief Clears the previously allocated PBR model resources.
   */
  void Clear();

  /**
   * @brief Retrieves the actor created by calling the Init() method.
   *
   * @return The Actor for the Physically Based Rendering.
   */
  Actor &GetActor();

  void SetShaderUniform(std::string property, const Property::Value& value);
  Texture GetCubeSpecularTexture();

  static Actor CreateNode( Shader shader, int blend, TextureSet textureSet, Geometry geometry, std::string name );

private:

  /**
   * @brief Creates a geometry from a @e obj model.
   *
   * @param[in] url A url pointing a file with a @e obj model.
   * @param[out] geometry an array of Geometry, each element correspond to a group in OBJ file
   * @param[out] names, array of string that correspond to name of groups in OBJ file
   */
  void CreateGeometry( const std::string& url,
                       std::vector<Geometry>& geometry,
                       std::vector<std::string>& names );

  Actor mActor;
  std::vector<Shader> mShaderArray;
  Texture mCubeSpecularTexture;
  TextureSet mTextureSet;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_MODELPBR_H

