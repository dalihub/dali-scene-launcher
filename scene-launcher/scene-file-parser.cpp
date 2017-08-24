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
#include <dali-toolkit/dali-toolkit.h>

#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>

#include "scene-file-parser.h"
#include "model-pbr.h"
#include "dli-loader.h"

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{

const Vector3 MODEL_DEFAULT_SCALE( Vector3::ONE );
const Vector3 SKYBOX_DEFAULT_SCALE( Vector3::ONE );

const float CAMERA_DEFAULT_FOV(    60.0f );
const float CAMERA_DEFAULT_NEAR(    0.1f );
const float CAMERA_DEFAULT_FAR(  1000.0f );
const Vector3 CAMERA_DEFAULT_POSITION( 0.0f, 0.0f, 3.5f );

const std::string NAME_TOKEN( "name" );
const std::string MODEL_TOKEN( "model" );
const std::string MODEL_SCALE_FACTOR_TOKEN( "model-scale-factor" );
const std::string ALBEDO_METALNESS_TOKEN( "albedo-metalness" );
const std::string NORMAL_ROUGHNESS_TOKEN( "normal-roughness" );
const std::string VERTEX_SHADER_TOKEN( "vertex-shader" );
const std::string FRAGMENT_SHADER_TOKEN( "fragment-shader" );
const std::string CUBEMAP_SPECULAR_TOKEN( "cubemap-specular" );
const std::string CUBEMAP_DIFFUSE_TOKEN( "cubemap-diffuse" );
const std::string CAMERA_DEFAULT_FOV_TOKEN( "camera-default-fov" );
const std::string CAMERA_DEFAULT_NEAR_TOKEN( "camera-default-near" );
const std::string CAMERA_DEFAULT_FAR_TOKEN( "camera-default-far" );
const std::string CAMERA_DEFAULT_POSITION_TOKEN( "camera-default-position" );

}  // namespace

namespace SceneLauncher
{

SceneFileParser::SceneFileParser()
: mModelFileIndex( 0u ),
  mViewModel( 0u )
{

}

SceneFileParser::~SceneFileParser()
{

}

bool SceneFileParser::CaseInsensitiveCharacterCompare( unsigned char a, unsigned char b )
{
  // Converts to lower case in the current locale.
  return std::tolower( a ) == std::tolower( b );
}


bool SceneFileParser::CaseInsensitiveStringCompare( const std::string& a, const std::string& b )
{
  bool result = false;
  if( a.length() == b.length() )
  {
    result = std::equal( a.begin(), a.end(), b.begin(), &SceneFileParser::CaseInsensitiveCharacterCompare );
  }
  return result;
}

void SceneFileParser::ReadPbrModelFolder( const char* const modelDirUrl )
{
  DIR *dp;
  dp = opendir( modelDirUrl );

  if( NULL == dp )
  {
    std::stringstream stream;
    stream << "Can't open " << std::string( modelDirUrl ) << " folder.";

    // Error while opening the folder.
    throw DaliException( ASSERT_LOCATION, stream.str().c_str() );
  }

  const std::string pbrExt = ".dalipbr";
  const size_t pbrExtSize = pbrExt.size();
  const std::string slash( "/" );

  struct dirent* dirp;
  while( ( dirp = readdir( dp ) ) )
  {
    std::string filePath = std::string( modelDirUrl ) + slash + dirp->d_name;

    struct stat fileStat;
    if( stat( filePath.c_str(), &fileStat ) || S_ISDIR( fileStat.st_mode ))
    {
      // Do nothing if it fails to retrieve the file info or if the file is a folder.
      continue;
    }

    if( filePath.size() > pbrExtSize )
    {
      const std::string fileExtension = filePath.substr( filePath.size() - pbrExtSize );
      if( CaseInsensitiveStringCompare( fileExtension, pbrExt ) )
      {
        mPbrModelFiles.push_back( filePath );
      }
    }
  }

  if( !mPbrModelFiles.empty() )
  {
    ParseModelFile( 0u );
  }
}

void SceneFileParser::ParseVector3( const TreeNode& node, Vector3& vector3 )
{
  unsigned int offset = 0u;
  for( TreeNode::ConstIterator it = node.CBegin(), endIt = node.CEnd(); it != endIt; ++it, ++offset )
  {
    const TreeNode& coord = (*it).second;
    vector3.AsFloat()[offset] = coord.GetFloat();
  }
}

void SceneFileParser::ParseModelFile( unsigned int fileIndex )
{
  if( fileIndex >= mPbrModelFiles.size() )
  {
    std::stringstream stream;
    stream << "Model index "<< fileIndex << " out of bounds.";

    throw DaliException( ASSERT_LOCATION, stream.str().c_str() );
  }

  std::string modelUrl = mPbrModelFiles[fileIndex];
  std::ifstream ifs( modelUrl.c_str() );
  std::string buffer( ( std::istreambuf_iterator<char>( ifs ) ),
                      ( std::istreambuf_iterator<char>() ) );

  JsonParser parser = JsonParser::New();
  const bool result = parser.Parse( buffer );

  mAssets.clear();

  if( !result )
  {
    std::stringstream stream;

    if( parser.ParseError() )
    {
      stream << "position: " << parser.GetErrorPosition() << ", line: " << parser.GetErrorLineNumber() << ", column: " << parser.GetErrorColumn() << ", description: " << parser.GetErrorDescription() << ".";
    }

    throw DaliException( ASSERT_LOCATION, stream.str().c_str() );
  }

  const TreeNode& root = ( *parser.GetRoot()->CBegin() ).second;

  mAssets.resize( root.Size() );

  unsigned int assetIndex = 0u;
  for( TreeNode::ConstIterator modelIt = root.CBegin(), modelEndIt = root.CEnd(); modelIt != modelEndIt; ++modelIt, ++assetIndex )
  {
    const TreeNode& modelNode = (*modelIt).second;
    SceneLauncher::Asset& asset = mAssets[assetIndex];

    for( TreeNode::ConstIterator it = modelNode.CBegin(), endIt = modelNode.CEnd(); it != endIt; ++it )
    {
      const TreeNode& node = (*it).second;

      const std::string name = node.GetName();

      if( CaseInsensitiveStringCompare( name, NAME_TOKEN ) )
      {
        asset.name = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, MODEL_TOKEN ) )
      {
        asset.model = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, MODEL_SCALE_FACTOR_TOKEN ) )
      {
        ParseVector3( node, asset.modelScaleFactor );
      }
      else if( CaseInsensitiveStringCompare( name, ALBEDO_METALNESS_TOKEN ) )
      {
        asset.albedoMetalness = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, NORMAL_ROUGHNESS_TOKEN ) )
      {
        asset.normalRoughness = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, VERTEX_SHADER_TOKEN ) )
      {
        asset.vertexShader = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, FRAGMENT_SHADER_TOKEN ) )
      {
        asset.fragmentShader = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, CUBEMAP_SPECULAR_TOKEN ) )
      {
        asset.cubeSpecular = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, CUBEMAP_DIFFUSE_TOKEN ) )
      {
        asset.cubeDiffuse = node.GetString();
      }
      else if( CaseInsensitiveStringCompare( name, CAMERA_DEFAULT_FOV_TOKEN ) )
      {
        asset.cameraFov = node.GetFloat();
      }
      else if( CaseInsensitiveStringCompare( name, CAMERA_DEFAULT_NEAR_TOKEN ) )
      {
        asset.cameraNear = node.GetFloat();
      }
      else if( CaseInsensitiveStringCompare( name, CAMERA_DEFAULT_FAR_TOKEN ) )
      {
        asset.cameraFar = node.GetFloat();
      }
      else if( CaseInsensitiveStringCompare( name, CAMERA_DEFAULT_POSITION_TOKEN ) )
      {
        ParseVector3( node, asset.cameraPosition );
      }
    }
    if( ( asset.model.rfind(".dli") + 4) == asset.model.length())
    {
      std::ifstream dalifs( std::string( SCENE_LAUNCHER_MODEL_DIR ) + asset.model.c_str() );
      std::string daliBuffer( ( std::istreambuf_iterator<char>( dalifs ) ),
                          ( std::istreambuf_iterator<char>() ) );

      JsonParser daliParser = JsonParser::New();
      const bool result_format = daliParser.Parse( daliBuffer );
      if( !result_format )
      {
        std::stringstream stream;

        if( daliParser.ParseError() )
        {
          stream << "position: " << daliParser.GetErrorPosition() << ", line: " << daliParser.GetErrorLineNumber() << ", column: " << daliParser.GetErrorColumn() << ", description: " << daliParser.GetErrorDescription() << ".";
        }

        throw DaliException( ASSERT_LOCATION, stream.str().c_str() );
      }
      const TreeNode *root = daliParser.GetRoot();
      DliCameraParameters camera;
      DliLoader::getCameraParameters( root, 0, camera );
      asset.cameraFar = camera.cameraFar;
      asset.cameraNear = camera.cameraNear;
      asset.cameraFov = camera.cameraFov;
      asset.cameraPosition = camera.cameraPosition;
      asset.objModel = false;
    }
  }
}

void SceneFileParser::LoadNextModel()
{
  ++mViewModel;
  if( mViewModel >= mAssets.size() )
  {
    ++mModelFileIndex;
    mModelFileIndex %= mPbrModelFiles.size();

    mViewModel = 0;

    ParseModelFile( mModelFileIndex );
  }
}

const Asset& SceneFileParser::GetAsset() const
{
  return mAssets[mViewModel];
}

std::string SceneFileParser::GetCurrentModelFile() const
{
  std::string file;

  if( mModelFileIndex < mPbrModelFiles.size() )
  {
    file = mPbrModelFiles[mModelFileIndex];
  }

  return file;
}

} // namespace SceneLauncher
