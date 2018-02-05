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
#include "lua-application-helper.h"

// EXTERNAL INCLUDES
#include <dali/public-api/common/stage.h>
#include <dali/public-api/events/key-event.h>

// INTERNAL INCLUDES
#include "lua-interface.h"

namespace SceneLauncher
{

LuaApplicationHelper::LuaApplicationHelper( Application& application, Lua& lua )
: mApplication( application ),
  mLua( lua )
{
}

LuaApplicationHelper::~LuaApplicationHelper()
{
}

void LuaApplicationHelper::Initialize()
{
  Stage stage = Stage::GetCurrent();
  stage.KeyEventSignal().Connect( this, &LuaApplicationHelper::OnKeyEvent );

  // Call the function OnCreate implemented in lua.
  mLua.FetchFunction( "OnCreate" );

  mLua.PushParameter( static_cast<void*>( this ) );

  mLua.ExecuteFunction( 0 ); // 0 is the number of returned parameters.
}

void LuaApplicationHelper::QuitApplication()
{
  mApplication.Quit();
}

void LuaApplicationHelper::OnKeyEvent( const KeyEvent& event )
{
  // Call the OnKeyEvent function in lua.
  mLua.FetchFunction( "OnKeyEvent" );

  mLua.PushParameter( event.keyCode );
  mLua.PushParameter( event.keyModifier );
  mLua.PushParameter( event.state );

  mLua.ExecuteFunction( 0 ); // 0 is the number of returned parameters.
}

} // namespace SceneLauncher
