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

#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/rendering/renderer.h>
#include <dali-toolkit/devel-api/builder/json-parser.h>
#include <dali-toolkit/devel-api/builder/builder.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
  const unsigned int MAX_ARRAY_PARAMETER_LENGTH = 256u;
} // namespace

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
    cameraOrthographicSize( -1.0f , 1.0f, 1.0f, -1.0f ),
    cameraFov( 60.f ),
    cameraNear( 0.1f ),
    cameraFar( 1000.f ),
    enablePerspective(true)
  {}

  ~DliCameraParameters()
  {}

  Matrix cameraMatrix;
  Vector4 cameraOrthographicSize;
  float cameraFov;
  float cameraNear;
  float cameraFar;
  bool enablePerspective;
};

class DliLoader
{
public:

  union Parameter
  {
    char arrayParameter[MAX_ARRAY_PARAMETER_LENGTH];
    int intParameter;
    float floatParameter;
  };

  struct Script
  {
    std::string url;
    std::vector<Parameter> parameters;
  };

  struct Event
  {
    std::string source;
    Script script;
  };

public:

  DliLoader();
  ~DliLoader();

  bool LoadObject( const std::string& modelUrl );

  bool CreateScene( std::vector<Shader>& shaderArray, Actor toActor, Texture& skyboxTexture );

  bool LoadAnimation( Actor toActor, std::vector<Animation> *animArray, const std::string& animationName );

  void GetCameraParameters( unsigned int eidx, DliCameraParameters* camera );

  std::string GetParseError() const;

  const std::vector<Event>& GetEvents() const;

private:

  bool LoadTextureSetArray( Texture& eCubeSpecular );

  bool LoadShaderArray( std::vector<Shader>& shaderArray );

  bool LoadGeometryArray();

  void AddNode( Actor toActor, const TreeNode* addnode );

  void LoadEvents();

  void CreateSkyboxTexture( const std::string& skyBoxTexturePath, Texture& skyboxTexture );

  void CreateEnvironmentTextures( const std::string& cubeDiffuse, const std::string& cubeSpecular, Texture& eDiffuseTexture, Texture& eSpecularTexture );

  void CreateTextures( std::string strTexture[4], Texture eTexture[4], bool createMipmaps );

  bool LoadBuffer(const TreeNode* mesh, Geometry geometry, std::string& ebinFilename, unsigned char*& efileContent);

  void ReadAnglePosition(const TreeNode* node, Actor &actor);

  JsonParser mParser;
  const TreeNode *mNodes;
  std::vector<Geometry> mGeometryArray;
  std::vector<TextureSet> mTextureSetArray;
  std::vector<RendererOptions> mRendererOptionsArray;
  std::vector<Shader>* mShaderArrayPtr;
  std::vector<Event> mEvents;
  std::string mDirectory;

};

} // namespace SceneLauncher



#endif // DALI_SCENE_LAUNCHER_DLI_LOADER_H
