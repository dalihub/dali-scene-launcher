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

///@brief Makes a number of calls to @a fn, passing to each one the given
/// @a actor then each of its children, in depth-first traversal.
///@note Obviously, use of a @a fn that is itself recursing in @a is
/// discouraged for performance and stability reasons.
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

///@brief Creates a new Renderer with all conceivable properties of the given
/// Renderer @a r. A new TextureSet will be created if needed (only), but all
/// Texture, Shader and Geometry references are shared between @a r and the new
/// instance.
Dali::Renderer CloneRenderer(Dali::Renderer r);

///@brief Create a new actor with all conceivable properties (excluding actual
/// Dali::Property-s at the moment) and Renderers of the given actor @a a.
Dali::Actor CloneActor(Dali::Actor a);

/**
* @brief Load vertex and fragment shader source
* @param[in] shaderVertexFileName is the filepath of Vertex shader
* @param[in] shaderFragFileName is the filepath of Fragment shader
* @return the Dali::Shader object
*/
Dali::Shader CreateShader(const std::string& vertexPath, const std::string& fragmentPath);

#endif /* SCENE_LAUNCHER_UTILS_H_ */
