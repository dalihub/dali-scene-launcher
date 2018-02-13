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
#include "scene-file-parser.h"

// EXTERNAL INCLUDES
#include <dali-toolkit/devel-api/builder/json-parser.h>

#include <sstream>
#include <dirent.h>
#include <sys/stat.h>

// INTERNAL INCLUDES
#include "dli-loader.h"

using namespace Dali::Toolkit;

namespace
{
const std::string NULL_STRING;
const SceneLauncher::Asset VOID_ASSET;

const std::string DLI_EXT = ".dli";
const size_t DLI_EXT_SIZE = DLI_EXT.size();
const std::string SLASH( "/" );

/**
 * @brief Insensitive case compare function.
 *
 * @param[in] a, compare string
 * @param[in] b, compare string
 * @return true if strings are equal
 */
bool CaseInsensitiveCharacterCompare( unsigned char a, unsigned char b )
{
  // Converts to lower case in the current locale.
  return std::tolower( a ) == std::tolower( b );
}

/**
 * @brief return true if the lower cased ASCII strings are equal.
 *
 * @param[in] a, compare string
 * @param[in] b, compare string
 * @return true if strings are equal
 */
bool CaseInsensitiveStringCompare( const std::string& a, const std::string& b )
{
  bool result = false;
  if( a.length() == b.length() )
  {
    result = std::equal( a.begin(), a.end(), b.begin(), &CaseInsensitiveCharacterCompare );
  }
  return result;
}

/**
 * @brief get a vector3 from parsed node
 *
 * @param[in] node, parsed node
 * @param[out] vector3, vector read.
 */
void ParseVector3( const TreeNode& node, Vector3& vector3 )
{
  unsigned int offset = 0u;
  for( TreeNode::ConstIterator it = node.CBegin(), endIt = node.CEnd(); it != endIt; ++it, ++offset )
  {
    const TreeNode& coord = (*it).second;
    vector3.AsFloat()[offset] = coord.GetFloat();
  }
}

}  // namespace

namespace SceneLauncher
{

SceneFileParser::SceneFileParser()
: mViewModel( 0u )
{
}

SceneFileParser::~SceneFileParser()
{
}

void SceneFileParser::ReadModelFolder( const char* const modelDirUrl )
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

  struct dirent* dirp;
  while( ( dirp = readdir( dp ) ) )
  {
    std::string filePath = std::string( modelDirUrl ) + SLASH + dirp->d_name;

    struct stat fileStat;
    if( stat( filePath.c_str(), &fileStat ) || S_ISDIR( fileStat.st_mode ))
    {
      // Do nothing if it fails to retrieve the file info or if the file is a folder.
      continue;
    }

    if( filePath.size() > DLI_EXT_SIZE )
    {
      const std::string fileExtension = filePath.substr( filePath.size() - DLI_EXT_SIZE );
      if( CaseInsensitiveStringCompare( fileExtension, DLI_EXT ) )
      {
        Asset asset;
        asset.model = filePath;
        mAssets.push_back( asset );
      }
    }
  }
}

void SceneFileParser::LoadNextModel()
{
  ++mViewModel;

  if( !mAssets.empty() )
  {
    mViewModel %= mAssets.size();
  }
}

const Asset& SceneFileParser::GetAsset() const
{
  if( !mAssets.empty() )
  {
    return mAssets[mViewModel];
  }

  return VOID_ASSET;
}

const std::string& SceneFileParser::GetCurrentModelFile() const
{
  if( !mAssets.empty() )
  {
    return mAssets[mViewModel].model;
  }

  return NULL_STRING;
}

void SceneFileParser::SetCameraParameters( const DliCameraParameters &camera )
{
  mAssets[mViewModel].cameraFar = camera.cameraFar;
  mAssets[mViewModel].cameraNear = camera.cameraNear;
  mAssets[mViewModel].cameraMatrix = camera.cameraMatrix;
  mAssets[mViewModel].cameraFov = camera.cameraFov;
  mAssets[mViewModel].enablePerspective = camera.enablePerspective;
  mAssets[mViewModel].cameraOrthographicSize = camera.cameraOrthographicSize;
}

} // namespace SceneLauncher
