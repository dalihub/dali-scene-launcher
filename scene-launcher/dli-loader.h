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
#include <dali-toolkit/devel-api/builder/json-parser.h>
#include <dali-toolkit/devel-api/builder/builder.h>
#include <dali/public-api/rendering/geometry.h>
#include <dali/public-api/actors/actor.h>
#include <dali/public-api/rendering/renderer.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace SceneLauncher
{
struct RendererOptions
{
  RendererOptions()
  : blend( false )
  {}

  ~RendererOptions()
  {}

  bool blend;
};

struct DliCameraParameters
{
  DliCameraParameters()
  : cameraPosition( 0.f, 0.f, 3.5f ),
    cameraFov( 60.f ),
    cameraNear( 0.1f ),
    cameraFar( 1000.f )
  {}

  ~DliCameraParameters()
  {}

  Vector3 cameraPosition;
  float cameraFov;
  float cameraNear;
  float cameraFar;
};

class DliLoader
{
public:

  DliLoader();
  ~DliLoader();
  static void getCameraParameters(const TreeNode *rootParser, unsigned int eidx, DliCameraParameters &camera);

  bool LoadObject( std::string modelUrl );

  std::string getParseError();

  bool CreateScene( std::vector<Shader> &shaderArray, Actor toActor, Texture specularTexture );


private:

  void AddNode( Actor toActor, const TreeNode *addnode );

  bool LoadBuffer(const TreeNode* mesh, Geometry geometry, std::string& ebinFilename, unsigned char*& efileContent);
  void CreateTextures(std::string strTexture[4], Texture eTexture[4] );
  void CreateEnvironmentTextures( std::string cubeDiffuse, std::string cubeSpecular, Texture &eDiffuseTexture, Texture &eSpecularTexture );

  void getShaders(const TreeNode *rootParser, unsigned int eidx, std::string &vertex, std::string &fragment );

  void getMaterial(const TreeNode *rootParser, unsigned int eidx, std::string &albedoMetal, std::string &normalRoughness );

  void getEnvironment(const TreeNode *rootParser, std::string &cubeDdiffuse, std::string &cubeSpecular, float &MaxLOD );

  bool LoadShaderArray(std::vector<Shader> &shaderArray);
  bool LoadTextureSetArray( Texture &eCubeSpecular );
  bool LoadGeometryArray();
  bool LoadAnimations( Actor toActor );

  JsonParser mParser;
  const TreeNode *mNodes;
  std::vector<Geometry> mGeometryArray;
  std::vector<TextureSet> mTextureSetArray;
  std::vector<RendererOptions> mRendererOptionsArray;
  std::vector<Shader> *mShaderArrayPtr;
  std::string mDirectory;
  float mMaxLOD;

};

} // namespace SceneLauncher



#endif // DALI_SCENE_LAUNCHER_DLI_LOADER_H
