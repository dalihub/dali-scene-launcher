#ifndef DALI_SCENE_LAUNCHER_LUA_APPLICATION_INTERFACE_H
#define DALI_SCENE_LAUNCHER_LUA_APPLICATION_INTERFACE_H

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
#include <dali/public-api/signals/connection-tracker.h>

namespace SceneLauncher
{

class LuaApplicationInterface : public Dali::ConnectionTracker
{
public:

  virtual ~LuaApplicationInterface() {}

  virtual void ConnectKeyEvents() = 0;
  virtual void QuitApplication() = 0;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_LUA_APPLICATION_INTERFACE_H

