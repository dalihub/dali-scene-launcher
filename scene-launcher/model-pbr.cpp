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
#include "model-pbr.h"

// EXTERNAL INCLUDES
#include <dali/public-api/rendering/renderer.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/devel-api/actors/actor-devel.h>

#include <algorithm>

// INTERNAL INCLUDES
#include "asset.h"
#include "dli-loader.h"

namespace
{

const std::string DLI_EXT( ".dli" );
const size_t DLI_EXT_SIZE = DLI_EXT.size();

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

int ModelPbr::mOrderIdx = 0;

ModelPbr::ModelPbr()
{
}

ModelPbr::~ModelPbr()
{
}

void ModelPbr::Init( Asset& asset, const Vector3& position, std::vector<std::vector<Animation>>& animations, std::vector<std::string>& animationsName )
{
  mActor = Actor::New();
  mActor.SetAnchorPoint( AnchorPoint::CENTER );
  mActor.SetParentOrigin( ParentOrigin::CENTER );
  mActor.SetPosition( position );
  mActor.SetSize( asset.modelScaleFactor );

  if( asset.model.rfind( DLI_EXT ) + DLI_EXT_SIZE == asset.model.length() )
  {
    //If it is a DLI file, ignore "shader" parameter
    DliLoader dliLoader;
    if( dliLoader.LoadObject( asset.model ) )
    {
      dliLoader.CreateScene( mShaderArray, mActor, mSkyboxTexture );
      dliLoader.GetCameraParameters( 0, asset.camera );

      for( const auto& animationName : animationsName )
      {
        std::vector<Animation> aniItem;
        dliLoader.LoadAnimation( mActor, aniItem, animationName );
        animations.push_back( aniItem );
      }
    }
    else
    {
      throw DaliException( ASSERT_LOCATION, dliLoader.GetParseError().c_str() );
    }
  }
}

void ModelPbr::Clear()
{
  for( std::vector<Shader>::iterator it = mShaderArray.begin(); it !=mShaderArray.end(); ++it )
  {
     (*it).Reset();
  }
  mShaderArray.clear();
  mSkyboxTexture.Reset();
  UnparentAndReset( mActor );
}

Actor& ModelPbr::GetActor()
{
  return mActor;
}

bool ModelPbr::GetUniform( const std::string& property, Property::Value& value, int shaderIndex )
{
  if( shaderIndex < 0 )
  {
    for( std::vector<Shader>::iterator it = mShaderArray.begin(); it != mShaderArray.end(); ++it )
    {
      int index = (*it).GetPropertyIndex( property );
      if( index != Property::INVALID_INDEX )
      {
        value = (*it).GetProperty( index );
        return true;
      }
    }
  }
  else if( shaderIndex < static_cast<int>( mShaderArray.size() ) )
  {
    int index = mShaderArray[shaderIndex].GetPropertyIndex( property );
    if( index != Property::INVALID_INDEX )
    {
      value = mShaderArray[shaderIndex].GetProperty( index );
      return true;
    }
  }
  return false;
}

void ModelPbr::SetShaderUniform( const std::string& property, const Property::Value& value )
{
  for( std::vector<Shader>::iterator it = mShaderArray.begin(); it != mShaderArray.end(); ++it )
  {
    int index = (*it).GetPropertyIndex( property );
    if( index != Property::INVALID_INDEX )
    {
      (*it).SetProperty( (*it).GetPropertyIndex( property ), value );
    }
  }
}

void ModelPbr::SetShaderAnimationUniform( const std::string& property, const Property::Value& value, AlphaFunction alpha, TimePeriod etime )
{
  for( std::vector<Shader>::iterator it = mShaderArray.begin(); it != mShaderArray.end(); ++it )
  {
    int index = (*it).GetPropertyIndex( property );
    if( index != Property::INVALID_INDEX )
    {
      Animation animation;
      animation = Animation::New( 3.0f );
      animation.AnimateTo( Property( *it, index ) , value, alpha, etime );
      animation.Play();
    }
  }
}

Texture ModelPbr::GetSkyboxTexture()
{
  return mSkyboxTexture;
}

void ModelPbr::Duplicate(ModelPbr& other, SceneLauncher::CloneOptions::Type cloneOptions) const
{
  other.mActor = CloneActor(mActor, cloneOptions);
  if (MaskMatch(cloneOptions, GET_SHADERS))
  {
    other.mShaderArray.assign(mShaderArray.begin(), mShaderArray.end());
  }

  if (MaskMatch(cloneOptions, GET_SKYBOX_TEXTURE))
  {
    other.mSkyboxTexture = mSkyboxTexture;
  }
}

void ModelPbr::AttachTexture(Texture texture, Sampler sampler)
{
  std::vector<TextureSet> textureSets;
  textureSets.reserve(16);

  auto fn = [&sampler, &texture, &textureSets](Actor a){
    unsigned int numRenderers = a.GetRendererCount();
    for(unsigned int i = 0; i < numRenderers; ++i)
    {
      TextureSet ts = a.GetRendererAt(i).GetTextures();

      auto iInsert = std::lower_bound(textureSets.begin(), textureSets.end(), ts);
      if (iInsert == textureSets.end() || *iInsert != ts)
      {
        auto count = ts.GetTextureCount();
        ts.SetTexture(count, texture);
        ts.SetSampler(count, sampler);

        textureSets.insert(iInsert, ts);
      }
    }
  };

  VisitActor(mActor, fn);
}

Actor ModelPbr::CreateNode( Shader shader, int blend, TextureSet textureSet, Geometry geometry, const Vector3& actorSize, const std::string& name )
{
  Renderer renderer = Renderer::New( geometry, shader );

  if( textureSet )
  {
    renderer.SetTextures( textureSet );
  }

  if( blend == 1 ) //to use mask textures with blend
  {
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::OFF );
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::OFF );

    renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::NONE );
    renderer.SetProperty( Renderer::Property::BLEND_MODE, BlendMode::ON );
  }
  else if( blend == 2 ) //to use "Shadow layers"
  {
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::OFF );
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );

    renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::NONE );
    renderer.SetProperty( Renderer::Property::BLEND_MODE, BlendMode::ON );
    renderer.SetProperty( Renderer::Property::BLEND_FACTOR_SRC_RGB, BlendFactor::ONE_MINUS_DST_ALPHA );
  }
  else if( blend == 3 )
  {
    // The stencil plane is only for stencilling.
    renderer.SetProperty( Renderer::Property::RENDER_MODE, RenderMode::STENCIL );

    renderer.SetProperty( Renderer::Property::STENCIL_FUNCTION, StencilFunction::ALWAYS );
    renderer.SetProperty( Renderer::Property::STENCIL_FUNCTION_REFERENCE, 1 );
    renderer.SetProperty( Renderer::Property::STENCIL_FUNCTION_MASK, 0xFF );
    renderer.SetProperty( Renderer::Property::STENCIL_OPERATION_ON_FAIL, StencilOperation::KEEP );
    renderer.SetProperty( Renderer::Property::STENCIL_OPERATION_ON_Z_FAIL, StencilOperation::KEEP );
    renderer.SetProperty( Renderer::Property::STENCIL_OPERATION_ON_Z_PASS, StencilOperation::REPLACE );
    renderer.SetProperty( Renderer::Property::STENCIL_MASK, 0xFF );

    // We don't want to write to the depth buffer, as this would block the reflection being drawn.
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::OFF );
    // We test the depth buffer as we want the stencil to only exist underneath the cube.
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );
  }
  else if( blend == 4 )
  {
    // Also enable the stencil buffer, as we will be testing against it to only draw to areas within the stencil.
    renderer.SetProperty( Renderer::Property::RENDER_MODE, RenderMode::COLOR_STENCIL );
    renderer.SetProperty( Renderer::Property::BLEND_MODE, BlendMode::ON );

    renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::NONE );
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::OFF );
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );

    // Enable stencil. Here we only draw to areas within the stencil.
    renderer.SetProperty( Renderer::Property::STENCIL_FUNCTION, StencilFunction::EQUAL );
    renderer.SetProperty( Renderer::Property::STENCIL_FUNCTION_REFERENCE, 1 );
    renderer.SetProperty( Renderer::Property::STENCIL_FUNCTION_MASK, 0xff );
    // Don't write to the stencil.
    renderer.SetProperty( Renderer::Property::STENCIL_MASK, 0x00 );
  }
  else //use default renderer options
  {
    // Face culling is enabled to hide the backwards facing sides of the model
    // This is sufficient to render a single object; for more complex scenes depth-testing might be required
    renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::ON );
    renderer.SetProperty( Renderer::Property::DEPTH_FUNCTION, DepthFunction::LESS_EQUAL );
    renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::BACK );
  }

  Actor actor = Actor::New();
  actor.SetAnchorPoint( AnchorPoint::CENTER );
  actor.SetParentOrigin( ParentOrigin::CENTER );
  actor.SetProperty( DevelActor::Property::SIBLING_ORDER, mOrderIdx++ );
  actor.SetColor( Vector4::ONE );
  actor.SetPosition( Vector3::ZERO );
  actor.SetSize( actorSize );
  actor.AddRenderer( renderer );
  actor.SetName(name);

  return actor;
}


} // namespace SceneLauncher
