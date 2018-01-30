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

//#define SHADOWMAPS_CAST // On its own it'll just render the scene with the shadow shader, with the main framebuffer
//#define SHADOWMAPS_RECEIVE
//#define SHADOWMAPS_DEBUG  // Enables rendering of the contents of the shadowmap into a quad at the bottom of the screen

#if defined SHADOWMAPS_RECEIVE
// Enable casting, since there's not a whole lot to receive without it.
#define SHADOWMAPS_CAST
#endif

using namespace Dali;
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
class Scene3dLauncher : public ConnectionTracker
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
   * @brief Called when any key event is received
   *
   * Will use this to quit the application if Back or the Escape key is received
   * @param[in] event The key event information
   */
  void OnKeyEvent( const KeyEvent& event );

  /**
   * @brief Initialise model geometry, shader, position and orientation
   */
  void InitPbrActor();

#if defined SHADOWMAPS_RECEIVE
  void InitShadowActor();
#endif

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

  void PlayAnimation( std::vector<Animation> animationList );

  void ApplicationQuit();

private:
#if defined SHADOWMAPS_CAST
  void GatherShadowCasters(Actor actor);
  void UpdateShadowCastersNearFar(CameraActor cam);
#endif

#if defined SHADOWMAPS_RECEIVE
  void GatherShadowReceivers(Actor actor);
  void UpdateShadowReceiversLightSpaceTransform();

  static void CalculateOrthographicProjection(float left, float right, float top, float bottom, float near, float far, Matrix& matrixOut);

  void SetShadowCameraOrthographicProjection(float left, float right,
      float top, float bottom, float near, float far, bool flipY);
  void SetShadowCameraPositionOrientation(Vector3 position, Quaternion orientation);
#endif

#if defined SHADOWMAPS_DEBUG
  static Geometry NewUnitQuadWithWholeTexture(bool flipY);
  static Actor CreateTexturedQuadActor(Shader shader, Texture texture, float size, bool flipY);
#endif

  Application& mApplication;

  SceneLauncher::SceneFileParser mSceneParser;


  TextLabel mErrorMessage;
  Timer mDoubleTapTime;

  Actor m3dRoot;
  Actor mUiRoot;
#if defined SHADOWMAPS_RECEIVE
  Actor mShadowRoot;
#endif

  CameraActor mMainCamera;
  CameraActor mUiCamera;
#if defined SHADOWMAPS_RECEIVE
  CameraActor mShadowCamera;

  Matrix mShadowCameraWorld;
  Matrix mShadowCameraProjection;
#endif

  ModelSkybox mSkybox;

  SceneLauncher::ModelPbr mModel;
#if defined SHADOWMAPS_RECEIVE
  SceneLauncher::ModelPbr mShadowModel;
#endif

  std::vector<std::vector<Animation>> mAnimations;
  std::vector<std::string> mAnimationsName;

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

#if defined SHADOWMAPS_CAST
  Shader mShadowShader;

  struct ShadowCaster
  {
    Actor actor;
    Property::Index uNearFar;
  };

  std::vector<ShadowCaster> mShadowCasters;
#endif

#if defined SHADOWMAPS_RECEIVE
  FrameBuffer mShadowRenderTarget;
  Texture mShadowRenderTargetTexture;

  struct ShadowReceiver
  {
    Actor actor;
    Property::Index uInverseShadowmapSize;
    Property::Index uLightSpaceTransform;
    Property::Index uLightDir;
  };

  std::vector<ShadowReceiver> mShadowReceivers;
#endif
};

#endif // SCENE_LAUNCHER_EXAMPLE_H
