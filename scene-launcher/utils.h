#ifndef SCENE_LAUNCHER_UTILS_H_
#define SCENE_LAUNCHER_UTILS_H_
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

#include <dali/public-api/actors/actor.h>
#include <dali/public-api/rendering/renderer.h>

namespace SceneLauncher
{

///@return Whether all of @a mask 's bits are set on @a value.
inline
bool MaskMatch(uint32_t value, uint32_t mask)
{
  return (value & mask) == mask;
}

///@brief Insensitive case compare function.
///@param[in] a, compare string
///@param[in] b, compare string
///@return true if strings are equal
inline
bool CaseInsensitiveCharacterCompare( unsigned char a, unsigned char b )
{
  // Converts to lower case in the current locale.
  return std::tolower( a ) == std::tolower( b );
}

///@return true if the lower cased ASCII strings are equal.
///@param[in] a, compare string
///@param[in] b, compare string
inline
bool CaseInsensitiveStringCompare( const std::string& a, const std::string& b )
{
  bool result = false;
  if( a.length() == b.length() )
  {
    result = std::equal( a.begin(), a.end(), b.begin(), &CaseInsensitiveCharacterCompare );
  }
  return result;
}

///@brief Makes a number of calls to @a fn, passing to each one the given
/// @a actor then each of its children, in depth-first traversal.
///@note @a fn must not change the actor hierarchy during traversal.
///@note Use of a @a fn that is itself recursing in @a is also discouraged
/// for performance and stability reasons.
template <typename Func>
inline
void VisitActor(Dali::Actor a, Func fn)
{
  fn(a);

  unsigned int numChildren = a.GetChildCount();
  for(unsigned int i = 0; i < numChildren; ++i)
  {
    VisitActor(a.GetChildAt(i), fn);
  }
}

namespace CloneOptions
{
using Type = uint32_t;

enum Values: Type
{
  NONE = 0x00,

  OMIT_TEXTURES = 0x01,   // Do not create TextureSet and share Texture references (Renderer).

  OMIT_RENDERERS = 0x10,  // Do not clone Renderers (Actor).
  NO_RECURSE = 0x20,      // Do not clone children (Actor).
  CONSTRAIN_XFORM = 0x40, // Constrain the position and orientation of the clone to the original.

  // Local additions to CloneOptions should start at this value; consider all preceding ones reserved.
  FIRST_USER_OPTION = 0x100,
};

}

///@brief Creates a new Renderer with all conceivable properties of the given
/// Renderer @a r. A new TextureSet will be created if needed, but all Texture,
/// Shader and Geometry references are shared between @a r and the new
/// instance. The creation of a TextureSet (and subsequent sharing of Texture
/// references) may be omitted using CloneOptions::OMIT_TEXTURES. The sharing
/// of the Shader reference maybe omitted using CloneOptions::OMIT_SHADER.
///@param r The Renderer to clone.
///@param cloneOptions A bitmask constructed from the combination of CloneOptions
/// values (plus any user defined values).
Dali::Renderer CloneRenderer(Dali::Renderer r, CloneOptions::Type cloneOptions = CloneOptions::NONE);

///@brief Create a new actor with all conceivable properties (excluding actual
/// Dali::Property-s at the moment) and Renderers of the given actor @a a.
/// Will clone all children unless CloneOptions::NO_RECURSE was set.
/// Will clone Renderers unless CloneOptions::OMIT_RENDERERS was set.
///@param a The Actor to clone.
///@param cloneOptions A bitmask constructed from the combination of CloneOptions
/// values (plus any user defined values).
Dali::Actor CloneActor(Dali::Actor a, CloneOptions::Type cloneOptions = CloneOptions::NONE);

///@brief Writes all property indices, names and values from the given @a object, to stdout.
///@note To be able to use the indices in Get/SetProperty(), they will need to be offset by
/// some value based on the type; see dali/public-api/object/property-index-range.h
void DumpProperties(Dali::Handle object);

/**
* @brief Load vertex and fragment shader source
* @param[in] shaderVertexFileName is the filepath of Vertex shader
* @param[in] shaderFragFileName is the filepath of Fragment shader
* @return the Dali::Shader object
*/
Dali::Shader CreateShader(const std::string& vertexPath, const std::string& fragmentPath);

}

#endif /* SCENE_LAUNCHER_UTILS_H_ */
