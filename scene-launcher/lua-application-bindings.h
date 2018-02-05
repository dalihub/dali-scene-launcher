#ifndef DALI_SCENE_LAUNCHER_LUA_APPLICATION_BINDINGS_H
#define DALI_SCENE_LAUNCHER_LUA_APPLICATION_BINDINGS_H

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

// EXTERNAL INCLUDES
#include <lua.hpp>

namespace SceneLauncher
{

struct LuaBindingFuction
{
  LuaBindingFuction()
  : name{ nullptr },
    function{ nullptr }
  {}

  LuaBindingFuction( const char* name, lua_CFunction function )
  : name{ name },
    function{ function }
  {}

  const char* name;
  lua_CFunction function;
};

extern const LuaBindingFuction LUA_BINDING_FUNCTIONS[];
extern const unsigned int NUMBER_OF_LUA_BINDING_FUNCTIONS;
} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_LUA_APPLICATION_BINDINGS_H
