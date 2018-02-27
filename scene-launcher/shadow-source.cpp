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
#include "shadow-source.h"
#include "dli-loader.h"
#include "utils.h"

#include <dali/public-api/rendering/shader.h>
#include <dali/public-api/render-tasks/render-task-list.h>

#include <algorithm>

using namespace Dali;

namespace SceneLauncher
{
namespace
{

#define DEFINE_PROPERTY(varName, propName) const std::string varName = #propName ;

// Casters
DEFINE_PROPERTY(NEAR_INV_FAR_LESS_NEAR, uNearInvFarLessNear)

// Receivers
DEFINE_PROPERTY(LIGHTSPACE_XFORM, uLightspaceTransform)
DEFINE_PROPERTY(LIGHT_DIRECTION, uLightDir)
DEFINE_PROPERTY(INV_SHADOWMAP_SIZE, uInvShadowmapSize)

// Shaders
const char* const DEPTH_PREPASS_SHADER_VSH = "#version 300 es\n"
    DALI_COMPOSE_SHADER(

precision mediump float;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjection;
uniform vec2 uNearFar;  // x is distance of near clipping plane; y is 1.0 / (zFar - zNear)

uniform vec2 uTilt;

in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;
in vec3 aTangent;

out highp float vDepth;
out highp float vDepthSqr;

void main()
{
vec4 vPosition = uModelMatrix * vec4(aPosition, 1.0);
vPosition.xz += uTilt * vPosition.y;  // apply parallax

vPosition = uViewMatrix * vPosition;  // transform to view
gl_Position = uProjection * vPosition;

vDepth = (vPosition.z - uNearFar.x) * uNearFar.y;

// Variance Shadow Maps - Write depth square for occlusion probability calculation.
vDepthSqr = vDepth * vDepth;
}
);

const char* const DEPTH_PREPASS_SHADER_FSH = "#version 300 es\n"
    DALI_COMPOSE_SHADER(

precision mediump float;

in float vDepth;
in float vDepthSqr;

out vec4 FragColor; // We need 4 channel and explicit alpha on device.

void main()
{
  // Variance Shadow Maps - prefilter depth [squared] with values from adjacent fragments.
  // More info: http://www.fabiensanglard.net/shadowmappingVSM/
  float dx = dFdx(vDepth);
  float dy = dFdy(vDepth);
  float moment2 = vDepthSqr + 0.25 * (dx * dx + dy * dy);

  FragColor = vec4(vDepth, moment2, .0, 1.0);
}
);

int sLightSources = 0;
Shader sDepthPrepassShader;

void CalculateOrthographicProjection(float left, float right, float top, float bottom, float near, float far, Matrix& matrixOut)
{
  float* data = matrixOut.AsFloat();
  float rightLessLeft = left - right; // NOTE: We've rotated the view 180 degrees in Y, so we need to flip these.
  float topLessBottom = top - bottom;
  float farLessNear = far - near;
  data[0] = 2.0f / rightLessLeft;
  data[1] = .0f;
  data[2] = .0f;
  data[3] = .0f;
  data[4] = .0f;
  data[5] = 2.0f / topLessBottom;
  data[6] = .0f;
  data[7] = .0f;
  data[8] = .0f;
  data[9] = .0f;
  data[10] = 2.0f / farLessNear;
  data[11] = .0f;
  data[12] = (right + left) / rightLessLeft;
  data[13] = (top + bottom) / topLessBottom;
  data[14] = (far + near) / -farLessNear;
  data[15] = 1;
}

TextureSet CloneTextureSet(TextureSet ts)
{
  auto tsNew = TextureSet::New();
  auto numTextures = ts.GetTextureCount();
  unsigned int i = 0;
  while(numTextures > 0)
  {
    Texture t = ts.GetTexture(i);
    tsNew.SetTexture(i, t);
    tsNew.SetSampler(i, ts.GetSampler(i));

    if (t)
    {
      --numTextures;
    }
    ++i;
  }
  return tsNew;
}

} // namespace

ShadowSource::ShadowSource(Stage& stage)
: mStage(stage),
  mTask(stage.GetRenderTaskList().CreateTask()),
  mCamera(CameraActor::New())
{
  if(sLightSources == 0)
  {
    sDepthPrepassShader = Shader::New(DEPTH_PREPASS_SHADER_VSH, DEPTH_PREPASS_SHADER_FSH);
  }
  ++sLightSources;

  mCamera.SetAnchorPoint(AnchorPoint::CENTER);
  mCamera.SetParentOrigin(ParentOrigin::CENTER);
  mCamera.SetInvertYAxis(true);

  mTask.SetCameraActor(mCamera);
  mTask.SetInputEnabled(false);
  mTask.SetExclusive(true);
  mTask.SetClearEnabled(true);
  mTask.SetClearColor(Vector4::ONE);  // clear to normalized zFar.
}

ShadowSource::~ShadowSource()
{
  if(--sLightSources == 0)
  {
    sDepthPrepassShader = Shader();
  }

  mStage.GetRenderTaskList().RemoveTask(mTask);
}

void ShadowSource::SetRenderTargetSize(unsigned int size)
{
  mRenderTarget = Texture::New(TextureType::TEXTURE_2D, Pixel::Format::RGB888, size, size);

  mFrameBuffer = FrameBuffer::New(size, size, FrameBuffer::Attachment::DEPTH);
  mFrameBuffer.AttachColorTexture(mRenderTarget);

  mTask.SetFrameBuffer(mFrameBuffer);
  mTask.SetViewportSize(Vector2(size, size));
}

void ShadowSource::SetModel(ModelPbr& model)
{
  auto root = model.GetActor();
  if (!root)
  {
    return; // Nothing to see here.
  }

  mCasters.clear();
  mReceivers.clear();

  model.Duplicate(mShadowModel, CloneOptions::OMIT_TEXTURES | CloneOptions::CONSTRAIN_XFORM);

  // Attempt to create Casters.
  if(auto shadowRoot = mShadowModel.GetActor())
  {
    auto& casters = mCasters;
    VisitActor(shadowRoot, [&casters](Actor a) {
      auto numRenderers = a.GetRendererCount();
      if (numRenderers > 0)
      {
        // Check role of node.
        Property::Index iShadowMode = a.GetPropertyIndex(DliLoader::SHADOW_MODE_PROPERTY);
        auto shadowMode = a.GetProperty<int>(iShadowMode);
        if (MaskMatch(shadowMode, static_cast<int>(DliLoader::ShadowMode::CAST_ONLY)))
        {
          // Replace Shader and set face culling mode on each of the Renderers.
          for(decltype(numRenderers) i = 0; i < numRenderers; ++i)
          {
            Renderer r = a.GetRendererAt(i);
            r.SetShader(sDepthPrepassShader);
            r.SetProperty(Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::FRONT);
          }

          // Register property and create Caster.
          Caster c = { a, a.RegisterProperty(NEAR_INV_FAR_LESS_NEAR, Vector2(.0f, 1.0f)) };
          casters.push_back(c);
        }
        else  // Not a caster - remove Renderers and ignore actor.
        {
          while (numRenderers > 0)
          {
            a.RemoveRenderer(0);
            --numRenderers;
          }
        }
      }
    });
  }

  // Attempt to create Receivers.
  // Register properties; collect TextureSets and their users.
  auto& receivers = mReceivers;

  // We're creating a mapping from each TextureSet used in the scene, to the Renderers of shadow receivers,
  // that are using it, and record whether they are shared with any non-receivers -- in which case we'll
  // have to clone the TextureSet for those.
  struct TextureSetRecord
  {
    TextureSet ts;
    bool hasNonReceivers;
    std::vector<Renderer> receivers;
  };

  std::vector<TextureSetRecord> textureSets;
  textureSets.reserve(16);

  VisitActor(root, [&receivers, &textureSets](Actor a) {
    auto numRenderers = a.GetRendererCount();
    if (numRenderers > 0)
    {
      // Check role of node.
      Property::Index iShadowMode = a.GetPropertyIndex(DliLoader::SHADOW_MODE_PROPERTY);
      auto shadowMode = a.GetProperty<int>(iShadowMode);
      const bool isReceiver = MaskMatch(shadowMode, static_cast<int>(DliLoader::ShadowMode::RECEIVE_ONLY));

      // Gather TextureSets from Renderes.
      for (unsigned int i = 0; i < numRenderers; ++i)
      {
        Renderer r = a.GetRendererAt(i);
        TextureSet ts = r.GetTextures();

        auto iFind = std::lower_bound(textureSets.begin(), textureSets.end(), ts,
            [](const TextureSetRecord& tsr, const TextureSet& ts) {
            return tsr.ts < ts;
          });
        if (iFind == textureSets.end() || iFind->ts != ts)  // not found -- insert.
        {
          TextureSetRecord tsr;
          if (isReceiver)
          {
            tsr = TextureSetRecord{ ts, false, { r } };
          }
          else
          {
            tsr = TextureSetRecord{ ts, true };
          }
          textureSets.insert(iFind, tsr);
        }
        else  // update
        {
          if (isReceiver)
          {
            iFind->receivers.push_back(r);
          }
          else if(!iFind->hasNonReceivers)
          {
            iFind->hasNonReceivers = true;
          }
        }
      }

      if (isReceiver) // Register properties.
      {
        Receiver r = { a,
            a.RegisterProperty(LIGHTSPACE_XFORM, Matrix::IDENTITY),
            a.RegisterProperty(LIGHT_DIRECTION, -Vector3::YAXIS),
            a.RegisterProperty(INV_SHADOWMAP_SIZE, Vector2::ONE),
        };
        receivers.push_back(r);
      }
    }
  });

  // Now perform the actual attaching of the shadow map.
  for (auto& tsr: textureSets)
  {
    if (!tsr.receivers.empty()) // Need to attach shadow map.
    {
      TextureSet ts = tsr.ts;
      if (tsr.hasNonReceivers)  // Need to copy TextureSet and update Renderers
      {
        ts = CloneTextureSet(ts);
        for (auto& r: tsr.receivers)
        {
          r.SetTextures(ts);
        }
      }

      // Attach shadow map. NOTE: default sampler will do.
      auto iTexture = ts.GetTextureCount();
      ts.SetTexture(iTexture, mRenderTarget);
    }
  }
}

void ShadowSource::SetPositionOrientation(const Dali::Vector3& position, const Dali::Quaternion& orientation)
{
  mCamera.SetPosition(position);
  mCamera.SetOrientation(orientation);

  mLightView.SetTransformComponents(Vector3::ONE, orientation, position);
  mLightView.Invert();

  UpdateReceivers();
}

void ShadowSource::SetOrthographicSize(float size, float zNear, float zFar)
{
  size *= .5f;
  mCamera.SetOrthographicProjection(-size, size, size, -size, zNear, zFar);

  // NOTE: y is flipped.
  CalculateOrthographicProjection(-size, size, -size, size, zNear, zFar, mLightProjection);

  UpdateCasters();
  UpdateReceivers();
}

void ShadowSource::UpdateCasters()
{
  if (auto shadowRoot = mShadowModel.GetActor())
  {
    Vector2 nearInvFarLessNear { mCamera.GetNearClippingPlane(),
      1.0f / (mCamera.GetFarClippingPlane() - mCamera.GetNearClippingPlane()) };
    for (auto& ic: mCasters)
    {
      ic.actor.SetProperty(ic.uNearInvFarLessNear, nearInvFarLessNear);
    }
  }
}

void ShadowSource::UpdateReceivers()
{
  Matrix lightSpaceTransform;
  Matrix::Multiply(lightSpaceTransform, mLightView, mLightProjection);

  Vector3 lightDir = Quaternion(mLightView).Rotate(Vector3::ZAXIS);

  Vector2 invShadowMapSize = Vector2(1.0f / mRenderTarget.GetWidth(), 1.0f / mRenderTarget.GetHeight());

  for (auto& ir: mReceivers)
  {
    ir.actor.SetProperty(ir.uInvShadowmapSize, invShadowMapSize);
    ir.actor.SetProperty(ir.uLightSpaceTransform, lightSpaceTransform);
    ir.actor.SetProperty(ir.uLightDir, lightDir);
  }
}

}
