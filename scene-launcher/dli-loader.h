#ifndef DALI_SCENE_LAUNCHER_DLI_LOADER_H
#define DALI_SCENE_LAUNCHER_DLI_LOADER_H

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
#include <dali/public-api/animation/animation.h>
#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/rendering/shader.h>
#include <dali/public-api/rendering/texture-set.h>

#include <dali-toolkit/devel-api/builder/json-parser.h>

// INTERNAL INCLUDES
#include "asset.h"
#include "lighting-mode.h"

using namespace Dali;
using namespace Dali::Toolkit;

namespace SceneLauncher
{

// Forward declarations
struct CameraParameters;

struct RendererOptions
{
  RendererOptions()
  : blend( -1 )
  {}

  ~RendererOptions()
  {}

  int blend;
};

class DliLoader
{
public:
  static const std::string LIGHTING_MODE_PROPERTY;

public:

  ///@brief Given a path @a modelDirUrl, return the full path of the first .dli we find, or throw
  /// a DaliException if there is none.
  static std::string GetFirstDliInFolder(const char* const modelDirUrl);

  DliLoader();
  ~DliLoader();

  bool LoadObject( const std::string& modelUrl );

  bool CreateScene( std::vector<Shader>& shaderArray, Actor toActor, Texture& skyboxTexture );

  bool LoadAnimation( Actor toActor, std::vector<Animation>& animArray, const std::string& animationName );

  void GetCameraParameters( unsigned int eidx, CameraParameters& camera );

  std::string GetParseError() const;

  const std::vector<Script>& GetScripts() const;

private:

  bool LoadTextureSetArray( Texture& eCubeSpecular );

  bool LoadShaderArray( std::vector<Shader>& shaderArray );

  bool LoadGeometryArray();

  void AddNode( Actor toActor, const TreeNode* addnode, const std::vector<Shader>& shaderArray, LightingMode::Type shadowMode );

  void LoadScripts();

  void CreateSkyboxTexture( const std::string& skyBoxTexturePath, Texture& skyboxTexture );

  void CreateEnvironmentTextures( const std::string& cubeDiffuse, const std::string& cubeSpecular, Texture& eDiffuseTexture, Texture& eSpecularTexture );

  void CreateTextures( std::string strTexture[4], Texture eTexture[4], bool createMipmaps );

  bool LoadBuffer(const TreeNode* mesh, Geometry geometry, std::string& ebinFilename, unsigned char*& efileContent);

  void ReadAnglePosition(const TreeNode* node, Actor& actor);

  std::vector<Geometry> mGeometryArray;
  std::vector<TextureSet> mTextureSetArray;
  std::vector<RendererOptions> mRendererOptionsArray;
  std::vector<Script> mScripts;
  const TreeNode* mNodes;
  JsonParser mParser;
  std::string mDirectory;

};

} // namespace SceneLauncher



#endif // DALI_SCENE_LAUNCHER_DLI_LOADER_H
