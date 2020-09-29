#ifndef SCENE_LAUNCHER_H_
#define SCENE_LAUNCHER_H_
/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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
#include "libdli/animation-definition.h"
#include "libdli/scene-definition.h"
#include "libdli/node-definition.h"
#include "libdli/camera-parameters.h"
#include "dali/public-api/common/vector-wrapper.h"
#include "dali/public-api/events/pan-gesture-detector.h"
#include "dali/public-api/actors/camera-actor.h"
#include "dali/public-api/render-tasks/render-task.h"
#include "dali/public-api/signals/connection-tracker.h"
#include "dali/public-api/adaptor-framework/application.h"
#include "dali-toolkit/public-api/controls/text-controls/text-label.h"

class SceneLauncher : public Dali::ConnectionTracker
{
public:
  SceneLauncher(Dali::Application& app, const char* sceneName);
  ~SceneLauncher() = default;

private:  // data
  Dali::Application& mApp;
  const char* mSceneName;

  std::vector<dli::AnimationDefinition> mAnimations;

  Dali::CameraActor mCamera;
  Dali::Actor mRoot;
  Dali::RenderTask mSceneRender;

  Dali::Toolkit::TextLabel mErrorMessage;

  Dali::Quaternion mCameraOrientationInv;

  Dali::PanGestureDetector  mPanDetector;

private:  // methods
  void OnInit(Dali::Application& app);

  void OnTerminate(Dali::Application& app);

  void OnKey(const Dali::KeyEvent& e);

  void OnPan(Dali::Actor actor, const Dali::PanGesture& pan);

  void TryLoadScene(std::string sceneName);

  void CleanScene();

  void CreateErrorMessage(std::string msg);

  void LoadScene(std::string sceneName);

  void ConfigureCamera(const dli::CameraParameters& params);

  void ConfigureBlendShapeShaders(dli::ResourceBundle& resources,
    const dli::SceneDefinition& scene, Dali::Actor root,
    std::vector<dli::BlendshapeShaderConfigurationRequest>&& requests);
};

#endif  //SCENE_LAUNCHER_H_
