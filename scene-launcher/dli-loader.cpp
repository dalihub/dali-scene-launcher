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
#include "dli-loader.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali-toolkit/public-api/image-loader/sync-image-loader.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

// INTERNAL INCLUDES
#include "application-resources.h"
#include "asset.h"
#include "model-pbr.h"
#include "ktx-loader.h"
#include "utils.h"

namespace
{

const std::string DLI_EXT( ".dli" );
const size_t DLI_EXT_SIZE = DLI_EXT.size();
const char SLASH = '/';

AlphaFunction GetAlphaFunction( const std::string& alphaFunctionValue )
{
  if( "DEFAULT" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::DEFAULT );
  }
  else if( "LINEAR" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::LINEAR );
  }
  else if( "REVERSE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::REVERSE );
  }
  else if( "EASE_IN_SQUARE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_IN_SQUARE );
  }
  else if( "EASE_OUT_SQUARE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_OUT_SQUARE );
  }
  else if( "EASE_IN" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_IN );
  }
  else if( "EASE_OUT" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_OUT );
  }
  else if( "EASE_IN_OUT" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_IN_OUT );
  }
  else if( "EASE_IN_SINE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_IN_SINE );
  }
  else if( "EASE_OUT_SINE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_OUT_SINE );
  }
  else if( "EASE_IN_OUT_SINE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_IN_OUT_SINE );
  }
  else if( "BOUNCE" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::BOUNCE );
  }
  else if( "SIN" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::SIN );
  }
  else if( "EASE_OUT_BACK" == alphaFunctionValue )
  {
    return AlphaFunction( Dali::AlphaFunction::EASE_OUT_BACK );
  }
  return AlphaFunction( Dali::AlphaFunction::DEFAULT );
}

bool ReadString( const TreeNode* node, std::string& strValue )
{
  if( !node )
  {
    return false;
  }
  bool returnValue = false;
  if( node->GetType() == TreeNode::STRING )
  {
    strValue = node->GetString();
    returnValue = true;
  }
  return returnValue;
}

bool ReadInt( const TreeNode* node, int& num )
{
  if( !node )
  {
    return false;
  }
  bool returnValue = false;
  if( node->GetType() == TreeNode::INTEGER )
  {
    num = node->GetInteger();
    returnValue = true;
  }
  else  if( node->GetType() == TreeNode::FLOAT )
  {
    num = node->GetFloat();
    returnValue = true;
  }

  return returnValue;
}

bool ReadFloat( const TreeNode* node, float& num )
{
  if( !node )
  {
    return false;
  }
  bool returnValue = false;

  if( node->GetType() == TreeNode::FLOAT )
  {
    num = node->GetFloat();
    returnValue = true;
  }
  else if( node->GetType() == TreeNode::INTEGER )
  {
    num = node->GetInteger();
    returnValue = true;
  }

  return returnValue;
}

bool ReadVector( const TreeNode* node, float* num, unsigned int size)
{
  if( !node )
  {
    return false;
  }
  bool returnValue = false;

  if( ( node->Size() >= size ) && ( node->GetType() == TreeNode::ARRAY ) )
  {
    unsigned int offset = 0u;
    for( TreeNode::ConstIterator it = node->CBegin(); offset < size; ++it, ++offset )
    {
      const TreeNode& coord = (*it).second;
      if( !ReadFloat( &coord, *( num + offset ) ) )
      {
        return false;
      }
    }
    returnValue = true;
  }

  return returnValue;
}

bool ReadBool( const TreeNode* node, bool& num )
{
  if( !node )
  {
    return false;
  }
  bool returnValue = false;

  if( node->GetType() == TreeNode::BOOLEAN )
  {
    num = node->GetBoolean();
    returnValue = true;
  }

  return returnValue;
}

bool ReadTimePeriod( const TreeNode* node, TimePeriod& timePeriod )
{
  if( !node )
  {
    return false;
  }

  if(!ReadFloat(node->GetChild("delay"), timePeriod.delaySeconds ) || !ReadFloat(node->GetChild("duration"), timePeriod.durationSeconds ) )
  {
    return false;
  }
  return true;
}

Property::Value GetPropertyValue( const Property::Type& propType, const TreeNode& child )
{
  switch ( propType )
  {
    case Property::BOOLEAN:
    {
      bool value = false;
      ReadBool(&child, value);
      return Property::Value( value );
    }

    case Property::FLOAT:
    {
      float value = 0.0f;
      ReadFloat(&child, value);
      return Property::Value( value );
    }

    case Property::VECTOR2:
    {
      Vector2 value;
      ReadVector(&child, value.AsFloat(), 2u );
      return Property::Value( value );
    }

    case Property::VECTOR3:
    {
      Vector3 value;
      ReadVector(&child, value.AsFloat(), 3u );
      return Property::Value( value );
    }

    case Property::VECTOR4:
    {
      Vector4 value;
      ReadVector(&child, value.AsFloat(), 4u );
      return Property::Value( value );
    }

    case Property::ROTATION:
    {
      if( 4 == child.Size() )
      {
        Vector4 v;
        ReadVector(&child, v.AsFloat(), 4u );
        //Quaternion
        return Property::Value( Quaternion( v ) );
      }
      else
      {
        // degrees as per spec
        Vector3 rotation;
        ReadVector(&child, rotation.AsFloat(), 3u );
        return Property::Value( Quaternion(Radian(Degree(rotation.x)),
                                           Radian(Degree(rotation.y)),
                                           Radian(Degree(rotation.z))) );
      }
    }

    case Property::NONE: // fall
    default:
    {
      DALI_ASSERT_ALWAYS( !"Property type incorrect" );
    }
  }
}

const TreeNode *Tidx(const TreeNode *node, int index)
{
  int i = 0;
  for( TreeNode::ConstIterator it = (*node).CBegin(); it != (*node).CEnd(); ++it, ++i )
  {
    if(i == index)
    {
      return &((*it).second);
    }
  }
  return NULL;
}

bool ReadFile(unsigned char*& efileContent, std::string directory, std::string fileName )
{
  FILE* fp = NULL;
  std::string path(directory);
  path.append(fileName);
  fp = fopen( path.c_str(),"rb");
  long lSize;
  if( NULL == fp )
  {
    return false;
  }
  fseek(fp, 0, SEEK_END);
  lSize = ftell(fp);
  rewind(fp);
  efileContent = (unsigned char *) realloc( efileContent, sizeof(char) * ( lSize + 1 ) );
  ssize_t result = fread( efileContent, 1, lSize, fp );
  fclose(fp);
  return ( result >= 0 );
}

}//namespace

namespace SceneLauncher
{

std::string DliLoader::GetFirstDliInFolder( const char* const modelDirUrl )
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
        return filePath;
      }
    }
  }

  std::stringstream stream;
  stream << "No .dli found in '" << std::string( modelDirUrl ) << "'.";
  throw DaliException( ASSERT_LOCATION, stream.str().c_str());
}

DliLoader::DliLoader()
: mNodes( NULL )
{
}

DliLoader::~DliLoader()
{
}

bool DliLoader::LoadObject( const std::string& modelUrl )
{
  std::ifstream dalifs( modelUrl.c_str() );
  std::string daliBuffer( ( std::istreambuf_iterator<char>( dalifs ) ),
                      ( std::istreambuf_iterator<char>() ) );
  mParser = JsonParser::New();
  bool result_format = mParser.Parse( daliBuffer );
  mDirectory = modelUrl.substr( 0 , modelUrl.rfind( "/" ) + 1 );
  return result_format;
}

bool DliLoader::CreateScene( std::vector<Shader>& shaderArray, Actor toActor, Texture& skyboxTexture )
{
  const TreeNode *root = mParser.GetRoot();
  const TreeNode *inodes = NULL;
  const TreeNode *itn = NULL;

  itn = root->GetChild( "scene" );
  int scene_load = itn->GetInteger();

  if( !( ( itn = root->GetChild( "scenes" ) ) && ( mNodes = root->GetChild( "nodes" ) ) ) )
  {
    return false;
  }

  itn = Tidx( itn, scene_load );
  if( !itn )
  {
    return false;
  }

  LoadTextureSetArray( skyboxTexture );
  LoadShaderArray( shaderArray );
  LoadGeometryArray();

  int starting_node = itn->GetInteger();

  inodes = Tidx(mNodes, starting_node);
  AddNode( toActor, inodes, shaderArray, LightingMode::UNLIT );

  LoadScripts();

  return true;
}

bool DliLoader::LoadAnimation( Actor toActor, std::vector<Animation>& animArray, const std::string& animationName )
{
  const TreeNode* root = mParser.GetRoot();
  const TreeNode* animations = root->GetChild( "animations" );

  if( nullptr == animations )
  {
    return false;
  }
  animations = animations->GetChild( animationName.c_str() );

  if( nullptr == animations )
  {
    return false;
  }

  for( TreeNode::ConstIterator animIt = ( *animations ).CBegin(); animIt != ( *animations ).CEnd(); ++animIt )
  {
    Animation animation( Animation::New( 0.0f ) );
    float duration = 0.0f;
    float durationSum = 0.0f;
    const TreeNode &animNode = (*animIt).second;
    if( ReadFloat( animNode.GetChild( "duration" ), duration ) )
    {
      animation.SetDuration( duration );
    }

    bool looping;
    if( ReadBool( animNode.GetChild( "loop" ), looping ) )
    {
      animation.SetLooping( looping );
    }

    std::string endAction;
    if( ReadString( animNode.GetChild( "endAction" ), endAction ) )
    {
      if( "BAKE" == endAction )
      {
        animation.SetEndAction( Animation::Bake );
      }
      else if( "DISCARD" == endAction )
      {
        animation.SetEndAction( Animation::Discard );
      }
      else if( "BAKE_FINAL" == endAction )
      {
        animation.SetEndAction( Animation::BakeFinal );
      }
    }
    std::string disconnectAction;
    if( ReadString( animNode.GetChild( "disconnectAction" ), disconnectAction ) )
    {
      if( "BAKE" == endAction )
      {
        animation.SetDisconnectAction( Animation::Bake );
      }
      else if( "DISCARD" == endAction )
      {
        animation.SetDisconnectAction( Animation::Discard );
      }
      else if( "BAKE_FINAL" == endAction )
      {
        animation.SetDisconnectAction( Animation::BakeFinal );
      }
    }
    const TreeNode *propertiesNode = animNode.GetChild( "properties" );

    if(propertiesNode)
    {
      for( TreeNode::ConstIterator iter = ( *propertiesNode ).CBegin(); iter != ( *propertiesNode ).CEnd(); ++iter )
      {
        const TreeNode::KeyNodePair& pKeyChild = *iter;
        Actor animActor;
        Property::Value propValue;
        Property::Index propIndex = Property::INVALID_INDEX;
        std::string actorName;
        std::string actorProperty;
        if( !ReadString( pKeyChild.second.GetChild( "actor" ), actorName ) )
        {
          continue;
        }
        if( ReadString( pKeyChild.second.GetChild( "property" ), actorProperty ) )
        {
          animActor = toActor.FindChildByName( actorName );
          if( !animActor )
          {
            continue;
          }
          propIndex = animActor.GetPropertyIndex( actorProperty );
          if( propIndex == Property::INVALID_INDEX )
          {
              throw DaliException( ASSERT_LOCATION, "Cannot find property on object\n");
              continue;
          }
        }

        // these are the defaults
        AlphaFunction alphaFunction( AlphaFunction::DEFAULT );
        TimePeriod timePeriod( 0.0f );
        bool timeDefined = false;

        timeDefined = ReadTimePeriod( pKeyChild.second.GetChild( "timePeriod" ), timePeriod );

        durationSum = std::max( durationSum, timePeriod.delaySeconds + timePeriod.durationSeconds );

        std::string alphaFunctionValue;
        if( ReadString( pKeyChild.second.GetChild( "alphaFunction" ), alphaFunctionValue ) )
        {
          alphaFunction = GetAlphaFunction(alphaFunctionValue);
        }
        if( const TreeNode* keyFrameChild = pKeyChild.second.GetChild( "keyFrames" ) )
        {
          DALI_ASSERT_ALWAYS( !actorProperty.empty()  && "Animation must specify a property name" );
          Property prop = Property( animActor, propIndex );
          KeyFrames keyframes = KeyFrames::New();

          const TreeNode::ConstIterator endIter = keyFrameChild->CEnd();
          for( TreeNode::ConstIterator iter = keyFrameChild->CBegin(); endIter != iter; ++iter )
          {
            const TreeNode::KeyNodePair& kfKeyChild = *iter;
            bool readResult;
            float progress = 0.0f;
            readResult = ReadFloat( kfKeyChild.second.GetChild( "progress" ), progress );
            DALI_ASSERT_ALWAYS( readResult && "Key frame entry must have 'progress'" );

            const TreeNode *kfValue = kfKeyChild.second.GetChild( "value" );
            DALI_ASSERT_ALWAYS( kfValue && "Key frame entry must have 'value'" );

            try
            {
              propValue = GetPropertyValue( prop.object.GetPropertyType(prop.propertyIndex), *kfValue );
            }
            catch(...)
            {
              DALI_LOG_WARNING( "Property:'%s' type does not match value type '%s'\n",
                                (*property).c_str(),
                                PropertyTypes::GetName(prop.object.GetPropertyType(prop.propertyIndex)) );

              throw;
            }

            AlphaFunction kfAlphaFunction( AlphaFunction::DEFAULT );
            std::string alphaFuncStr;

            if( ReadString( kfKeyChild.second.GetChild( "alphaFunction" ), alphaFuncStr ) )
            {
              kfAlphaFunction = GetAlphaFunction( alphaFuncStr );
            }

            keyframes.Add( progress, propValue, kfAlphaFunction );
          }

          if( timeDefined )
          {
            animation.AnimateBetween( prop, keyframes, alphaFunction, timePeriod );
          }
          else
          {
            animation.AnimateBetween( prop, keyframes, alphaFunction );
          }

        }
        else
        {
          Property prop = Property( animActor, propIndex );
          try
          {
            const TreeNode *valueNode = pKeyChild.second.GetChild( "value" );
            if( valueNode )
            {
              propValue = GetPropertyValue( prop.object.GetPropertyType(prop.propertyIndex), *valueNode );
            }

          }
          catch(...)
          {
            DALI_LOG_WARNING( "Property:'%s' type does not match value type '%s'\n", actorProperty.c_str(),
                              PropertyTypes::GetName( prop.object.GetPropertyType(prop.propertyIndex) ) );

            throw;
          }

          bool relative = false;
          ReadBool( pKeyChild.second.GetChild( "relative" ), relative );
          if( relative )
          {
            if( timeDefined )
            {
              animation.AnimateBy( prop, propValue, alphaFunction, timePeriod );
            }
            else
            {
              animation.AnimateBy( prop, propValue, alphaFunction );
            }
          }
          else
          {
            if( timeDefined )
            {
              animation.AnimateTo( prop, propValue, alphaFunction, timePeriod );
            }
            else
            {
              animation.AnimateTo( prop, propValue, alphaFunction );
            }
          }

        }
      }

    }

    animArray.push_back( animation );
  }

  return true;
}

void DliLoader::GetCameraParameters( unsigned int eidx, CameraParameters& camera )
{
  const TreeNode* cameras = mParser.GetRoot()->GetChild( "cameras" );

  if( nullptr == cameras )
  {
    return;
  }

  const TreeNode* node = Tidx(cameras, eidx);
  const TreeNode* parameter;
  if(node)
  {
    ReadFloat(node->GetChild( "fov" ), camera.cameraFov );
    ReadFloat(node->GetChild( "near" ), camera.cameraNear );
    ReadFloat(node->GetChild( "far" ), camera.cameraFar );
    if(ReadVector( node->GetChild( "orthographic" ), camera.cameraOrthographicSize.AsFloat(), 4u ))
    {
      camera.enablePerspective = false;
    }

    if((parameter = node->GetChild( "matrix" )))
    {
      ReadVector( node->GetChild( "matrix" ), camera.cameraMatrix.AsFloat(), 16u );
    }
  }
}

bool DliLoader::GetLightParameters(unsigned int eidx, LightParameters& light)
{
  const TreeNode* root = mParser.GetRoot();
  if (!root)
  {
    return false;
  }

  const TreeNode* lightRoot = root->GetChild("lights");
  if (!lightRoot)
  {
    return false;
  }

  const TreeNode* lightNode = Tidx(lightRoot, eidx);
  if (!lightNode)
  {
    return false;
  }

  if (!ReadVector(lightNode->GetChild("matrix"), light.transform.AsFloat(), 16))
  {
    DALI_LOG_WARNING("Failed to parse light %d - \"matrix\" child with 16 floats expected.\n", eidx);
    return false;
  }

  return true;
}

std::string DliLoader::GetParseError() const
{
  std::stringstream stream;

  if( mParser.ParseError() )
  {
    stream << "position: " << mParser.GetErrorPosition() << ", line: " << mParser.GetErrorLineNumber() << ", column: " << mParser.GetErrorColumn() << ", description: " << mParser.GetErrorDescription() << ".";
  }

  return stream.str();
}

const std::vector<Script>& DliLoader::GetScripts() const
{
  return mScripts;
}

bool DliLoader::LoadTextureSetArray( Texture& skyboxTexture )
{
  const TreeNode* materials = mParser.GetRoot()->GetChild( "materials" );
  const TreeNode* environment = mParser.GetRoot()->GetChild( "environment" );
  if( ( NULL == materials ) || ( NULL == environment ) )
  {
    // Nothing to do if there aren't materials or environment.
    return false;
  }

  const TreeNode* skybox = mParser.GetRoot()->GetChild( "skybox" );
  if( NULL != skybox )
  {
    std::string skyboxTexturePath;
    if( ReadString( skybox->GetChild( "texture" ), skyboxTexturePath ) )
    {
      CreateSkyboxTexture( skyboxTexturePath, skyboxTexture );
    }
  }

  const unsigned int numberOfEnvironments = environment->Size();

  std::vector<Texture> cubeDiffuseTextures;
  std::vector<Texture> cubeSpecularTextures;

  cubeDiffuseTextures.reserve( numberOfEnvironments );
  cubeSpecularTextures.reserve( numberOfEnvironments );

  for( TreeNode::ConstIterator it = environment->CBegin(); it != environment->CEnd(); ++it )
  {
    const TreeNode *node = &((*it).second);

    std::string cubeDiffuse;
    std::string cubeSpecular;

    ReadString( node->GetChild( "cubeDiffuse" ), cubeDiffuse );
    ReadString( node->GetChild( "cubeSpecular" ), cubeSpecular );

    Texture diffuseTexture;
    Texture specularTexture;

    CreateEnvironmentTextures( cubeDiffuse, cubeSpecular, diffuseTexture, specularTexture );

    cubeDiffuseTextures.push_back( diffuseTexture );
    cubeSpecularTextures.push_back( specularTexture );
  }

  for( TreeNode::ConstIterator it = (*materials).CBegin(); it != (*materials).CEnd(); ++it)
  {
    const TreeNode *node = &((*it).second);

    int environmentIndex = 0;
    ReadInt( node->GetChild( "environment" ), environmentIndex );

    Texture diffuseTexture = cubeDiffuseTextures[environmentIndex];
    Texture specularTexture = cubeSpecularTextures[environmentIndex];

    std::string strTexture[4];
    Texture texture[4];
    if( ReadString( node->GetChild( "texture1" ), strTexture[0] ) )
    {
      if( ReadString( node->GetChild( "texture2" ), strTexture[1] ) )
      {
        if( ReadString( node->GetChild( "texture3" ), strTexture[2] ) )
        {
          ReadString( node->GetChild( "texture4" ), strTexture[3] );
        }
      }
    }
    bool createMipmap = false;
    ReadBool( node->GetChild( "mipmap" ), createMipmap );

    CreateTextures( strTexture, texture, createMipmap );

    TextureSet textureSet = TextureSet::New();

    Sampler samplerA = Sampler::New();
    samplerA.SetWrapMode( WrapMode::REPEAT, WrapMode::REPEAT );
    Sampler sampler = Sampler::New();
    sampler.SetWrapMode( WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE, WrapMode::CLAMP_TO_EDGE );
    if(createMipmap)
    {
      samplerA.SetFilterMode( FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR );
      sampler.SetFilterMode( FilterMode::LINEAR_MIPMAP_LINEAR, FilterMode::LINEAR );
    }

    unsigned int i = 0u;
    for( ; i < 4u ; ++i )
    {
      if( texture[i] )
      {
        textureSet.SetTexture( i , texture[i] );
        textureSet.SetSampler( i, samplerA );
      }
      else
      {
        break;
      }
    }

    if( diffuseTexture )
    {
      textureSet.SetTexture( i++ , diffuseTexture );
    }

    if( specularTexture )
    {
      textureSet.SetTexture( i , specularTexture );
      textureSet.SetSampler( i , sampler );
    }

    mTextureSetArray.push_back( textureSet );
  }

  return true;
}

bool DliLoader::LoadShaderArray( std::vector<Shader>& shaderArray )
{
  const TreeNode *shaders = mParser.GetRoot()->GetChild( "shaders" );
  if(!shaders)
  {
    return false;
  }

  for( TreeNode::ConstIterator it = (*shaders).CBegin(); it != (*shaders).CEnd(); ++it)
  {
    const TreeNode *node = &((*it).second);

    std::string vertex;
    std::string fragment;
    ReadString( node->GetChild( "vertex" ), vertex );
    ReadString( node->GetChild( "fragment" ), fragment );
    Shader shader = CreateShader( ApplicationResources::Get().GetShadersPath() + vertex, ApplicationResources::Get().GetShadersPath() + fragment );

    if( !shader )
    {
      throw DaliException( ASSERT_LOCATION, "Failed to load shader." );
      return false;
    }
    else
    {
      shaderArray.push_back( shader );
    }

    for( TreeNode::ConstIterator uniformNode = node->CBegin(); uniformNode != node->CEnd(); ++uniformNode )
    {
      const TreeNode::KeyNodePair& pKeyChild = *uniformNode;
      std::string strItem( pKeyChild.first );

      if( !strItem.compare( "vertex" ) || !strItem.compare( "fragment" ) || !strItem.compare( "renderMode" ))
      {
        continue;
      }

      float fNum[16];
      if( ( pKeyChild.second.GetType() ==  TreeNode::INTEGER ) || ( pKeyChild.second.GetType() ==  TreeNode::FLOAT ) )
      {
        if(ReadFloat( &pKeyChild.second, fNum[0]))
        {
          shader.RegisterProperty( pKeyChild.first, fNum[0] );
        }
      }
      else if( ReadVector( &pKeyChild.second, fNum, 16 ) )
      {
        Matrix matrix( fNum );
        shader.RegisterProperty( pKeyChild.first, matrix );
      }
      else if( ReadVector( &pKeyChild.second, fNum, 9 ) )
      {
        Matrix3 matrix( fNum[0], fNum[1], fNum[2], fNum[3], fNum[4], fNum[5], fNum[6], fNum[7], fNum[8] );
        shader.RegisterProperty( pKeyChild.first, matrix );
      }
      else if( ReadVector( &pKeyChild.second, fNum, 4 ) )
      {
        Vector4 vector( fNum );
        shader.RegisterProperty( pKeyChild.first, vector );
      }
      else if( ReadVector( &pKeyChild.second, fNum, 3 ) )
      {
        Vector3 vector( fNum );
        shader.RegisterProperty( pKeyChild.first, vector );
      }
      else if( ReadVector( &pKeyChild.second, fNum, 2 ) )
      {
        Vector2 vector( fNum );
        shader.RegisterProperty( pKeyChild.first, vector );
      }

    }


    RendererOptions renderer;
    ReadInt( node->GetChild( "renderMode" ), renderer.blend );

    mRendererOptionsArray.push_back( renderer );
  }

  return true;
}

bool DliLoader::LoadGeometryArray()
{
  unsigned char *fileContent = NULL;
  std::string binFilename;
  const TreeNode *root = mParser.GetRoot();
  const TreeNode *meshRoot = root->GetChild( "meshes" );
  for( TreeNode::ConstIterator mesh = ( *meshRoot ).CBegin(); mesh != ( *meshRoot ).CEnd(); ++mesh )
  {
    Geometry geometry = Geometry::New();
    LoadBuffer(&(*mesh).second, geometry, binFilename, fileContent);
    mGeometryArray.push_back(geometry);
  }

  if(fileContent)
  {
    free(fileContent);
  }
  return true;
}

void DliLoader::AddNode( Actor toActor, const TreeNode *addnode, const std::vector<Shader>& shaderArray, LightingMode::Type lightingMode )
{
  const TreeNode *itn = NULL;
  Actor actor;
  Vector3 actorSize( Vector3::ONE );
  float floatVec[3];
  if( ReadVector( addnode->GetChild( "bounds" ), floatVec, 3 ) || ReadVector( addnode->GetChild( "size" ), floatVec, 3 ) )
  {
    actorSize = Vector3( floatVec[0], floatVec[1], floatVec[2] );
  }

  std::string nodeName;
  ReadString( addnode->GetChild( "name" ), nodeName );

  if( !( itn = addnode->GetChild( "mesh" ) ) )
  {
    actor = Actor::New();
    actor.SetAnchorPoint( AnchorPoint::CENTER );
    actor.SetParentOrigin( ParentOrigin::CENTER );
    actor.SetPosition( Vector3() );
    actor.SetSize( actorSize );
    actor.SetName(nodeName);
  }
  else
  {
    int shaderIdx = 0;
    int materialIndex = 0;
    int meshIndex = itn->GetInteger();
    ReadInt( addnode->GetChild( "shader" ), shaderIdx );
    ReadInt( addnode->GetChild( "material" ), materialIndex );

    actor = ModelPbr::CreateNode( shaderArray[shaderIdx],
                                  mRendererOptionsArray[shaderIdx].blend,
                                  mTextureSetArray[materialIndex],
                                  mGeometryArray[meshIndex],
                                  actorSize,
                                  nodeName );
  }

  // Attempt to read shadow mode. It's optional.
  if ( auto node = addnode->GetChild(LightingMode::PROPERTY_NAME) )  // overriding setting from parent?
  {
    if( node->GetType() == TreeNode::STRING )
    {
      if (!LightingMode::InterpretValue(node->GetString(), lightingMode))
      {
        DALI_LOG_WARNING("Failed to interpret %s value '%s', keeping '%s'.\n", LightingMode::PROPERTY_NAME,
          value.c_str(), GetValue(lightingMode));
      }
    }
    else if( node->GetType() == TreeNode::INTEGER )
    {
      int value = node->GetInteger();
      if (LightingMode::GetValue(static_cast<LightingMode::Type>(value)))
      {
        lightingMode = static_cast<LightingMode::Type>(value);
      }
      else
      {
        DALI_LOG_WARNING("Ignoring invalid %s value (%d) in '%s'.\n", LightingMode::PROPERTY_NAME,
          value, nodeName.c_str());
      }
    }
    else  //... but if present, it has to be valid type.
    {
      DALI_LOG_WARNING("Ignoring invalid %s type in '%s' (string or integer expected).\n",
          LIGHTING_MODE_PROPERTY.c_str(), nodeName.c_str());
    }
  }
  actor.RegisterProperty(LightingMode::PROPERTY_NAME, static_cast<int>(lightingMode));

  // Read transform.
  ReadAnglePosition(addnode, actor );

  if( actor )
  {
    toActor.Add( actor );

    if((itn = addnode->GetChild("children")))
    {
      Vector<int> children;
      children.Reserve(itn->Size());
      for( TreeNode::ConstIterator it = ( *itn ).CBegin(); it != ( *itn ).CEnd(); ++it )
      {
        children.PushBack( ( ( *it ).second).GetInteger() );
      }

      for( unsigned int i = 0; i < children.Size(); ++i )
      {
        const TreeNode *inodes = Tidx( mNodes, children[i] );
        AddNode( actor, inodes, shaderArray, lightingMode );
      }
    }
  }
}

void DliLoader::LoadScripts()
{
  const TreeNode* root = mParser.GetRoot();

  if( nullptr == root )
  {
    // nothing to do
    return;
  }

  const TreeNode* scriptsRoot = root->GetChild( "scripts" );

  if( nullptr == scriptsRoot )
  {
    // nothing to do
    return;
  }

  // Resizes the Event vector.
  mScripts.resize( scriptsRoot->Size() );
  unsigned int scriptIndex = 0u;

  for( TreeNode::ConstIterator scriptIt = scriptsRoot->CBegin(), scriptEndIt = scriptsRoot->CEnd(); scriptIt != scriptEndIt; ++scriptIt, ++scriptIndex )
  {
    const TreeNode& scriptNode = (*scriptIt).second;

    Script& script = *( mScripts.begin() + scriptIndex );

    ReadString( scriptNode.GetChild( "url" ), script.url );
  }
}

void DliLoader::CreateSkyboxTexture( const std::string& skyBoxTexturePath, Texture& skyboxTexture )
{
  if( !skyBoxTexturePath.empty() )
  {
    CubeData skybox;
    const bool result = LoadCubeMapFromKtxFile( ApplicationResources::Get().GetImagesPath() + skyBoxTexturePath, skybox );

    if( !result )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load skybox textures." );
    }

    skyboxTexture = Texture::New( TextureType::TEXTURE_CUBE, skybox.img[0][0].GetPixelFormat(), skybox.img[0][0].GetWidth(), skybox.img[0][0].GetHeight() );
    for( unsigned int midmapLevel = 0u; midmapLevel < skybox.img[0].size(); ++midmapLevel )
    {
      for( unsigned int i = 0u; i < skybox.img.size(); ++i )
      {
        skyboxTexture.Upload( skybox.img[i][midmapLevel], CubeMapLayer::POSITIVE_X + i, midmapLevel, 0, 0, skybox.img[i][midmapLevel].GetWidth(), skybox.img[i][midmapLevel].GetHeight() );
      }
    }
  }
}

void DliLoader::CreateEnvironmentTextures( const std::string& cubeDiffuse, const std::string& cubeSpecular, Texture& eDiffuseTexture, Texture& eSpecularTexture )
{
  // This texture should have 6 faces and only one mipmap
  CubeData diffuse;
  bool result = false;
  if( !cubeDiffuse.empty() )
  {
    result = LoadCubeMapFromKtxFile( ApplicationResources::Get().GetImagesPath() + cubeDiffuse, diffuse );

    if( !result )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load cubemap textures." );
    }

    eDiffuseTexture = Texture::New( TextureType::TEXTURE_CUBE, diffuse.img[0][0].GetPixelFormat(), diffuse.img[0][0].GetWidth(), diffuse.img[0][0].GetHeight() );
    for( unsigned int midmapLevel = 0; midmapLevel < diffuse.img[0].size(); ++midmapLevel )
    {
      for( unsigned int i = 0; i < diffuse.img.size(); ++i )
      {
        eDiffuseTexture.Upload( diffuse.img[i][midmapLevel], CubeMapLayer::POSITIVE_X + i, midmapLevel, 0, 0, diffuse.img[i][midmapLevel].GetWidth(), diffuse.img[i][midmapLevel].GetHeight() );
      }
    }
  }

  // This texture should have 6 faces and 6 mipmaps
  if( !cubeSpecular.empty() )
  {
    CubeData specular;
    result = LoadCubeMapFromKtxFile( ApplicationResources::Get().GetImagesPath() + cubeSpecular, specular);

    if( !result )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load cubemap textures." );
    }

    eSpecularTexture = Texture::New( TextureType::TEXTURE_CUBE, specular.img[0][0].GetPixelFormat(), specular.img[0][0].GetWidth(), specular.img[0][0].GetHeight() );
    for( unsigned int midmapLevel = 0; midmapLevel < specular.img[0].size(); ++midmapLevel )
    {
      for( unsigned int i = 0; i < specular.img.size(); ++i )
      {
        eSpecularTexture.Upload( specular.img[i][midmapLevel], CubeMapLayer::POSITIVE_X + i, midmapLevel, 0, 0, specular.img[i][midmapLevel].GetWidth(), specular.img[i][midmapLevel].GetHeight() );
      }
    }
  }
}

void DliLoader::CreateTextures(std::string strTexture[4], Texture eTexture[4], bool createMipmaps )
{
  PixelData pixelData;
  for(int i = 0; i < 4; i++)
  {
    if( strTexture[i].empty() )
      return;
    pixelData = SyncImageLoader::Load( ApplicationResources::Get().GetImagesPath() + strTexture[i] );

    if( !pixelData )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load texture." );
    }

    eTexture[i] = Texture::New( TextureType::TEXTURE_2D, pixelData.GetPixelFormat(), pixelData.GetWidth(), pixelData.GetHeight() );
    eTexture[i].Upload( pixelData, 0, 0, 0, 0, pixelData.GetWidth(), pixelData.GetHeight() );
    if( createMipmaps )
    {
      eTexture[i].GenerateMipmaps();
    }
  }

}

bool DliLoader::LoadBuffer( const TreeNode* mesh, Geometry geometry, std::string& ebinFilename, unsigned char*& efileContent)
{
  int byteOffset = 0;
  int byteLength = 0;
  const TreeNode *temp = mesh->GetChild( "uri" );
  if(!temp)
    return false;

  std::string strQuad("quad");

  if( !strQuad.compare( temp->GetString() ) )
  {
    struct Vertex
    {
      Vector3 aPosition;
      Vector2 aTexCoord;
    };
    Vertex vertices[] = {
          { Vector3( -0.5f,-0.5f, 0.0f ), Vector2( 0.0f, 1.0f )},
          { Vector3(  0.5f,-0.5f, 0.0f ), Vector2( 1.0f, 1.0f )},
          { Vector3( -0.5f, 0.5f, 0.0f ), Vector2( 0.0f, 0.0f )},
          { Vector3(  0.5f, 0.5f, 0.0f ), Vector2( 1.0f, 0.0f )}
        };

    Property::Map property;
    property.Insert( "aPosition", Property::VECTOR3 );
    property.Insert( "aTexCoord", Property::VECTOR2 );

    PropertyBuffer vertexBuffer = PropertyBuffer::New( property );

    vertexBuffer.SetData( vertices, sizeof(vertices) / sizeof(Vertex) );

    geometry.AddVertexBuffer( vertexBuffer );

    geometry.SetType( Geometry::TRIANGLE_STRIP );
    return true;
  }

  if( ebinFilename.compare( temp->GetString() ) )
  {
    ebinFilename = temp->GetString();

    if( !ReadFile( efileContent, mDirectory, ebinFilename) )
    {
      return false;
    }
  }
  temp = mesh->GetChild( "attributes" );
  if(temp->GetInteger() != 63)
  {
    // It is missing some of the attributes,
    // bit0, indices
    // bit1, positions
    // bit2, normals
    // bit3, textures
    // bit4, tangents
    // bit5, bitangents
    return false;
  }

  std::string primitive;
  ReadString( mesh->GetChild( "primitive" ), primitive );

  if ( primitive == "TRIANGLES" )
  {
    geometry.SetType( Geometry::TRIANGLES );
  }
  else if ( primitive == "LINES" )
  {
    geometry.SetType( Geometry::LINES );
  }
  else if ( primitive == "POINTS" )
  {
    geometry.SetType( Geometry::POINTS );
  }
  else
  {
    return false;
  }
  temp = mesh->GetChild( "indices" );
  if( temp && ( !ReadInt( temp->GetChild( "byteOffset" ), byteOffset ) || !ReadInt( temp->GetChild( "byteLength" ), byteLength ) ) )
  {
    return false;
  }

  geometry.SetIndexBuffer ((unsigned short*) &efileContent[byteOffset], byteLength / sizeof( unsigned short ) );

  temp = mesh->GetChild( "positions" );
  if( temp && ( !ReadInt( temp->GetChild( "byteOffset" ), byteOffset ) || !ReadInt( temp->GetChild( "byteLength" ), byteLength ) ) )
  {
    return false;
  }
  Property::Map positionMap;
  positionMap["aPosition"] = Property::VECTOR3;
  PropertyBuffer positionBuffer = PropertyBuffer::New( positionMap );
  positionBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector3 ) ) );

  temp = mesh->GetChild( "normals" );
  if( temp && ( !ReadInt( temp->GetChild( "byteOffset" ), byteOffset ) || !ReadInt( temp->GetChild( "byteLength" ), byteLength ) ) )
  {
    return false;
  }
  Property::Map normalMap;
  normalMap["aNormal"] = Property::VECTOR3;
  PropertyBuffer normalBuffer = PropertyBuffer::New( normalMap );
  normalBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector3 ) ) );


  temp = mesh->GetChild( "textures" );
  if( temp && ( !ReadInt( temp->GetChild( "byteOffset" ), byteOffset ) || !ReadInt( temp->GetChild( "byteLength" ), byteLength ) ) )
  {
    return false;
  }
  Property::Map textCoordMap;
  textCoordMap["aTexCoord"] = Property::VECTOR2;
  PropertyBuffer texCoordBuffer = PropertyBuffer::New( textCoordMap );
  texCoordBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector2 ) ) );

  temp = mesh->GetChild( "tangents" );
  if( temp && ( !ReadInt( temp->GetChild( "byteOffset" ), byteOffset ) || !ReadInt( temp->GetChild( "byteLength" ), byteLength ) ) )
  {
    return false;
  }
  Property::Map tangentMap;
  tangentMap["aTangent"] = Property::VECTOR3;
  PropertyBuffer tangentBuffer = PropertyBuffer::New( tangentMap );
  tangentBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector3 ) ) );

  geometry.AddVertexBuffer( positionBuffer );
  geometry.AddVertexBuffer( normalBuffer );
  geometry.AddVertexBuffer( texCoordBuffer );
  geometry.AddVertexBuffer( tangentBuffer );

  return true;

}

void DliLoader::ReadAnglePosition(const TreeNode* node, Actor &actor)
{
  float num[16];

  if( ReadVector( node->GetChild( "matrix" ), num, 16 ) )
  {
    Dali::Quaternion quaternion( Vector3( num ), Vector3( num + 4 ), Vector3( num + 8 ) );
    actor.SetOrientation( quaternion );
    actor.SetPosition( Vector3( num + 12 ) );
  }
  else
  {
    if( ReadVector( node->GetChild( "angle" ), num, 3 ) )
    {
      Dali::Quaternion quaternion( Radian( Degree( (float) num[0] ) ), Radian( Degree( (float) num[1] ) ), Radian( Degree( (float)num[2] ) ) );
      actor.SetOrientation( quaternion );
    }

    if( ReadVector( node->GetChild( "position" ), num, 3 ) )
    {
      actor.SetPosition( Vector3( num ) );
    }
  }

}

} // namespace SceneLauncher
