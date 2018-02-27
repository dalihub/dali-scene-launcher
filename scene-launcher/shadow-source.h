#ifndef DALI_SCENE_LAUNCHER_LIGHT_SOURCE_H
#define DALI_SCENE_LAUNCHER_LIGHT_SOURCE_H
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

#include <dali/public-api/actors/actor.h>
#include <dali/public-api/actors/camera-actor.h>
#include <dali/public-api/common/stage.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/rendering/frame-buffer.h>
#include <dali/public-api/rendering/renderer.h>
#include <dali/public-api/render-tasks/render-task.h>

#include "model-pbr.h"

namespace SceneLauncher
{

class ShadowSource
{
public:
  ShadowSource(Dali::Stage& stage);
  ~ShadowSource();

  ///@brief Configures the size of the shadow map, which is assumed to be equilateral.
  ///@param size number of texels in U and V.
  void SetRenderTargetSize(unsigned int size);

  ///@brief
  void SetModel(ModelPbr& model);

  ///@brief Sets the world @a position and @a orientation of the light source.
  void SetPositionOrientation(const Dali::Vector3& position, const Dali::Quaternion& orientation);

  ///@brief Sets the orthographic @a size of the light's camera. along with the @a zNear and @a zFar
  /// clipping planes.
  void SetOrthographicSize(float size, float zNear, float zFar);

  Dali::Texture GetRenderTarget() const;

private:
  struct Caster
  {
    Actor actor;
    Property::Index uNearInvFarLessNear;
  };

  struct Receiver
  {
    Actor actor;
    Property::Index uInvShadowmapSize; // Vector2 with 1.0 / shadowmap width and height
    Property::Index uLightSpaceTransform; // Matrix
    Property::Index uLightDir; // Vector3
  };

  Dali::Stage mStage;
  Dali::RenderTask  mTask;
  Dali::CameraActor mCamera;

  Dali::FrameBuffer mFrameBuffer;
  Dali::Texture mRenderTarget;

  Dali::Matrix mLightView;
  Dali::Matrix mLightProjection;

  ModelPbr mShadowModel;

  std::vector<Caster> mCasters;
  std::vector<Receiver> mReceivers;

  void UpdateCasters();
  void UpdateReceivers();
};

}

#endif //DALI_SCENE_LAUNCHER_LIGHT_SOURCE_H
