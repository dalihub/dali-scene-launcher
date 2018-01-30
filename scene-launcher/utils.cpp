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

Actor CloneActor(Actor a)
{
  Actor b = Actor::New();
  b.SetName(a.GetName());

  b.SetAnchorPoint(a.GetProperty(Actor::Property::ANCHOR_POINT).Get<Vector3>());
  b.SetParentOrigin(a.GetProperty(Actor::Property::PARENT_ORIGIN).Get<Vector3>());
  b.SetInheritPosition(a.GetProperty(Actor::Property::INHERIT_POSITION).Get<bool>());
  b.SetVisible(a.GetProperty(Actor::Property::VISIBLE).Get<bool>());

  b.SetSize(a.GetProperty(Actor::Property::SIZE).Get<Vector3>());
  b.SetPosition(a.GetProperty(Actor::Property::POSITION).Get<Vector3>());
  b.SetOrientation(a.GetProperty(Actor::Property::ORIENTATION).Get<Quaternion>());
  b.SetScale(a.GetProperty(Actor::Property::SCALE).Get<Vector3>());
  b.SetColor(a.GetProperty(Actor::Property::COLOR).Get<Vector4>());

  for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
  {
    Renderer rA = a.GetRendererAt(i);
    Geometry g = rA.GetGeometry();
    Shader s = rA.GetShader();
    Renderer rB = Renderer::New(g, s);

    if(TextureSet tsA = rA.GetTextures())
    {
      TextureSet tsB = TextureSet::New();
      for(size_t j = 0; j < tsB.GetTextureCount(); ++j)
      {
        tsB.SetTexture(j, tsA.GetTexture(j));
      }
      rB.SetTextures(tsB);
    }

    b.AddRenderer(rB);
  }

  for(unsigned int i = 0; i < a.GetChildCount(); ++i)
  {
    b.Add(CloneActor(a.GetChildAt(i)));
  }
  return b;
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
