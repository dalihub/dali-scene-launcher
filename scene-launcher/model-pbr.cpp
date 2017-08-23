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
#include "model-pbr.h"

// EXTERNAL INCLUDES
#include <dali/devel-api/adaptor-framework/file-loader.h>
#include <dali/integration-api/debug.h>
#include <cstdio>
#include <string.h>

// INTERNAL INCLUDES
#include "obj-loader.h"

namespace
{

struct Vertices
{
  Vector<Vector3> positions;
  Vector<Vector3> normals;
  Vector<Vector3> tangents;
  Vector<Vector2> texCoords;
};

#if defined(DEBUG_ENABLED)
Dali::Integration::Log::Filter* gLogFilter = Dali::Integration::Log::Filter::New(Debug::NoLogging, false, "LOG_MODEL_PBR");
#endif

} // namespace

namespace PbrDemo
{

ModelPbr::ModelPbr()
{
}

ModelPbr::~ModelPbr()
{
}

void ModelPbr::Init( Shader shader, const std::string& modelUrl, const Vector3& position, const Vector3& size )
{
  std::vector<Geometry> geometry;
  std::vector<std::string> names;
  CreateGeometry( modelUrl, geometry, names );

  mActor = Actor::New();
  mActor.SetAnchorPoint( AnchorPoint::CENTER );
  mActor.SetParentOrigin( ParentOrigin::CENTER );
  mActor.SetPosition( position );
  mActor.SetSize( size );

  std::vector<Geometry>::iterator gIt = geometry.begin(), gEndIt = geometry.end();
  std::vector<std::string>::iterator nIt = names.begin(), nEndIt = names.end();
  for( ; ( gIt != gEndIt ) && ( nIt != nEndIt ); ++gIt, ++ nIt )
  {
    Renderer renderer = Renderer::New( *gIt, shader );
    const std::string& name = *nIt;

    if( mTextureSet )
    {
      renderer.SetTextures( mTextureSet );
    }

    // Face culling is enabled to hide the backwards facing sides of the model
    // This is sufficient to render a single object; for more complex scenes depth-testing might be required
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_FUNCTION, DepthFunction::LESS_EQUAL );

    Actor actor = Actor::New();
    actor.SetName( name );

    actor.SetAnchorPoint( AnchorPoint::CENTER );
    actor.SetParentOrigin( ParentOrigin::CENTER );
    actor.SetSize( size );
    actor.AddRenderer( renderer );

    mActor.Add( actor );
  }
}

void ModelPbr::InitPbrTexture( const ModelPbr::Textures& textures, Texture texDiffuse, Texture texSpecular )
{
  DALI_LOG_INFO( gLogFilter, Debug::General, "-->InitPbrTexture\n" );

  mTextureSet = TextureSet::New();

  Sampler samplerA = Sampler::New();
  samplerA.SetWrapMode(WrapMode::REPEAT,WrapMode::REPEAT);
  samplerA.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR,FilterMode::LINEAR);

  Sampler sampler = Sampler::New();
  sampler.SetWrapMode(WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE);
  sampler.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR,FilterMode::LINEAR);

  switch( textures.type )
  {
    case ModelPbr::Textures::ALBEDO_AND_METALNESS_AS_ALPHA_NORMAL_AND_ROUGHNESS_AS_ALPHA:
    {
      mTextureSet.SetTexture( 0u, textures.texture1 );
      mTextureSet.SetTexture( 1u, textures.texture2 );
      mTextureSet.SetTexture( 2u, texDiffuse );
      mTextureSet.SetTexture( 3u, texSpecular );

      mTextureSet.SetSampler(0,samplerA);
      mTextureSet.SetSampler(1,samplerA);
      mTextureSet.SetSampler(3,sampler);
      break;
    }
    case ModelPbr::Textures::ALBEDO_METALNESS_NORMAL_AND_ROUGHNESS_AS_ALPHA:
    case ModelPbr::Textures::ALBEDO_AND_METALNESS_AS_ALPHA_NORMAL_ROUGHNESS: // FALLTHROUGH
    {
      mTextureSet.SetTexture( 0u, textures.texture1 );
      mTextureSet.SetTexture( 1u, textures.texture2 );
      mTextureSet.SetTexture( 2u, textures.texture3 );
      mTextureSet.SetTexture( 3u, texDiffuse );
      mTextureSet.SetTexture( 4u, texSpecular );

      mTextureSet.SetSampler(0,samplerA);
      mTextureSet.SetSampler(1,samplerA);
      mTextureSet.SetSampler(2,samplerA);
      mTextureSet.SetSampler(4,sampler);
      break;
    }
    case ModelPbr::Textures::ALBEDO_METALNESS_NORMAL_ROUGHNESS:
    {
      mTextureSet.SetTexture( 0u, textures.texture1 );
      mTextureSet.SetTexture( 1u, textures.texture2 );
      mTextureSet.SetTexture( 2u, textures.texture3 );
      mTextureSet.SetTexture( 3u, textures.texture4 );
      mTextureSet.SetTexture( 4u, texDiffuse );
      mTextureSet.SetTexture( 5u, texSpecular );

      mTextureSet.SetSampler(0,samplerA);
      mTextureSet.SetSampler(1,samplerA);
      mTextureSet.SetSampler(2,samplerA);
      mTextureSet.SetSampler(3,samplerA);
      mTextureSet.SetSampler(5,sampler);
      break;
    }
  }

#if defined(DEBUG_ENABLED)
  switch( textures.type )
  {
    case ModelPbr::Textures::ALBEDO_AND_METALNESS_AS_ALPHA_NORMAL_AND_ROUGHNESS_AS_ALPHA:
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "Texture type: albedo-metalness and normal-roughness\n" );
      break;
    }
    case ModelPbr::Textures::ALBEDO_METALNESS_NORMAL_AND_ROUGHNESS_AS_ALPHA:
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "Texture type: albedo, metalness and normal-roughness\n" );
      break;
    }
    case ModelPbr::Textures::ALBEDO_AND_METALNESS_AS_ALPHA_NORMAL_ROUGHNESS:
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "Texture type: albedo-metalness, normal and roughness\n" );
      break;
    }
    case ModelPbr::Textures::ALBEDO_METALNESS_NORMAL_ROUGHNESS:
    {
      DALI_LOG_INFO( gLogFilter, Debug::General, "Texture type: albedo, metalness, normal and roughness\n" );
      break;
    }
  }
#endif

  DALI_LOG_INFO( gLogFilter, Debug::General, "<--InitPbrTexture\n" );
}

void ModelPbr::Clear()
{
  mTextureSet.Reset();
  UnparentAndReset( mActor );
}

Actor& ModelPbr::GetActor()
{
  return mActor;
}

void ModelPbr::CreateGeometry( const std::string& url,
                               std::vector<Geometry>& geometry,
                               std::vector<std::string>& names )
{
  std::streampos fileSize;
  Dali::Vector<char> fileContent;

  if( FileLoader::ReadFile( url, fileSize, fileContent, FileLoader::TEXT ) )
  {
    PbrDemo::ObjLoader objLoader;

    objLoader.ClearArrays();
    objLoader.LoadObject( fileContent.Begin(), fileSize );

    objLoader.CreateGeometry( PbrDemo::ObjLoader::TEXTURE_COORDINATES | PbrDemo::ObjLoader::TANGENTS, true, geometry, names );
  }
  else
  {
    throw DaliException( ASSERT_LOCATION, "Failed to load obj model.");
  }
}

} // namespace PbrDemo
