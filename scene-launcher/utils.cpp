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
#include "utils.h"

using namespace Dali;

#define COPY_PROPERTY(property, a, b) (b).SetProperty(property, (a).GetProperty(property))

Renderer CloneRenderer(Renderer r)
{
  Geometry g = r.GetGeometry();
  Shader s = r.GetShader();
  Renderer rNew = Renderer::New(g, s);

  COPY_PROPERTY(Renderer::Property::FACE_CULLING_MODE, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_MODE, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_EQUATION_RGB, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_EQUATION_ALPHA, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_FACTOR_SRC_RGB, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_FACTOR_SRC_ALPHA, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_FACTOR_DEST_RGB, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_FACTOR_DEST_ALPHA, r, rNew);
  COPY_PROPERTY(Renderer::Property::BLEND_COLOR, r, rNew);
  COPY_PROPERTY(Renderer::Property::DEPTH_WRITE_MODE, r, rNew);
  COPY_PROPERTY(Renderer::Property::DEPTH_FUNCTION, r, rNew);
  COPY_PROPERTY(Renderer::Property::DEPTH_TEST_MODE, r, rNew);
  COPY_PROPERTY(Renderer::Property::RENDER_MODE, r, rNew);

  if(TextureSet ts = r.GetTextures())
  {
    TextureSet tsNew = TextureSet::New();
    for(unsigned int i = 0; i < tsNew.GetTextureCount(); ++i)
    {
      tsNew.SetTexture(i, ts.GetTexture(i));
    }
    rNew.SetTextures(tsNew);
  }

  return rNew;
}


Actor CloneActor(Actor a)
{
  Actor aNew = Actor::New();
  aNew.SetName(a.GetName() + "-cloned");

  COPY_PROPERTY(Actor::Property::ANCHOR_POINT, a, aNew);
  COPY_PROPERTY(Actor::Property::PARENT_ORIGIN, a, aNew);
  COPY_PROPERTY(Actor::Property::INHERIT_POSITION, a, aNew);
  COPY_PROPERTY(Actor::Property::VISIBLE, a, aNew);
  COPY_PROPERTY(Actor::Property::SIZE, a, aNew);
  COPY_PROPERTY(Actor::Property::POSITION, a, aNew);
  COPY_PROPERTY(Actor::Property::ORIENTATION, a, aNew);
  COPY_PROPERTY(Actor::Property::SCALE, a, aNew);
  COPY_PROPERTY(Actor::Property::COLOR, a, aNew);

  for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
  {
    Renderer rNew = CloneRenderer(a.GetRendererAt(i));
    aNew.AddRenderer(rNew);
  }

  for(unsigned int i = 0; i < a.GetChildCount(); ++i)
  {
    aNew.Add(CloneActor(a.GetChildAt(i)));
  }
  return aNew;
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


Shader CreateShader( const std::string& shaderVertexFileName, const std::string& shaderFragFileName )
{
  Shader shader;
  std::vector<char> bufV, bufF;

  if( LoadShaderCode( shaderVertexFileName.c_str(), bufV ) )
  {
    if( LoadShaderCode( shaderFragFileName.c_str(), bufF ) )
    {
      shader = Shader::New( bufV.data() , bufF.data(), Shader::Hint::MODIFIES_GEOMETRY );
    }
  }
  return shader;
}
