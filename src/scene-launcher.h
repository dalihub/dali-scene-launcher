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
#include "dali-toolkit/public-api/controls/scrollable/item-view/item-view.h"
#include "dali-toolkit/devel-api/controls/navigation-view/navigation-view.h"

class SceneLauncher : public Dali::ConnectionTracker
{
public:
  SceneLauncher(Dali::Application& app);
  ~SceneLauncher() = default;

private:  // data
  Dali::Application& mApp;

  std::vector<std::string> mSceneNames;

  Dali::Toolkit::NavigationView mNavigationView;

  std::unique_ptr<Dali::Toolkit::ItemFactory> mItemFactory;
  Dali::Toolkit::ItemLayoutPtr mItemLayout;
  Dali::Toolkit::ItemView mItemView;

  Dali::CameraActor mSceneCamera;
  Dali::RenderTask mSceneRender;
  Dali::Actor mScene;

  Dali::Quaternion mCameraOrientationInv;

  Dali::TapGestureDetector mTapDetector;
  Dali::PanGestureDetector  mPanDetector;

private:  // methods
  void OnInit(Dali::Application& app);
  void OnTerminate(Dali::Application& app);

  void OnKey(const Dali::KeyEvent& e);
  void OnPan(Dali::Actor actor, const Dali::PanGesture& pan);
  void OnTap(Dali::Actor actor, const Dali::TapGesture& tap);
};

#endif  //SCENE_LAUNCHER_H_
