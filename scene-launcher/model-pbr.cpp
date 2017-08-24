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
#include <cstdio>
#include <string.h>
#include <dali/devel-api/actors/actor-devel.h>

// INTERNAL INCLUDES
#include "obj-loader.h"
#include "dli-loader.h"

namespace
{

struct Vertices
{
  Vector<Vector3> positions;
  Vector<Vector3> normals;
  Vector<Vector3> tangents;
  Vector<Vector2> texCoords;
};

} // namespace

namespace SceneLauncher
{

ModelPbr::ModelPbr()
{
}

ModelPbr::~ModelPbr()
{
}

void ModelPbr::Init( const std::string& modelUrl, const Vector3& position, const Vector3& size )
{
  mActor = Actor::New();
  mActor.SetAnchorPoint( AnchorPoint::CENTER );
  mActor.SetParentOrigin( ParentOrigin::CENTER );
  mActor.SetPosition( position );
  mActor.SetSize( size );

  if( modelUrl.rfind( ".dli" ) + 4 == modelUrl.length() )
  {
    //If it is a DLI file, ignore "shader" parameter
    DliLoader dliLoader;
    if( dliLoader.LoadObject( modelUrl ) )
    {
      dliLoader.CreateScene(mShaderArray, mActor, mCubeSpecularTexture);
    }
    else
    {
      throw DaliException( ASSERT_LOCATION, dliLoader.GetParseError().c_str() );
    }
  }
  else
  {
    std::vector<Geometry> geometry;
    std::vector<std::string> names;
    CreateGeometry( modelUrl, geometry, names );

    std::vector<Geometry>::iterator gIt = geometry.begin(), gEndIt = geometry.end();
    std::vector<std::string>::iterator nIt = names.begin(), nEndIt = names.end();
    for( ; ( gIt != gEndIt ) && ( nIt != nEndIt ); ++gIt, ++ nIt )
    {
      Renderer renderer = Renderer::New( *gIt, mShaderArray[0] );
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
      renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::BACK );

      Actor actor = Actor::New();
      actor.SetName( name );

      actor.SetAnchorPoint( AnchorPoint::CENTER );
      actor.SetParentOrigin( ParentOrigin::CENTER );
      actor.SetSize( size );
      actor.AddRenderer( renderer );

      mActor.Add( actor );
    }
  }
}

void ModelPbr::InitPbrTexture(Texture albedoM, Texture normalR, Texture texDiffuse, Texture texSpecular)
{
  mTextureSet = TextureSet::New();
  mTextureSet.SetTexture( 0u, albedoM );
  mTextureSet.SetTexture( 1u, normalR );
  mTextureSet.SetTexture( 2u, texDiffuse );
  mTextureSet.SetTexture( 3u, texSpecular );

  Sampler samplerA = Sampler::New();
  samplerA.SetWrapMode(WrapMode::REPEAT,WrapMode::REPEAT);
  samplerA.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR,FilterMode::LINEAR);
  mTextureSet.SetSampler(0,samplerA);
  mTextureSet.SetSampler(1,samplerA);

  Sampler sampler = Sampler::New();
  sampler.SetWrapMode(WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE,WrapMode::CLAMP_TO_EDGE);
  sampler.SetFilterMode(FilterMode::LINEAR_MIPMAP_LINEAR,FilterMode::LINEAR);
  mTextureSet.SetSampler(3,sampler);
}

void ModelPbr::Clear()
{
  mTextureSet.Reset();
  for(std::vector<Shader>::iterator it = mShaderArray.begin(); it !=mShaderArray.end(); ++it)
  {
     (*it).Reset();
  }
  mShaderArray.clear();
  mCubeSpecularTexture.Reset();
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
  if( url.rfind( ".obj" ) + 4 == url.length() )
  {
    if( FileLoader::ReadFile( url, fileSize, fileContent, FileLoader::TEXT ) )
    {
      PbrDemo::ObjLoader objLoader;

      objLoader.LoadObject( fileContent.Begin(), fileSize );

      objLoader.CreateGeometry( PbrDemo::ObjLoader::TEXTURE_COORDINATES | PbrDemo::ObjLoader::TANGENTS, true, geometry, names );
    }
    else
    {
      throw DaliException( ASSERT_LOCATION, "Failed to load obj model.");
    }
  }
  else
  {
     throw DaliException( ASSERT_LOCATION, "Format not supported");
  }

}

void ModelPbr::SetShaderUniform(std::string property, const Property::Value& value)
{

  for(std::vector<Shader>::iterator it = mShaderArray.begin(); it != mShaderArray.end(); ++it)
  {
    (*it).SetProperty( (*it).GetPropertyIndex( property ), value );
  }

}

Texture ModelPbr::GetCubeSpecularTexture()
{
  return mCubeSpecularTexture;
}

Actor ModelPbr::CreateNode( Shader shader, bool blend, TextureSet textureSet, Geometry geometry, std::string name )
{
  Renderer renderer = Renderer::New( geometry, shader );
  renderer.SetTextures( textureSet );
  if( !blend )
  {
    // Face culling is enabled to hide the backwards facing sides of the model
    // This is sufficient to render a single object; for more complex scenes depth-testing might be required
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_FUNCTION, DepthFunction::LESS_EQUAL );
    renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::BACK );
  }
  else
  {
    //renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );

    renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::NONE );
    renderer.SetProperty( Renderer::Property::BLEND_MODE, BlendMode::ON );
    renderer.SetProperty( Renderer::Property::BLEND_FACTOR_DEST_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA );

  }

  Actor actor = Actor::New();
  actor.SetAnchorPoint( AnchorPoint::CENTER );
  actor.SetParentOrigin( ParentOrigin::CENTER );
  actor.SetPosition( Vector3() );
  actor.SetSize( Vector3::ONE );
  actor.AddRenderer( renderer );
  actor.SetName(name);

  return actor;
}


} // namespace SceneLauncher
