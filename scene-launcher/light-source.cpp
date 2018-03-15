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
#include "light-source.h"
#include "model-pbr.h"
#include "utils.h"

namespace SceneLauncher
{

LightSource::LightSource()
{}

void LightSource::SetTransform(Dali::Matrix xform)
{
  xform.Invert();
  mLightView = xform;

  UpdateReceivers();
}

void LightSource::SetModel(ModelPbr& model)
{
  auto& receivers = mReceivers;
  receivers.clear();

  VisitActor(model.GetActor(), [&receivers](Actor a) {
    if (a.GetRendererCount() > 0)
    {
      receivers.push_back({ a, a.RegisterProperty("uLightDirection", -Vector3::YAXIS) });
    }
  });

  UpdateReceivers();
}

void LightSource::UpdateReceivers()
{
  Vector3 lightDirection = Quaternion(mLightView).Rotate(Vector3::ZAXIS);
  for(auto& r: mReceivers)
  {
    r.actor.SetProperty(r.uLightDirection, lightDirection);
  }
}

}
