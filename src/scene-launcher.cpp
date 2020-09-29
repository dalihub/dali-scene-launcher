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
#include "scene-launcher.h"
#include "libdli/light-parameters.h"
#include "libdli/dli-loader.h"
#include "libdli/load-result.h"
#include "dali/public-api/adaptor-framework/key.h"
#include "dali/public-api/events/key-event.h"
#include "dali/public-api/actors/layer.h"
#include "dali/public-api/render-tasks/render-task-list.h"

using namespace Dali;
using namespace Dali::Toolkit;
using namespace dli;

namespace
{
  const float ROTATION_SCALE = 0.05f;
  const Vector3 CAMERA_DEFAULT_POSITION(0.0f, 0.0f, 3.5f);

  const std::string RESOURCE_PATHS[]{
    "environments/",
    "shaders/",
    "models/",
    "images/",
  };
}

SceneLauncher::SceneLauncher(Dali::Application& app, const char* sceneName)
: mApp(app),
  mSceneName(sceneName)
{
  if (!std::getenv("DALI_APPLICATION_PACKAGE"))
  {
    if (auto desktopPrefix = std::getenv("DESKTOP_PREFIX"))
    {
      std::stringstream sstr;
      sstr << desktopPrefix << "/share/com.samsung.dali-demo/res/";

      auto daliApplicationPackage = sstr.str();
      setenv("DALI_APPLICATION_PACKAGE", daliApplicationPackage.c_str(), 0);
    }
  }

  app.InitSignal().Connect(this, &SceneLauncher::OnInit);
  app.TerminateSignal().Connect(this, &SceneLauncher::OnTerminate);
}

void SceneLauncher::OnInit(Application& app)
{
  auto window = app.GetWindow();

  auto camera = CameraActor::New();
  window.Add(camera);
  mCamera = camera;

  auto root = Actor::New();
  SetActorCentered(root);
  window.Add(root);
  mRoot = root;

  auto renderTasks = window.GetRenderTaskList();
  auto sceneRender = renderTasks.CreateTask();
  sceneRender.SetCameraActor(camera);
  sceneRender.SetSourceActor(root);
  sceneRender.SetExclusive(true);
  mSceneRender = sceneRender;

  TryLoadScene(mSceneName);

  window.KeyEventSignal().Connect(this, &SceneLauncher::OnKey);

  mPanDetector = PanGestureDetector::New();
  mPanDetector.DetectedSignal().Connect(this, &SceneLauncher::OnPan);
  mPanDetector.Attach(window.GetRootLayer());
}

void SceneLauncher::OnTerminate(Application& app)
{
  mPanDetector.Reset();

  auto window = app.GetWindow();
  auto renderTasks = window.GetRenderTaskList();
  renderTasks.RemoveTask(mSceneRender);
  mSceneRender.Reset();

  CleanScene();
  UnparentAndReset(mCamera);
  UnparentAndReset(mRoot);
}

void SceneLauncher::OnKey(const KeyEvent& e)
{
  if (e.GetState() == KeyEvent::UP)
  {
    if (IsKey(e, DALI_KEY_ESCAPE) || IsKey(e, DALI_KEY_BACK))
    {
      mApp.Quit();
    }
    else switch (e.GetKeyCode())
    {
    case 116: // F5
    {
      TryLoadScene(mSceneName);
      break;
    }

    default:
      break;
    }
  }
}

void SceneLauncher::OnPan(Actor actor, const PanGesture& pan)
{
  auto windowSize = mApp.GetWindow().GetSize();
  Vector2 size{ float(windowSize.GetWidth()), float(windowSize.GetHeight()) };
  float aspect = size.y / size.x;

  size *= ROTATION_SCALE;

  Vector2 rotation{ pan.GetDisplacement().x / size.x, pan.GetDisplacement().y / size.y * aspect };

  Quaternion q = Quaternion(Radian(rotation.y), Radian(rotation.x), Radian(0.f));
  Quaternion q0 = mRoot.GetProperty(Actor::Property::ORIENTATION).Get<Quaternion>();

  mRoot.SetProperty(Actor::Property::ORIENTATION, q * q0);
}

void SceneLauncher::TryLoadScene(std::string sceneName)
{
  CleanScene();
  try
  {
    LoadScene(mSceneName);
  }
  catch (const DaliException& e)
  {
    CreateErrorMessage(e.condition);
  }
}

void SceneLauncher::CleanScene()
{
  UnparentAndReset(mErrorMessage);

  uint32_t i = mRoot.GetChildCount();
  while (i-- > 0)
  {
    Actor actor = mRoot.GetChildAt(i);
    UnparentAndReset(actor);
  }
}

void SceneLauncher::CreateErrorMessage(std::string msg)
{
  auto window = mApp.GetWindow();
  auto windowSize = window.GetSize();

  TextLabel label = TextLabel::New();
  label.SetProperty(TextLabel::Property::TEXT_COLOR, Color::WHITE);
  label.SetProperty(TextLabel::Property::MULTI_LINE, true);
  label.SetProperty(TextLabel::Property::HORIZONTAL_ALIGNMENT, HorizontalAlignment::LEFT);
  label.SetProperty(TextLabel::Property::VERTICAL_ALIGNMENT, VerticalAlignment::TOP);
  label.SetProperty(TextLabel::Property::TEXT, msg);
  label.SetProperty(Actor::Property::SIZE, Vector3(windowSize.GetWidth(), windowSize.GetHeight(), 1.f));
  SetActorCentered(label);
  window.Add(label);
}

void SceneLauncher::LoadScene(std::string sceneName)
{
  std::string resPath = mApp.GetResourcePath();
  ResourceBundle::PathProvider pathProvider = [resPath](ResourceType::Value type) {
    return resPath + RESOURCE_PATHS[type];
  };

  ResourceBundle resources;
  SceneDefinition scene;
  std::vector<AnimationGroupDefinition> animGroups;
  std::vector<CameraParameters> cameras;
  std::vector<LightParameters> lights;

  DliLoader loader;
  DliLoader::InputParams input{
    pathProvider(ResourceType::Mesh),
    nullptr,
    nullptr,
    nullptr,
  {},
  {},
  nullptr,
  };
  LoadResult output{
    resources,
    scene,
    mAnimations,
    animGroups,
    cameras,
    lights
  };
  auto path = pathProvider(ResourceType::Mesh) + sceneName + ".dli";
  DliLoader::LoadParams loadParams{ input, output };
  if (!loader.LoadScene(path, loadParams))
  {
    ExceptionFlinger(ASSERT_LOCATION) << "Failed to load scene from '" << path << "': " << loader.GetParseError();
  }

  if (cameras.empty())
  {
    cameras.push_back(CameraParameters());
    cameras[0].matrix.SetTranslation(CAMERA_DEFAULT_POSITION);
  }
  ConfigureCamera(cameras[0]);

  ViewProjection viewProjection = cameras[0].GetViewProjection();
  Transforms xforms{
    MatrixStack{},
    viewProjection
  };
  NodeDefinition::CreateParams nodeParams{
    resources,
    xforms,
  };
  Customization::Choices choices;
  for (auto iRoot : scene.GetRoots())
  {
    auto resourceRefs = resources.CreateRefCounter();
    scene.CountResourceRefs(iRoot, choices, resourceRefs);
    resources.CountEnvironmentReferences(resourceRefs);

    resources.LoadResources(resourceRefs, pathProvider);

    if (auto actor = scene.CreateNodes(iRoot, choices, nodeParams))
    {
      scene.ConfigureSkeletonJoints(iRoot, resources.mSkeletons, actor);
      scene.ConfigureSkinningShaders(resources, actor, std::move(nodeParams.mSkinnables));
      ConfigureBlendShapeShaders(resources, scene, actor, std::move(nodeParams.mBlendshapeRequests));

      scene.ApplyConstraints(actor, std::move(nodeParams.mConstrainables));

      mRoot.Add(actor);
    }
  }

  if (!mAnimations.empty())
  {
    Actor root = mRoot;
    auto getActor = [&root](const std::string& name) {
      return root.FindChildByName(name);
    };

    mAnimations[0].ReAnimate(getActor).Play();
  }
}

void SceneLauncher::ConfigureCamera(const CameraParameters& params)
{
  if (params.isPerspective)
  {
    mCamera.SetProjectionMode(Camera::PERSPECTIVE_PROJECTION);
    mCamera.SetNearClippingPlane(params.zNear);
    mCamera.SetFarClippingPlane(params.zFar);
    mCamera.SetFieldOfView(Radian(Degree(params.yFov)));
  }
  else
  {
    mCamera.SetProjectionMode(Camera::ORTHOGRAPHIC_PROJECTION);
    mCamera.SetOrthographicProjection(params.orthographicSize.x,
      params.orthographicSize.y,
      params.orthographicSize.z,
      params.orthographicSize.w,
      params.zNear,
      params.zFar);
  }

  // model
  Vector3 camTranslation;
  Vector3 camScale;
  Quaternion camOrientation;
  params.CalculateTransformComponents(camTranslation, camOrientation, camScale);

  SetActorCentered(mCamera);
  mCamera.SetInvertYAxis(true);
  mCamera.SetProperty(Actor::Property::POSITION, camTranslation);
  mCamera.SetProperty(Actor::Property::ORIENTATION, camOrientation);
  mCamera.SetProperty(Actor::Property::SCALE, camScale);

  camOrientation.Conjugate();
  mCameraOrientationInv = camOrientation;
}

void SceneLauncher::ConfigureBlendShapeShaders(ResourceBundle& resources, const dli::SceneDefinition& scene, Actor root,
  std::vector<BlendshapeShaderConfigurationRequest>&& requests)
{
  std::vector<std::string> errors;
  auto onError = [&errors](const std::string& msg) {
    errors.push_back(msg);
  };
  if (!scene.ConfigureBlendshapeShaders(resources, root, std::move(requests), onError))
  {
    ExceptionFlinger flinger(ASSERT_LOCATION);
    for (auto& msg : errors)
    {
      flinger << msg << '\n';
    }
  }
}
