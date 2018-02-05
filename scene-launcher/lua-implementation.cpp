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
#include "lua-interface.h"

// EXTERNAL INCLUDES
#include <lua.hpp>

// INTERNAL INCLUDES
#include "lua-application-interface.h"

namespace
{

void PrintLuaError( lua_State* state )
{
  // Retrieves the error message from the top of the stack.
  const char* message = lua_tostring( state, -1 );

  // Display the error message.
  puts( message );

  // Pops the message from the top of the stack.
  lua_pop( state, 1 );
}

int ConnectKeyEvent( lua_State* state )
{
  // Retrieves the scene-launcher pointer from the top of the stack.
  void* luaApplicationPtr = const_cast<void*>( lua_topointer( state, 1 ) );

  if( nullptr != luaApplicationPtr )
  {
    // Creates the connection with the OnKeyEvent method of the SceneLauncher.
    ( static_cast<SceneLauncher::LuaApplicationInterface*>( luaApplicationPtr ) )->ConnectKeyEvents();
  }

  // Pops the parameter from the top of the stack.
  lua_pop( state, 1 );

  return 0;
}

int QuitApplication( lua_State* state )
{
  // Retrieves the scene-launcher pointer from the top of the stack.
  void* luaApplicationPtr = const_cast<void*>( lua_topointer( state, 1 ) );

  if( nullptr != luaApplicationPtr )
  {
    static_cast<SceneLauncher::LuaApplicationInterface*>( luaApplicationPtr )->QuitApplication();
  }

  // Pops the parameter from the top of the stack.
  lua_pop( state, 1 );

  return 0;
}

} // namespace

namespace SceneLauncher
{

struct Lua::Impl
{

  Impl()
  : mState{ nullptr },
    mNumberOfPushedParameters{ 0 }
  {
    if( nullptr == mState )
    {
      mState = luaL_newstate();

      // Opens all standard Lua libraries into the given state.
      luaL_openlibs( mState );

      // Register the c api exposed to Lua.
      Register();
    }
  }

  ~Impl()
  {
    lua_close( mState );
  }

  void LoadScriptFile( const char* const file )
  {
    // Loads a file as a Lua chunk.
    int result = luaL_dofile( mState, file );

    if ( result != LUA_OK )
    {
      PrintLuaError( mState );
    }
  }

  void FetchFunction( const char* const name )
  {
    // Clears the parameters counter.
    Clear();

    // Pushes onto the stack the name of the function to be executed.
    lua_getglobal( mState, name );
  }

  void PushParameter( void* parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushlightuserdata( mState, parameter );
  }

  void PushParameter( int parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushinteger( mState, parameter );
  }

  void ExecuteFunction( int numberOfResults )
  {
    // Calls the function.
    int result = lua_pcall( mState, mNumberOfPushedParameters, numberOfResults, 0 );

    if ( result != LUA_OK )
    {
      PrintLuaError( mState );
    }
  }

  void RetrieveResult( int resultIndex )
  {
    // TODO
  }

  void Register()
  {
    if( nullptr != mState )
    {
      // Registers the C API to be used in Lua.
      lua_register( mState, "ConnectKeyEvent", ConnectKeyEvent );
      lua_register( mState, "QuitApplication", QuitApplication );
    }
  }

  void Clear()
  {
    mNumberOfPushedParameters = 0;
  }

  lua_State* mState; ///< The lua state.
  int mNumberOfPushedParameters; ///< The number of parameters pushed on the stack before a function is called.
};

Lua::Lua()
: mImpl{ new Impl() }
{
}

Lua::~Lua()
{
  delete mImpl;
}

void Lua::LoadScriptFile( const char* const file )
{
  mImpl->LoadScriptFile( file );
}

void Lua::FetchFunction( const char* const name )
{
  mImpl->FetchFunction( name );
}

void Lua::PushParameter( void* parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::PushParameter( int parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::ExecuteFunction( int numberOfResults )
{
  mImpl->ExecuteFunction( numberOfResults );
}

} // namespace SceneLauncher
