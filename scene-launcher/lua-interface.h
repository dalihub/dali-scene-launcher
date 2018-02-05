#ifndef DALI_SCENE_LAUNCHER_LUA_INTERFACE_H
#define DALI_SCENE_LAUNCHER_LUA_INTERFACE_H

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

namespace SceneLauncher
{

class Lua
{
public:
  /**
   * @brief Constructor. Initializes the Lua state.
   */
  Lua();

  /**
   * @brief Destructor. Destroys the Lua state and frees all the allocated resources.
   */
  ~Lua();

  /**
   * @brief Loads a Lua script file.
   *
   * @param[in] file The file with the Lua script.
   */
  void LoadScriptFile( const char* const file );

  /**
   * @brief Pushes onto the stack the name of the function to be executed.
   *
   * @param[in] name The name of the function.
   */
  void FetchFunction( const char* const name );

  /**
   * @brief Pushes a @e lightuser parameter onto the stack.
   *
   * @param[in] parameter The parameter to be pushed onto the stack.
   */
  void PushParameter( void* parameter );

  /**
   * @brief Pushes an @e integer onto the stack.
   *
   * @param[in] parameter The @e integer to be pushed onto the stack.
   */
  void PushParameter( int parameter );

  /**
   * @brief Executes the function previously fetched.
   *
   * @param[in] numberOfResults The number of results the function returns.
   */
  void ExecuteFunction( int numberOfResults );

  // TODO: Add methods to retrieve the results!

private:
  // Declared private and left unimplemented to avoid copies.
  Lua( const Lua& );
  Lua& operator=( const Lua& );

private:
  struct Impl;
  Impl* mImpl;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_LUA_INTERFACE_H
