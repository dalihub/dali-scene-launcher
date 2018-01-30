#ifndef DALI_SCENE_LAUNCHER_DLI_LOADER_H
#define DALI_SCENE_LAUNCHER_DLI_LOADER_H

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

#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/rendering/renderer.h>
#include <dali-toolkit/devel-api/builder/json-parser.h>
#include <dali-toolkit/devel-api/builder/builder.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace SceneLauncher
{
struct RendererOptions
{
  RendererOptions()
  : blend( -1 )
  {}

  ~RendererOptions()
  {}

  int blend;
};

struct DliCameraParameters
{
  DliCameraParameters()
  : cameraMatrix( Matrix::IDENTITY ),
    cameraFov( 60.f ),
    cameraNear( 0.1f ),
    cameraFar( 1000.f )
  {}

  ~DliCameraParameters()
  {}

  Matrix cameraMatrix;
  float cameraFov;
  float cameraNear;
  float cameraFar;
};

class DliLoader
{
public:
  DliLoader();
  ~DliLoader();

  void GetCameraParameters( unsigned int eidx, DliCameraParameters* camera );

  bool LoadObject( const std::string& modelUrl );

  std::string GetParseError() const;

  bool CreateScene( std::vector<Shader>& shaderArray, Actor toActor, Texture& skyboxTexture );

  bool LoadAnimation( Actor toActor, std::vector<Animation> *animArray, const std::string& animationName );

private:

  void AddNode( Actor toActor, const TreeNode* addnode );

  bool LoadBuffer(const TreeNode* mesh, Geometry geometry, std::string& ebinFilename, unsigned char*& efileContent);
  void CreateTextures( std::string strTexture[4], Texture eTexture[4] );
  void CreateEnvironmentTextures( const std::string& cubeDiffuse, const std::string& cubeSpecular, Texture& eDiffuseTexture, Texture& eSpecularTexture );
  void CreateSkyboxTexture( const std::string& skyBoxTexturePath, Texture& skyboxTexture );

  bool LoadShaderArray( std::vector<Shader>& shaderArray );
  bool LoadTextureSetArray( Texture& eCubeSpecular );
  bool LoadGeometryArray();

  JsonParser mParser;
  const TreeNode *mNodes;
  std::vector<Geometry> mGeometryArray;
  std::vector<TextureSet> mTextureSetArray;
  std::vector<RendererOptions> mRendererOptionsArray;
  std::vector<Shader>* mShaderArrayPtr;
  std::string mDirectory;

};

} // namespace SceneLauncher



#endif // DALI_SCENE_LAUNCHER_DLI_LOADER_H
