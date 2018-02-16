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
#include "utils.h"
#include <vector>
#include <iostream>

using namespace Dali;

namespace SceneLauncher
{

Renderer CloneRenderer(Renderer r, CloneOptions::Type cloneOptions)
{
  Geometry g = r.GetGeometry();
  Shader s = r.GetShader();
  Renderer rNew = Renderer::New(g, s);

  // Copy properties.
  Property::IndexContainer indices;
  r.GetPropertyIndices(indices);

  for (auto& i: indices)
  {
    auto actualIndex = PropertyRanges::DEFAULT_RENDERER_PROPERTY_START_INDEX + i;
    rNew.SetProperty(actualIndex, r.GetProperty(actualIndex));
  }

  // Copy texture references (and create TextureSet, if there's any textures).
  TextureSet ts = r.GetTextures();
  if(!MaskMatch(cloneOptions, CloneOptions::OMIT_TEXTURES) && ts)
  {
    TextureSet tsNew = TextureSet::New();
    for(unsigned int i = 0; i < ts.GetTextureCount(); ++i)
    {
      tsNew.SetTexture(i, ts.GetTexture(i));
      tsNew.SetSampler(i, ts.GetSampler(i));
    }
    rNew.SetTextures(tsNew);
  }

  return rNew;
}

Actor CloneActor(Actor a, CloneOptions::Type cloneOptions)
{
  Actor aNew = Actor::New();
  aNew.SetName(a.GetName());

  // Copy properties.
  Property::IndexContainer indices;
  a.GetPropertyIndices(indices);

  for (auto& i: indices)
  {
    auto actualIndex = PropertyRanges::DEFAULT_ACTOR_PROPERTY_START_INDEX + i;
    aNew.SetProperty(actualIndex, a.GetProperty(actualIndex));
  }

  // Clone renderers.
  if (!MaskMatch(cloneOptions, CloneOptions::OMIT_RENDERERS))
  {
    for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
    {
      Renderer rNew = CloneRenderer(a.GetRendererAt(i), cloneOptions);
      aNew.AddRenderer(rNew);
    }
  }

  if (!MaskMatch(cloneOptions, CloneOptions::NO_RECURSE))
  {
    // Recurse into children.
    for(unsigned int i = 0; i < a.GetChildCount(); ++i)
    {
      Actor newChild = CloneActor(a.GetChildAt(i), cloneOptions);
      aNew.Add(newChild);
    }
  }
  return aNew;
}

void DumpProperties(Dali::Handle object)
{
  Property::IndexContainer indices;
  object.GetPropertyIndices(indices);

  for (auto& i: indices)
  {
    auto prop = object.GetCurrentProperty(i);
    std::cout << "[" << i << "] " << object.GetPropertyName(i) << ": " << prop.GetType() << std::endl;
  }
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

}
