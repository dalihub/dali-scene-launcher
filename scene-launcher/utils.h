#ifndef SCENE_LAUNCHER_UTILS_H_
#define SCENE_LAUNCHER_UTILS_H_
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

#include <dali/dali.h>

///@brief Call @a fn with the given @a actor and its children, depth-first.
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

///@brief Create a new actor with all concievable properties (excluding
/// actual Dali::Property-s at the moment) of the given actor @a.
Dali::Actor CloneActor(Dali::Actor a);

/**
* @brief Load vertex and fragment shader source
* @param[in] shaderVertexFileName is the filepath of Vertex shader
* @param[in] shaderFragFileName is the filepath of Fragment shader
* @return the Dali::Shader object
*/
Dali::Shader CreateShader(std::string const& vertexPath, std::string const& fragmentPath);

#endif /* SCENE_LAUNCHER_UTILS_H_ */