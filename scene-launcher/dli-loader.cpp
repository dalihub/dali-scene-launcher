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

// CLASS HEADER
#include "dli-loader.h"
#include "model-pbr.h"
#include "ktx-loader.h"

// EXTERNAL INCLUDES
#include <fstream>
#include <sstream>
#include <dali/integration-api/debug.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali-toolkit/devel-api/builder/json-parser.h>
#include <dali-toolkit/devel-api/builder/tree-node.h>
#include <dali-toolkit/dali-toolkit.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
const std::string ASSET_SHADER_DIR = SCENE_LAUNCHER_SHADER_DIR;
const std::string ASSET_TEXTURE_DIR = SCENE_LAUNCHER_IMAGE_DIR;

AlphaFunction GetAlphaFunction(std::string alphaFunctionValue)
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
  if(!node)
    return false;
  bool returnValue = false;
  if( node->GetType() == TreeNode::STRING )
  {
    strValue = node->GetString();
    returnValue = true;
  }
  return returnValue;
}

bool ReadFloat( const TreeNode* node, float& num )
{
  if(!node)
    return false;
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
  if(!node)
    return false;
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
  if(!node)
    return false;
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
  if(!node)
    return false;
  if(!ReadFloat(node->GetChild("delay"), timePeriod.delaySeconds ) || !ReadFloat(node->GetChild("duration"), timePeriod.durationSeconds ) )
  {
    return false;
  }
  return true;
}

void ParseVector3( const TreeNode& node, Vector3& vector3 )
{
  unsigned int offset = 0u;
  for( TreeNode::ConstIterator it = node.CBegin(), endIt = node.CEnd(); it != endIt; ++it, ++offset )
  {
    const TreeNode& coord = (*it).second;
    vector3.AsFloat()[offset] = coord.GetFloat();
  }
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
      Vector2 value( 0.0f, 0.0f );
      ReadVector(&child, value.AsFloat(), 2 );
      return Property::Value( value );
    }

    case Property::VECTOR3:
    {
      Vector3 value( 0.0f, 0.0f, 0.0f );
      ReadVector(&child, value.AsFloat(), 3 );
      return Property::Value( value );
    }

    case Property::VECTOR4:
    {
      Vector4 value( 0.0f, 0.0f, 0.0f, 0.0f );
      ReadVector(&child, value.AsFloat(), 4 );
      return Property::Value( value );
    }

    case Property::ROTATION:
    {
      if( 4 == child.Size() )
      {
        Vector4 v( 0.0f, 0.0f, 0.0f, 0.0f );
        ReadVector(&child, v.AsFloat(), 4 );
        // angle, axis as per spec
        return Property::Value( Quaternion(Radian(Degree(v[3])),
                                           Vector3(v[0],v[1],v[2])) );
      }
      else
      {
        // degrees as per spec
        Vector3 rotation( 0.0f, 0.0f, 0.0f );
        ReadVector(&child, rotation.AsFloat(), 3 );
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

bool readFile(unsigned char*& efileContent, std::string directory, std::string fileName )
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

/**
* @brief Load a shader source file
* @param[in] The path of the source file
* @param[out] The contents of file
* @return True if the source was read successfully
*/
bool LoadShaderCode( const std::string& fullpath, std::vector<char>& output )
{
  FILE* f = fopen( fullpath.c_str(), "rb" );

  if( NULL == f )
  {
    return false;
  }

  fseek( f, 0, SEEK_END );
  size_t size = ftell( f );
  fseek( f, 0, SEEK_SET );
  output.resize( size + 1 );
  std::fill( output.begin(), output.end(), 0 );
  ssize_t result = fread( output.data(), size, 1, f );
  fclose( f );
  return ( result >= 0 );
}

/**
* @brief Load vertex and fragment shader source
* @param[in] shaderVertexFileName is the filepath of Vertex shader
* @param[in] shaderFragFileName is the filepath of Fragment shader
* @return the Dali::Shader object
*/
Shader LoadShaders( const std::string& shaderVertexFileName, const std::string& shaderFragFileName )
{
  Shader shader;
  std::vector<char> bufV, bufF;

  if( LoadShaderCode( shaderVertexFileName.c_str(), bufV ) )
  {
    if( LoadShaderCode( shaderFragFileName.c_str(), bufF ) )
    {
      shader = Shader::New( bufV.data() , bufF.data() );
    }
  }
  return shader;
}

}//namespace


namespace SceneLauncher
{

void DliLoader::GetCameraParameters( const TreeNode* rootParser, unsigned int eidx, DliCameraParameters& camera )
{
  const TreeNode* cameras = rootParser->GetChild( "cameras" );

  if( NULL == cameras )
  {
    return;
  }

  const TreeNode* node = Tidx(cameras, eidx);
  const TreeNode* parameter;
  if(node)
  {
    if((parameter = node->GetChild( "fov" )))
    {
      camera.cameraFov = parameter->GetFloat();
    }
    if((parameter = node->GetChild( "near" )))
    {
      camera.cameraNear = parameter->GetFloat();
    }
    if((parameter = node->GetChild( "far" )))
    {
      camera.cameraFar = parameter->GetFloat();
    }
    if((parameter = node->GetChild( "position" )))
    {
      ParseVector3(*parameter, camera.cameraPosition);
    }
  }
}

DliLoader::DliLoader()
: mNodes( NULL ),
  mShaderArrayPtr( NULL ),
  mMaxLOD( 8.f )
{
}

DliLoader::~DliLoader()
{
}

void DliLoader::CreateEnvironmentTextures( std::string cubeDiffuse, std::string cubeSpecular, Texture &eDiffuseTexture, Texture &eSpecularTexture )
{
  // This texture should have 6 faces and only one mipmap
  CubeData diffuse;
  bool result = LoadCubeMapFromKtxFile( ASSET_TEXTURE_DIR + cubeDiffuse, diffuse );

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

  // This texture should have 6 faces and 6 mipmaps
  CubeData specular;
  result = LoadCubeMapFromKtxFile( ASSET_TEXTURE_DIR + cubeSpecular, specular);

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

void DliLoader::CreateTextures(std::string strTexture[4], Texture eTexture[4] )
{
  PixelData pixelData;
  for(int i = 0; i < 4; i++)
  {
    if( strTexture[i].empty() )
      return;
    pixelData = SyncImageLoader::Load( ASSET_TEXTURE_DIR + strTexture[i] );

    if( !pixelData )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load texture." );
    }

    eTexture[i] = Texture::New( TextureType::TEXTURE_2D, pixelData.GetPixelFormat(), pixelData.GetWidth(), pixelData.GetHeight() );
    eTexture[i].Upload( pixelData, 0, 0, 0, 0, pixelData.GetWidth(), pixelData.GetHeight() );
    eTexture[i].GenerateMipmaps();
  }

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

std::string DliLoader::GetParseError() const
{
  std::stringstream stream;

  if( mParser.ParseError() )
  {
    stream << "position: " << mParser.GetErrorPosition() << ", line: " << mParser.GetErrorLineNumber() << ", column: " << mParser.GetErrorColumn() << ", description: " << mParser.GetErrorDescription() << ".";
  }

  return stream.str();
}

bool DliLoader::LoadTextureSetArray( Texture &eCubeSpecular )
{
  const TreeNode *materials = mParser.GetRoot()->GetChild( "materials" );
  const TreeNode *environment = mParser.GetRoot()->GetChild( "environment" );
  if(!materials || !environment)
  {
    return false;
  }

  std::string cubeDiffuse = environment->GetChild( "cubeDiffuse" )->GetString();
  std::string cubeSpecular = environment->GetChild( "cubeSpecular" )->GetString();
  const TreeNode *nodeMaxLOD = environment->GetChild( "maxLOD" );

  if( nodeMaxLOD->GetType() ==  TreeNode::INTEGER )
  {
    mMaxLOD = nodeMaxLOD->GetInteger();
  }
  else
  {
    mMaxLOD = nodeMaxLOD->GetFloat();
  }
  Texture diffuseTexture;

  CreateEnvironmentTextures( cubeDiffuse, cubeSpecular, diffuseTexture, eCubeSpecular);

  for( TreeNode::ConstIterator it = (*materials).CBegin(); it != (*materials).CEnd(); ++it)
  {
    const TreeNode *node = &((*it).second);
    std::string strTexture[4];
    Texture texture[4];
    if(node->GetChild( "texture1" ))
    {
      strTexture[0] = node->GetChild( "texture1" )->GetString();
      if(node->GetChild( "texture2" ))
      {
        strTexture[1] = node->GetChild( "texture2" )->GetString();
        if(node->GetChild( "texture3" ))
        {
          strTexture[2] = node->GetChild( "texture3" )->GetString();
          if(node->GetChild( "texture4" ))
          {
            strTexture[3] = node->GetChild( "texture4" )->GetString();
          }
        }
      }
    }
    CreateTextures(strTexture, texture );

    TextureSet textureSet = TextureSet::New();

    Sampler samplerA = Sampler::New();
    samplerA.SetWrapMode(WrapMode::REPEAT,WrapMode::REPEAT);
    samplerA.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR,FilterMode::LINEAR);

    Sampler sampler = Sampler::New();
    sampler.SetWrapMode(WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE);
    sampler.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR,FilterMode::LINEAR);
    unsigned int i = 0;
    for( ; i < 4u ; i++)
    {
      if(texture[i])
      {
        textureSet.SetTexture( i , texture[i] );
        textureSet.SetSampler( i, samplerA );
      }
      else
      {
        break;
      }
    }
    textureSet.SetTexture( i , diffuseTexture );
    textureSet.SetTexture( i + 1 , eCubeSpecular );
    textureSet.SetSampler( i + 1 , sampler );
    mTextureSetArray.push_back( textureSet );
  }

  return true;
}

bool DliLoader::LoadShaderArray(std::vector<Shader> &shaderArray)
{
  const TreeNode *shaders = mParser.GetRoot()->GetChild( "shaders" );
  if(!shaders)
  {
    return false;
  }

  for( TreeNode::ConstIterator it = (*shaders).CBegin(); it != (*shaders).CEnd(); ++it)
  {
    const TreeNode *node = &((*it).second);

    std::string vertex = node->GetChild( "vertex" )->GetString();
    std::string fragment = node->GetChild( "fragment" )->GetString();
    Shader shader = LoadShaders( ASSET_SHADER_DIR + vertex, ASSET_SHADER_DIR + fragment );

    if( !shader )
    {
      throw DaliException( ASSERT_LOCATION, "Failed to load shader." );
      return false;
    }
    else
    {
      shaderArray.push_back( shader );
    }
    const TreeNode *nodeBlend = node->GetChild( "blend" );
    RendererOptions renderer;
    if( nodeBlend )
    {
      renderer.blend = nodeBlend->GetInteger();
    }
    mRendererOptionsArray.push_back(renderer);
    //mMaxLOD is set from the LoadTextureSetArray function
    shader.RegisterProperty( "uMaxLOD", mMaxLOD );
    shader.RegisterProperty( "uCubeMatrix" , Matrix::IDENTITY );
  }
  mShaderArrayPtr = &shaderArray;
  return true;
}

bool DliLoader::LoadAnimations( Actor toActor )
{
  const TreeNode *root = mParser.GetRoot();
  const TreeNode *animations = root->GetChild( "animations" );

  if( NULL == animations )
  {
    return false;
  }

  for( TreeNode::ConstIterator animIt = ( *animations ).CBegin(); animIt != ( *animations ).CEnd(); ++animIt )
  {
    Animation animation( Animation::New( 0.0f ) );
    float duration;
    float durationSum = 0.0f;
    const TreeNode &animNode = (*animIt).second;
    if( ReadFloat( animNode.GetChild( "duration" ), duration ) )
      animation.SetDuration( duration );

    bool looping;
    if( ReadBool( animNode.GetChild( "loop" ), looping ) )
      animation.SetLooping( looping );

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
        if( ReadString( pKeyChild.second.GetChild( "actor" ), actorName ) )
        {

        }
        if( ReadString( pKeyChild.second.GetChild( "property" ), actorProperty ) )
        {
          animActor = toActor.FindChildByName( actorName );
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
        if( pKeyChild.second.GetChild( "keyFrames" ) )
        {


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
            DALI_LOG_WARNING( "Property: type does not match value type '%s'\n",
                              PropertyTypes::GetName( prop.object.GetPropertyType(prop.propertyIndex) ) );

            throw;
          }

          bool looping;
          if( ReadBool( pKeyChild.second.GetChild( "relative" ), looping ) )
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
    animation.Play();
  }

  return true;
}

bool DliLoader::CreateScene( std::vector<Shader> &shaderArray, Actor toActor, Texture &specularTexture )
{
  const TreeNode *root = mParser.GetRoot();
  const TreeNode *inodes = NULL;
  const TreeNode *itn = NULL;

  itn = root->GetChild("scene");
  int scene_load = itn->GetInteger();

  if(!((itn = root->GetChild( "scenes" ) ) && (mNodes = root->GetChild( "nodes" ) ) ) )
    return false;

  itn = Tidx(itn, scene_load);
  if(!itn)
    return false;


  LoadTextureSetArray(specularTexture);
  LoadShaderArray(shaderArray);
  LoadGeometryArray();

  int starting_node = itn->GetInteger();

  inodes = Tidx(mNodes, starting_node);
  AddNode( toActor, inodes );

  LoadAnimations( toActor );
  mShaderArrayPtr = NULL;
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

bool DliLoader::LoadBuffer(const TreeNode* mesh, Geometry geometry, std::string& ebinFilename, unsigned char*& efileContent)
{
  unsigned int byteOffset = 0;
  unsigned int byteLength = 0;
  const TreeNode *temp = mesh->GetChild( "uri" );
  if(!temp)
    return false;

  if( ebinFilename.compare( temp->GetString() ) )
  {
    ebinFilename = temp->GetString();

    if( !readFile( efileContent, mDirectory, ebinFilename) )
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
  temp = mesh->GetChild( "primitive" );
  std::string primitive(temp->GetString());

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
  byteOffset = temp->GetChild( "byteOffset" )->GetInteger();
  byteLength = temp->GetChild( "byteLength" )->GetInteger();
  geometry.SetIndexBuffer ((unsigned short*) &efileContent[byteOffset], byteLength / sizeof( unsigned short ) );

  temp = mesh->GetChild( "positions" );
  byteOffset = temp->GetChild( "byteOffset" )->GetInteger();
  byteLength = temp->GetChild( "byteLength" )->GetInteger();
  Property::Map positionMap;
  positionMap["aPosition"] = Property::VECTOR3;
  PropertyBuffer positionBuffer = PropertyBuffer::New( positionMap );
  positionBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector3 ) ) );

  temp = mesh->GetChild( "normals" );
  byteOffset = temp->GetChild( "byteOffset" )->GetInteger();
  byteLength = temp->GetChild( "byteLength" )->GetInteger();
  Property::Map normalMap;
  normalMap["aNormal"] = Property::VECTOR3;
  PropertyBuffer normalBuffer = PropertyBuffer::New( normalMap );
  normalBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector3 ) ) );


  temp = mesh->GetChild( "textures" );
  byteOffset = temp->GetChild( "byteOffset" )->GetInteger();
  byteLength = temp->GetChild( "byteLength" )->GetInteger();
  Property::Map textCoordMap;
  textCoordMap["aTexCoord"] = Property::VECTOR2;
  PropertyBuffer texCoordBuffer = PropertyBuffer::New( textCoordMap );
  texCoordBuffer.SetData( &efileContent[byteOffset], byteLength / ( sizeof( Vector2 ) ) );

  temp = mesh->GetChild( "tangents" );
  byteOffset = temp->GetChild( "byteOffset" )->GetInteger();
  byteLength = temp->GetChild( "byteLength" )->GetInteger();
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

void DliLoader::AddNode( Actor toActor, const TreeNode *addnode)
{
  const TreeNode *itn = NULL;
  Actor actor;
  if( !( itn = addnode->GetChild( "mesh" ) ) )
  {
    actor = Actor::New();
    actor.SetAnchorPoint( AnchorPoint::CENTER );
    actor.SetParentOrigin( ParentOrigin::CENTER );
    actor.SetPosition( Vector3() );
    actor.SetSize( Vector3::ONE );
    const TreeNode *itname = addnode->GetChild( "name" );
    if( itname )
    {
      actor.SetName(itname->GetString());
    }
  }
  else
  {
    const TreeNode *itnode = addnode->GetChild( "name" );
    std::string nodeName;
    int shaderIdx = 0;
    int materialIndex = 0;
    if(itnode)
    {
      nodeName = itnode->GetString();
    }
    itnode = addnode->GetChild( "shader" );
    if(itnode)
    {
      shaderIdx = itnode->GetInteger();
    }
    itnode = addnode->GetChild( "material" );
    if(itnode)
    {
      materialIndex = itnode->GetInteger();
    }
    actor = ModelPbr::CreateNode( (*mShaderArrayPtr)[shaderIdx], mRendererOptionsArray[shaderIdx].blend, mTextureSetArray[materialIndex], mGeometryArray[ itn->GetInteger() ], nodeName );
  }


  if( ( itn = addnode->GetChild( "matrix" ) ) )
  {
    int i = 0;
    if( itn->GetType() == TreeNode::ARRAY && itn->Size() == 16 )
    {
      float num[16];
      for( TreeNode::ConstIterator it = (*itn).CBegin(); it != (*itn).CEnd(); ++it, ++i )
      {
        const TreeNode& coord = (*it).second;
        if( coord.GetType() == TreeNode::INTEGER )
        {
          num[i] = ((*it).second).GetInteger();
        }else
        {
          num[i] = ((*it).second).GetFloat();
        }
      }
      Dali::Quaternion quaternion( Vector3( num ), Vector3( num + 4 ), Vector3( num + 8 ) );
      actor.SetOrientation( quaternion );
      actor.SetPosition( Vector3( num + 12 ) );
    }
  }

  toActor.Add( actor );


  if((itn = addnode->GetChild("children")))
  {
    Vector<int> children;
    children.Reserve(itn->Size());
    for( TreeNode::ConstIterator it = (*itn).CBegin(); it != (*itn).CEnd(); ++it )
    {
      children.PushBack( ( (*it).second).GetInteger() );
    }

    for( unsigned int i = 0; i < children.Size(); ++i )
    {
      const TreeNode *inodes = Tidx( mNodes, children[i] );
      AddNode( actor, inodes );
    }
  }

}

} // namespace SceneLauncher
