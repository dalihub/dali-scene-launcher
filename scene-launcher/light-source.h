#ifndef SCENE_LAUNCHER_LIGHT_SOURCE_H_
#define SCENE_LAUNCHER_LIGHT_SOURCE_H_

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
#include <dali/public-api/math/matrix.h>
#include <dali/public-api/common/vector-wrapper.h>

namespace SceneLauncher
{

class ModelPbr;

///@brief The abstraction of a directional light source.
class LightSource
{
public:
  LightSource();

  ///@brief Sets the transform of the light source.
  ///@note Currently only the orientation is used.
  void SetTransform(Dali::Matrix xform);

  ///@brief Sets the model whose nodes are needed to be lit. The actors that
  /// have a Renderer are gathered, a uniform of Vector3 type named uLightDirection
  /// is registered on them and they're updated with the transform of the light.
  void SetModel(ModelPbr& model);

private:
  struct Receiver
  {
    Dali::Actor actor;
    Dali::Property::Index uLightDirection;
  };

  Dali::Matrix mLightView;
  std::vector<Receiver> mReceivers;

  void UpdateReceivers();
};

}

#endif /* SCENE_LAUNCHER_LIGHT_SOURCE_H_ */
