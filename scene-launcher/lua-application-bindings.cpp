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

// FILE HEADER
#include "lua-application-bindings.h"

// EXTERNAL INCLUDES
#include <dali/public-api/actors/layer.h>
#include <dali/public-api/common/stage.h>

// INTERNAL INCLUDES
#include "lua-application-helper.h"

namespace
{
const float TWO_PI_OVER_360 = Math::PI / 180.f; // 2 * PI / 360
} // namespace

namespace SceneLauncher
{

int QuitApplication( lua_State* state );
int RotateActor( lua_State* state );

const LuaBindingFuction LUA_BINDING_FUNCTIONS[] =
{
  {
    "QuitApplication",
    QuitApplication
  },
  {
    "RotateActor",
    RotateActor
  }
};
const unsigned int NUMBER_OF_LUA_BINDING_FUNCTIONS = sizeof( LUA_BINDING_FUNCTIONS ) / sizeof( LuaBindingFuction );

int QuitApplication( lua_State* state )
{
  // Retrieves the scene-launcher pointer from the top of the stack.
  void* luaApplicationPtr = const_cast<void*>( lua_topointer( state, 1 ) );

  if( nullptr != luaApplicationPtr )
  {
    static_cast<SceneLauncher::LuaApplicationHelper*>( luaApplicationPtr )->QuitApplication();
  }

  // Pops the parameter from the top of the stack.
  lua_pop( state, 1 );

  return 0;
}

int RotateActor( lua_State* state )
{
  const std::string actorName( lua_tostring( state, 1 ) );
  const float angle = lua_tonumber( state, 2 );
  Vector3 axis;
  axis.x = lua_tonumber( state, 3 );
  axis.y = lua_tonumber( state, 4 );
  axis.z = lua_tonumber( state, 5 );

  Layer root = Stage::GetCurrent().GetRootLayer();

  Actor actor = root.FindChildByName( actorName );

  if( actor )
  {
    actor.SetOrientation( Radian( -TWO_PI_OVER_360 * angle ), axis );
  }

  return 0;
}

} // namespace SceneLauncher
