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
// EXTERNAL
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/animation/constraints.h>
#include <iostream>
#include <fstream>
#include <cstring>

// INTERNAL
#include "utils.h"

using namespace Dali;

namespace SceneLauncher
{

char ExceptionFlinger::sMessageBuffer[];

std::string LoadTextFile(const char * path, bool* fail)
{
  std::ifstream inFile(path);
  if (inFile)
  {
    std::istreambuf_iterator<char> eos;
    std::istreambuf_iterator<char> i(inFile.rdbuf());
    return std::string(i, eos);
  }
  else if (fail)
  {
    *fail = true;
  }
  return std::string();
}

TextureSet CloneTextures(TextureSet ts)
{
  auto tsNew = TextureSet::New();
  auto numTextures = ts.GetTextureCount();
  decltype(numTextures) i = 0;
  while (numTextures > 0)
  {
    Texture t = ts.GetTexture(i);
    tsNew.SetTexture(i, t);
    tsNew.SetSampler(i, ts.GetSampler(i));

    if (t)
    {
      --numTextures;
    }
    ++i;
  }
  return tsNew;
}

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
    auto actualIndex = i;
//    auto actualIndex = PropertyRanges::DEFAULT_RENDERER_PROPERTY_START_INDEX + i;
    auto value = r.GetProperty(actualIndex);
    if (rNew.GetPropertyType(actualIndex) == Property::NONE)
    {
      rNew.RegisterProperty(r.GetPropertyName(actualIndex), value);
    }
    else
    {
      rNew.SetProperty(actualIndex, value);
    }
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
    auto value = a.GetProperty(actualIndex);
    if (aNew.GetPropertyType(actualIndex) == Property::NONE)
    {
      aNew.RegisterProperty(a.GetPropertyName(actualIndex), value);
    }
    else
    {
      aNew.SetProperty(actualIndex, value);
    }
  }

  // NOTE: for whatever reason, setting the Actor::Property::SIZE is broken;
  // we have to explicitly SetSize() -- and it has to happen after any
  // SetProperty(Actor::Property::SIZE).
  aNew.SetSize(a.GetProperty(Actor::Property::SIZE).Get<Vector3>());  // W H Y ?

  // Clone renderers.
  if (!MaskMatch(cloneOptions, CloneOptions::OMIT_RENDERERS))
  {
    for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
    {
      Renderer rNew = CloneRenderer(a.GetRendererAt(i), cloneOptions);
      aNew.AddRenderer(rNew);
    }
  }

  // Constrain transform.
  if (MaskMatch(cloneOptions, CloneOptions::CONSTRAIN_XFORM))
  {
    Constraint constraint = Constraint::New<Vector3>(aNew, Actor::Property::POSITION, EqualToConstraint() );
    constraint.AddSource( Source( a, Actor::Property::POSITION ));
    constraint.Apply();

    constraint = Constraint::New<Quaternion>(aNew, Actor::Property::ORIENTATION, EqualToConstraint() );
    constraint.AddSource( Source( a, Actor::Property::ORIENTATION ));
    constraint.Apply();
  }

  if (MaskMatch(cloneOptions, CloneOptions::CONSTRAIN_VISIBILITY))
  {
    Constraint constraint = Constraint::New<bool>(aNew, Actor::Property::VISIBLE,
      []( bool& current, const PropertyInputContainer& inputs) {
      current = inputs[0]->GetBoolean();
    });
    constraint.AddSource(Source(a, Actor::Property::VISIBLE));
    constraint.Apply();
  }

  // Recurse into children.
  if (!MaskMatch(cloneOptions, CloneOptions::NO_RECURSE))
  {
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

Shader CreateShader( const std::string& shaderVertexFileName, const std::string& shaderFragFileName )
{
  Shader shader;

  bool fail;
  std::string vsh = LoadTextFile(shaderVertexFileName.c_str(), &fail);
  if (!fail)
  {
    std::string fsh = LoadTextFile(shaderFragFileName.c_str(), &fail);
    if (!fail)
    {
      shader = Shader::New( vsh, fsh, Shader::Hint::MODIFIES_GEOMETRY );
    }
  }
  return shader;
}

Geometry MakeTexturedQuadGeometry(TexturedQuadOptions::Type options)
{
  Property::Map properties;
  properties.Insert("aPosition", Property::VECTOR3);
  properties.Insert("aTexCoord", Property::VECTOR2);

  std::vector<uint8_t> bytes;
  size_t stride = 0;
  size_t uvOffset = 0;
  if (MaskMatch(options, TexturedQuadOptions::GENERATE_BARYCENTRICS))
  {
    properties.Insert("aBarycentric", Property::VECTOR3);

    struct
    {
      Vector3 aPosition;
      Vector2 aTexCoord;
      Vector3 aBarycentric;
    } vertices[] = {
      { Vector3(-0.5f, 0.5f, 0.0f), Vector2(0.0f, .0f), Vector3::XAXIS },
      { Vector3(0.5f, 0.5f, 0.0f), Vector2(1.0f, .0f), Vector3::ZAXIS },
      { Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f), Vector3::YAXIS },
      { Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f), Vector3::XAXIS }
    };

    bytes.resize(sizeof(vertices));
    stride = sizeof(vertices[0]);
    uvOffset = reinterpret_cast<const uint8_t*>(&vertices[0].aTexCoord) - reinterpret_cast<const uint8_t*>(&vertices[0]);

    std::memcpy(bytes.data(), vertices, sizeof(vertices));
  }
  else
  {
    struct
    {
      Vector3 aPosition;
      Vector2 aTexCoord;
    } vertices[] = {
      { Vector3(-0.5f, 0.5f, 0.0f), Vector2(0.0f, .0f) },
      { Vector3(0.5f, 0.5f, 0.0f), Vector2(1.0f, .0f) },
      { Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f) },
      { Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f) }
    };

    bytes.resize(sizeof(vertices));
    stride = sizeof(vertices[0]);
    uvOffset = reinterpret_cast<const uint8_t*>(&vertices[0].aTexCoord) - reinterpret_cast<const uint8_t*>(&vertices[0]);

    std::memcpy(bytes.data(), vertices, sizeof(vertices));
  }

  if (MaskMatch(options, TexturedQuadOptions::FLIP_VERTICAL))
  {
    Vector2* uv = reinterpret_cast<Vector2*>(reinterpret_cast<uint8_t*>(bytes.data()) + uvOffset);
    for (int i = 0; i < 4; ++i)
    {
      uv->y = 1.0f - uv->y;
      uv = reinterpret_cast<Vector2*>(reinterpret_cast<uint8_t*>(uv) + stride);
    }
  }

  PropertyBuffer vertexBuffer = PropertyBuffer::New(properties);
  vertexBuffer.SetData(bytes.data(), bytes.size() / stride);

  Geometry geometry = Geometry::New();
  geometry.AddVertexBuffer(vertexBuffer);
  geometry.SetType(Geometry::TRIANGLE_STRIP);
  return geometry;
}

Renderer MakeTexturedQuadRenderer(Texture texture, Shader shader, TexturedQuadOptions::Type options)
{
  Geometry geometry = MakeTexturedQuadGeometry(options);

  TextureSet ts = TextureSet::New();
  ts.SetTexture(0, texture);

  Renderer r = Renderer::New(geometry, shader);
  // NOTE: alpha blending is AUTO by default, thus depends on an alpha channel being present in the given texture.
  r.SetProperty(Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::BACK);
  r.SetTextures(ts);
  return r;
}

Actor MakeTexturedQuadActor(Texture texture, Shader shader, TexturedQuadOptions::Type options)
{
  Actor quad = Actor::New();
  SetActorCentered(quad);

  Renderer r = MakeTexturedQuadRenderer(texture, shader, options);
  quad.AddRenderer(r);  // by reference, why?
  quad.SetVisible(true);
  return quad;
}

}
