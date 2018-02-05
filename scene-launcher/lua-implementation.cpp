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

// lua 5.1 doesn't define LUA_OK
#if !defined(LUA_OK )
#define LUA_OK 0
#endif

// INTERNAL INCLUDES
#include "lua-application-bindings.h"

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

  void PushParameter( bool parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushboolean( mState, parameter );
  }

  void PushParameter( int parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushinteger( mState, parameter );
  }

  void PushParameter( float parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushnumber( mState, parameter );
  }

  void PushParameter( const char* const parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushstring( mState, parameter );
  }

  void PushParameter( const std::string& parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushstring( mState, parameter.c_str() );
  }

  void PushParameter( void* parameter )
  {
    // Increases the parameter counter.
    ++mNumberOfPushedParameters;

    // Pushes the parameter onto the stack.
    lua_pushlightuserdata( mState, parameter );
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

  void RetrieveResult( int resultIndex, bool& result )
  {
    result = lua_toboolean( mState, resultIndex );
  }

  void RetrieveResult( int resultIndex, int& result )
  {
    result = lua_tointeger( mState, resultIndex );
  }

  void RetrieveResult( int resultIndex, float& result )
  {
    result = lua_tonumber( mState, resultIndex );
  }

  void RetrieveResult( int resultIndex, std::string& result )
  {
    result = lua_tostring( mState, resultIndex );
  }

  void RetrieveResult( int resultIndex, void*& result )
  {
    result = lua_touserdata( mState, resultIndex );
  }

  void Register()
  {
    if( nullptr != mState )
    {
      // Registers the C API to be used in Lua.
      for( unsigned int index = 0u; index < NUMBER_OF_LUA_BINDING_FUNCTIONS; ++index )
      {
        const LuaBindingFuction& luaFunction = LUA_BINDING_FUNCTIONS[index];
        lua_register( mState, luaFunction.name, luaFunction.function );
      }
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

void Lua::PushParameter( bool parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::PushParameter( int parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::PushParameter( float parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::PushParameter( const char* const parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::PushParameter( const std::string& parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::PushParameter( void* parameter )
{
  mImpl->PushParameter( parameter );
}

void Lua::ExecuteFunction( int numberOfResults )
{
  mImpl->ExecuteFunction( numberOfResults );
}

void Lua::RetrieveResult( int resultIndex, bool& result )
{
  mImpl->RetrieveResult( resultIndex, result );
}

void Lua::RetrieveResult( int resultIndex, int& result )
{
  mImpl->RetrieveResult( resultIndex, result );
}

void Lua::RetrieveResult( int resultIndex, float& result )
{
  mImpl->RetrieveResult( resultIndex, result );
}

void Lua::RetrieveResult( int resultIndex, std::string& result )
{
  mImpl->RetrieveResult( resultIndex, result );
}

void Lua::RetrieveResult( int resultIndex, void*& result )
{
  mImpl->RetrieveResult( resultIndex, result );
}

} // namespace SceneLauncher
