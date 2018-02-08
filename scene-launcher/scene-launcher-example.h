#ifndef SCENE_LAUNCHER_EXAMPLE_H
#define SCENE_LAUNCHER_EXAMPLE_H

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
#include <dali-toolkit/dali-toolkit.h>

// INTERNAL INCLUDES
#include "model-pbr.h"
#include "scene-file-parser.h"
#include "model-skybox.h"
#include "lua-application-helper.h"
#include "lua-interface.h"
#include "data-provider.h"

using namespace Toolkit;

/**
 *
 * This example shows a Physically Based Rendering illumination model.
 *
 * - Double-tap to toggle between 3D models.
 * - Pan up/down on left side of screen to zoom out/in
 * - Pan anywhere else to rotate scene
 *
 */
class Scene3dLauncher : public Dali::ConnectionTracker, public SceneLauncher::DataProvider::Observer
{
public:

  Scene3dLauncher( Application& application );
  ~Scene3dLauncher();

  /**
   * @brief The Init signal is received once (only) during the Application lifetime
   */
  void Create( Application& application );

  /**
   * @brief Function triggered by timer, to calculate the double tap event
   */
  bool OnDoubleTapTime();

  /**
   * @brief This function will zoom of model, and orientation or model or environment according to mode.
   *
   * The mode can be changed double tap the screen bottom, if double tap
   * in other place, will change model.
   */
  bool OnTouch( Actor actor, const TouchData& touch );

  /**
   * @brief Initialise model geometry, shader, position and orientation
   */
  void InitPbrActor();

  /**
   * @brief Creates scene actors and setup camera parameters
   */
  void InitActors();

  /**
   * @brief Clear resources
   */
  void ClearModel();

  /**
   * @brief loads model and initialise textures.
   */
  void CreateModel();

  void DisplayError( const std::string& errorMessage );

  void PlayAnimation( std::vector<Animation>& animationList );

private: // Inherited from SceneLauncher::DataProvider::Observer

  void OnNotification( const SceneLauncher::DataProvider::Notification& notification );

private:
  Application& mApplication;

  SceneLauncher::FileParser mSceneFileParser;
  SceneLauncher::Lua mLua;
  SceneLauncher::LuaApplicationHelper mLuaApplicationHelper;
  SceneLauncher::DataProvider mDataProvider;

  TextLabel mErrorMessage;
  Timer mDoubleTapTime;

  Actor m3dRoot;
  Actor mUiRoot;

  ModelSkybox mSkybox;
  SceneLauncher::ModelPbr mModel;
  std::vector<std::vector<Animation>> mAnimations;
  std::vector<std::string> mAnimationsName;

  std::vector<SceneLauncher::DliLoader::Script> mScripts;

  Vector3 mCameraPosition;
  Vector2 mPointZ;
  Vector2 mStartTouch;

  //Quaternion of ViewMatrix
  Quaternion mCameraOrientationInv;
  Quaternion mModelOrientation;
  Quaternion mCubeOrientation;

  float mZoomLevel;
  bool mDoubleTap;
  bool mRotateEnvironment;
};

#endif // SCENE_LAUNCHER_EXAMPLE_H
