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

// CLASS HEADER
#include "scene-launcher-example.h"

// INTERNAL INCLUDES
#include "utils.h"

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

namespace
{

const char* MODEL_DIR_URL = SCENE_LAUNCHER_MODEL_DIR "scenes";

const std::string ASSET_MODEL_DIR = SCENE_LAUNCHER_MODEL_DIR;

const Vector3 CAMERA_DEFAULT_POSITION( 0.0f, 0.0f, 3.5f );

const float TEXT_AUTO_SCROLL_SPEED = 200.f;

#ifdef SHADOWMAPS_RECEIVE
const unsigned int SHADOWMAP_SIZE = 512;
#endif

#if defined SHADOWMAPS_DEBUG && defined SHADOWMAPS_RECEIVE
/*
 * Vertex shader - flat
 */
const char* VERTEX_SHADER_FLAT = DALI_COMPOSE_SHADER(
attribute mediump vec3 aPosition;\n // DALi shader builtin
attribute mediump vec2 aUv0;\n // DALi shader builtin

uniform   mediump mat4 uMvpMatrix;\n  // DALI shader builtin
uniform   mediump vec3 uSize;\n // DALi shader builtin

varying   mediump vec2 vUv0;\n

void main()
{
  mediump vec4 vertexPosition = vec4(aPosition, 1.0);
  vertexPosition.xyz *= uSize;

  gl_Position = uMvpMatrix * vertexPosition;
  vUv0 = aUv0;
});

/*
 * Fragment shader - flat
 */
const char* FRAGMENT_SHADER_FLAT = DALI_COMPOSE_SHADER(
uniform sampler2D uTexture0;\n

varying mediump vec2 vUv0;\n

void main()
{
  gl_FragColor = texture2D( uTexture0, vUv0 );
});
#endif

} // namespace

Scene3dLauncher::Scene3dLauncher( Application& application )
: mApplication( application ),
  mSceneParser(),
  mDoubleTapTime(),
  m3dRoot(),
  mUiRoot(),
  mAnimations(),
  mAnimationsName(),
  mCameraPosition( CAMERA_DEFAULT_POSITION ),
  mCameraOrientationInv(),
  mModelOrientation(),
  mCubeOrientation(),
  mZoomLevel( 1.f ),
  mDoubleTap( false ),
  mRotateEnvironment( false )
{
  mAnimationsName.push_back("loaded");

  // Connect to the Application's Init signal
  mApplication.InitSignal().Connect( this, &Scene3dLauncher::Create );
}

Scene3dLauncher::~Scene3dLauncher()
{
  // Nothing to do here;
}

void Scene3dLauncher::Create( Application& application )
{
  // Disable indicator
  Dali::Window winHandle = application.GetWindow();
  winHandle.ShowIndicator( Dali::Window::INVISIBLE );

  // Get a handle to the stage
  Stage stage = Stage::GetCurrent();
  stage.SetBackgroundColor( Color::BLUE );

  // Creating root and camera actor for rendertask for 3D Scene rendering
  mUiRoot = Actor::New();
  mUiRoot.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  mUiRoot.SetParentOrigin(ParentOrigin::TOP_LEFT);
  mUiRoot.SetSize(stage.GetSize());

  m3dRoot = Actor::New();
  m3dRoot.SetAnchorPoint(AnchorPoint::CENTER);
  m3dRoot.SetParentOrigin(ParentOrigin::CENTER);

#if defined SHADOWMAPS_RECEIVE
  mShadowRoot = Actor::New();
  mShadowRoot.SetName("Shadow Root");
  mShadowRoot.SetAnchorPoint(AnchorPoint::CENTER);
  mShadowRoot.SetParentOrigin(ParentOrigin::CENTER);
#endif

  mMainCamera = CameraActor::New( stage.GetSize() );
  mMainCamera.SetAnchorPoint(AnchorPoint::CENTER);
  mMainCamera.SetParentOrigin(ParentOrigin::CENTER);

  RenderTaskList renderTasks = stage.GetRenderTaskList();
  RenderTask mainPass = renderTasks.CreateTask();
  mainPass.SetCameraActor( mMainCamera );
  mainPass.SetSourceActor( m3dRoot );
  mainPass.SetExclusive( true );

#if defined SHADOWMAPS_RECEIVE
  mShadowRenderTargetTexture = Texture::New(TextureType::TEXTURE_2D, Pixel::Format::RGB888,
      SHADOWMAP_SIZE, SHADOWMAP_SIZE);  // NOTE: no depth format with DALi.
  mShadowRenderTarget = FrameBuffer::New(SHADOWMAP_SIZE, SHADOWMAP_SIZE,
      FrameBuffer::Attachment::DEPTH);  // NOTE: we'll still have to specify a depth attachment for depth testing.
  mShadowRenderTarget.AttachColorTexture(mShadowRenderTargetTexture);

  mShadowCamera = CameraActor::New();
  mShadowCamera.SetName("Shadow Camera");
  mShadowCamera.SetAnchorPoint(AnchorPoint::CENTER);
  mShadowCamera.SetParentOrigin(ParentOrigin::CENTER);

  RenderTask shadowPass = stage.GetRenderTaskList().CreateTask();
  shadowPass.SetCameraActor( mShadowCamera );
  shadowPass.SetSourceActor( mShadowRoot );
  shadowPass.SetInputEnabled(false);
  shadowPass.SetExclusive(true);

  shadowPass.SetFrameBuffer(mShadowRenderTarget);
  shadowPass.SetViewportSize(Vector2(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
  shadowPass.SetClearEnabled(true);
  shadowPass.SetClearColor(Vector4::ONE); // NOTE: clear to normalised far.
#endif

  stage.Add( mMainCamera );
#if defined SHADOWMAPS_RECEIVE
  stage.Add( mShadowCamera );
#endif

  stage.Add( mUiRoot );
  stage.Add( m3dRoot );
#if defined SHADOWMAPS_RECEIVE
  stage.Add( mShadowRoot );

#if defined SHADOWMAPS_DEBUG
  // debug quad
  Shader plainOldShader = Shader::New(VERTEX_SHADER_FLAT, FRAGMENT_SHADER_FLAT);
  Actor shadowDebugActor = CreateTexturedQuadActor(plainOldShader, mShadowRenderTargetTexture, 100.0f, false);
  shadowDebugActor.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER);
  shadowDebugActor.SetParentOrigin( ParentOrigin::BOTTOM_CENTER);
  mUiRoot.Add( shadowDebugActor);
#endif  // SHADOWMAPS_DEBUG
#endif

  try
  {
#if defined SHADOWMAPS_CAST
    mShadowShader = CreateShader(SCENE_LAUNCHER_SHADER_DIR "/scenes/shadowmap.vsh",
      SCENE_LAUNCHER_SHADER_DIR "/scenes/shadowmap.fsh");
#endif

    // Read models from the filesystem
    mSceneParser.ReadPbrModelFolder( MODEL_DIR_URL );

    CreateModel();
  }
  catch( DaliException& e )
  {
    std::stringstream stream;
    stream << "Error while loading " << mSceneParser.GetCurrentModelFile() << ". Error : " << std::string( e.condition );

    DisplayError( stream.str() );
  }
  catch( ... )
  {
    DALI_LOG_ERROR( "Unknown error while loading %s\n", mSceneParser.GetCurrentModelFile().c_str() );
  }


  // Respond to a click anywhere on the stage
  mUiRoot.TouchSignal().Connect( this, &Scene3dLauncher::OnTouch );
  // Respond to key events
  stage.KeyEventSignal().Connect( this, &Scene3dLauncher::OnKeyEvent );

  mDoubleTapTime = Timer::New(150);
  mDoubleTapTime.TickSignal().Connect( this, &Scene3dLauncher::OnDoubleTapTime );
}

bool Scene3dLauncher::OnDoubleTapTime()
{
  mDoubleTap = false;
  return true;
}

bool Scene3dLauncher::OnTouch( Actor actor, const TouchData& touch )
{
  const PointState::Type state = touch.GetState( 0 );

  switch( state )
  {
    case PointState::DOWN:
    {
      if(mDoubleTap)
      {
        const Size& size = Stage::GetCurrent().GetSize();
        const float scaleY = size.height;
        if( mStartTouch.y > ( scaleY * 0.7f ) )
        {
          mRotateEnvironment = !mRotateEnvironment;
        }
        else
        {
          ClearModel();

          try
          {
            mSceneParser.LoadNextModel();

            CreateModel();
          }
          catch( DaliException e )
          {
            std::stringstream stream;
            stream << "Error while loading " << mSceneParser.GetCurrentModelFile() << ". Error : " << std::string( e.condition );

            DisplayError( stream.str() );
          }
          catch( ... )
          {
            DALI_LOG_ERROR( "Unknown error while loading %s\n", mSceneParser.GetCurrentModelFile().c_str() );
          }
        }
      }
      mDoubleTapTime.Stop();
      mStartTouch = touch.GetScreenPosition(0);
      mPointZ = mStartTouch;
      break;
    }
    case PointState::MOTION:
    {
      const Stage stage = Stage::GetCurrent();
      const Size& size = stage.GetSize();
      const float scaleX = size.width;
      const float scaleY = size.height;
      const Vector2& point = touch.GetScreenPosition(0);
      if( ( mStartTouch.x < ( scaleX * 0.3f ) ) && ( point.x < ( scaleX * 0.3f ) ) )
      {
        mZoomLevel += ( mStartTouch.y - point.y ) / ( scaleY * 0.9f );

        //Clamp mZoomLevel to 0.0 to 1.0
        mZoomLevel = std::max( 0.f, std::min( 1.f, mZoomLevel ) );

        CameraActor cam = stage.GetRenderTaskList().GetTask(0).GetCameraActor();
        if( cam )
        {
          cam.SetPosition( mCameraPosition * ( mZoomLevel + 0.1f ) );
        }

        mStartTouch = point;
      }
      //If the touch is not processed above, then change the model orientation
      else
      {
        const float angle1 = ( ( mPointZ.y - point.y ) / scaleY );
        const float angle2 = ( ( mPointZ.x - point.x ) / scaleX );

        if( mRotateEnvironment )
        {
          Quaternion cubeOrientation = mCameraOrientationInv * mCubeOrientation;
          cubeOrientation.Conjugate();
          const Quaternion pitchRot( Radian( Degree( angle1 * -200.0f ) ), cubeOrientation.Rotate( Vector3::XAXIS ) );
          const Quaternion yawRot( Radian( Degree( angle2 * 200.0f ) ), cubeOrientation.Rotate( Vector3::YAXIS ) );

          mCubeOrientation = mCubeOrientation * pitchRot * yawRot;

          Actor skyboxActor = mSkybox.GetActor();
          if( skyboxActor )
          {
            skyboxActor.SetOrientation( mCubeOrientation );
          }

          Matrix matCube( mCameraOrientationInv * mCubeOrientation );
          matCube.Transpose();
          mModel.SetShaderUniform("uCubeMatrix" , matCube );
        }
        else
        {
          Quaternion modelOrientation = mCameraOrientationInv * mModelOrientation;
          modelOrientation.Conjugate();
          const Quaternion pitchRot( Radian( Degree( angle1 * 200.0f ) ), modelOrientation.Rotate( Vector3::XAXIS ) );
          const Quaternion yawRot( Radian( Degree( angle2 * -200.0f ) ), modelOrientation.Rotate( Vector3::YAXIS ) );

          mModelOrientation = mModelOrientation * pitchRot * yawRot;
          Actor modelActor = mModel.GetActor();
          if( modelActor )
          {
            modelActor.SetOrientation( mModelOrientation );
          }

#if defined SHADOWMAPS_RECEIVE
          // NOTE: we're applying the same modelOrientation to the shadow actor, thereby
          // simulating a link between the two. Ideally these should be constrained and so
          // should all animated parts, which is a TODO for later.
          Actor shadowActor = mShadowModel.GetActor();
          if( shadowActor )
          {
            shadowActor.SetOrientation( mModelOrientation );
          }
#endif
        }

        mPointZ = point;
      }
      break;
    }
    case PointState::UP:
    {
      mDoubleTapTime.Start();
      mDoubleTap = true;
      break;
    }
    default:
    {
      break;
    }
  }
  return true;
}

void Scene3dLauncher::OnKeyEvent( const KeyEvent& event )
{
  if( event.state == KeyEvent::Down )
  {
    if( IsKey( event, Dali::DALI_KEY_ESCAPE ) || IsKey( event, Dali::DALI_KEY_BACK ) )
    {
      mApplication.Quit();
    }
  }
}

void Scene3dLauncher::InitPbrActor()
{
  const SceneLauncher::Asset& asset = mSceneParser.GetAsset();
  SceneLauncher::DliCameraParameters camera;
  mModel.Init( ASSET_MODEL_DIR + asset.model, Vector3::ZERO, asset.modelScaleFactor, &camera, &mAnimations, &mAnimationsName );

  mModel.GetActor().SetOrientation( mModelOrientation );
  mSceneParser.SetCameraParameters( camera );
}

#if defined SHADOWMAPS_RECEIVE
void Scene3dLauncher::InitShadowActor()
{
  // Clone the actor hierarchy for shadow casting.
  mModel.Duplicate(mShadowModel);

  // Set depth writing shader + render objects inside out for increased accuracy of shadows.
  VisitActor(mShadowModel.GetActor(), [this](Actor a) {
    for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
    {
      a.GetRendererAt(i).SetShader(mShadowShader);
      a.GetRendererAt(i).SetProperty(Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::FRONT);
    }
  });

  // Gather casters
  GatherShadowCasters(mShadowModel.GetActor());

  mShadowRoot.Add(mShadowModel.GetActor());
}
#endif

void Scene3dLauncher::InitActors()
{
  Stage stage = Stage::GetCurrent();
  const SceneLauncher::Asset& asset = mSceneParser.GetAsset();

  if( mModel.GetSkyboxTexture() )
  {
    mSkybox.InitTexture( mModel.GetSkyboxTexture() );
    mSkybox.Init();
  }

  mCameraPosition = asset.cameraMatrix.GetTranslation3();

  mMainCamera.SetInvertYAxis( true );
  mMainCamera.SetPosition( mCameraPosition );
  if( asset.enablePerspective )
  {
    mMainCamera.SetNearClippingPlane( asset.cameraNear );
    mMainCamera.SetFarClippingPlane( asset.cameraFar );
    mMainCamera.SetFieldOfView( Radian( Degree( asset.cameraFov ) ) );
  }
  else
  {
    mMainCamera.SetOrthographicProjection( asset.cameraOrthographicSize.x,
                                        asset.cameraOrthographicSize.y,
                                        asset.cameraOrthographicSize.z,
                                        asset.cameraOrthographicSize.w,
                                        asset.cameraNear,
                                        asset.cameraFar );
  }

  // Setting camera parameters for 3D Scene
  // CameraActors should face in the negative Z direction, towards the other actors
  //viewQuaternion is the viewMatrix used by camera in DALI
  Quaternion viewQuaternion( Dali::ANGLE_180, Vector3::YAXIS );
  Quaternion camOrientation( asset.cameraMatrix );
  camOrientation = camOrientation * viewQuaternion;
  mMainCamera.SetOrientation( camOrientation );
  mCameraOrientationInv = camOrientation;
  mCameraOrientationInv.Conjugate();

  Property::Value uniformValue;
  if( mModel.GetUniform( "uCubeMatrix", uniformValue, -1 ) )
  {
    Matrix cubeMatrix;
    uniformValue.Get( cubeMatrix );
    mCubeOrientation = Quaternion(cubeMatrix);
  }

  Actor skyBoxActor = mSkybox.GetActor();
  if( skyBoxActor )
  {
    skyBoxActor.SetOrientation( mCubeOrientation );
    m3dRoot.Add( skyBoxActor );
  }

  Actor pbrModelActor = mModel.GetActor();
  m3dRoot.Add( pbrModelActor );

  Matrix matCube( mCameraOrientationInv * mCubeOrientation );
  matCube.Transpose();
  mModel.SetShaderUniform("uCubeMatrix" , matCube );

#if defined SHADOWMAPS_RECEIVE
  // TODO: make the light position, direction and orthographic size data driven / smart.
  float size = 1.5f;
  SetShadowCameraOrthographicProjection(-size, size, size, -size, asset.cameraNear, asset.cameraFar, false);

  SetShadowCameraPositionOrientation(Vector3(0, 5, -5),
      Quaternion(Dali::ANGLE_90 * .5f, Vector3::XAXIS));

  UpdateShadowCastersNearFar(mShadowCamera);

  mModel.AttachTexture(mShadowRenderTargetTexture, Sampler::New());
  GatherShadowReceivers(pbrModelActor);
  UpdateShadowReceiversLightSpaceTransform();
#elif defined SHADOWMAPS_CAST
  // <REMOVEME> This is to enable rendering the original actors with the shadow shader, to the front buffer.
  GatherShadowCasters(pbrModelActor);
  UpdateShadowCastersNearFar(mMainCamera);
  VisitActor(mModel.GetActor(), [this](Actor a) {
    for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
    {
      a.GetRendererAt(i).SetShader(mShadowShader);
      a.GetRendererAt(i).SetProperty(Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::FRONT);
    }
  });
  // </REMOVEME>
#endif
}

void Scene3dLauncher::ClearModel()
{
  mSkybox.Clear();
  mModel.Clear();
#if defined SHADOWMAPS_RECEIVE
  mShadowModel.Clear();
#endif
}

void Scene3dLauncher::CreateModel()
{
  UnparentAndReset( mModel.GetActor() );
#if defined SHADOWMAPS_RECEIVE
  UnparentAndReset( mShadowModel.GetActor() );
#endif
  UnparentAndReset( mErrorMessage );

#if defined SHADOWMAPS_CAST
  mShadowCasters.clear();
#endif
#if defined SHADOWMAPS_RECEIVE
  mShadowReceivers.clear();
#endif

  // Init Pbr actor
  InitPbrActor();

#if defined SHADOWMAPS_RECEIVE
  // Init Shadow actor
  InitShadowActor();
#endif

  // Initialise Main Actors
  InitActors();

  if( mAnimations.size() > 0)
  {
    PlayAnimation( mAnimations[0] );
  }
}

void Scene3dLauncher::DisplayError( const std::string& errorMessage )
{
  DALI_LOG_ERROR( "%s\n", errorMessage.c_str() );

  mErrorMessage = TextLabel::New();
  mErrorMessage.SetProperty( TextLabel::Property::TEXT, errorMessage );
  mErrorMessage.SetProperty( TextLabel::Property::TEXT_COLOR, Color::WHITE );
  mErrorMessage.SetProperty( TextLabel::Property::AUTO_SCROLL_SPEED, TEXT_AUTO_SCROLL_SPEED );
  mErrorMessage.SetProperty( TextLabel::Property::AUTO_SCROLL_LOOP_COUNT, 0 );
  mErrorMessage.SetProperty( TextLabel::Property::ENABLE_AUTO_SCROLL, true );

  Property::Map colorMap;
  colorMap.Insert( Visual::Property::TYPE, Visual::COLOR );
  colorMap.Insert( ColorVisual::Property::MIX_COLOR, Color::BLACK );
  mErrorMessage.SetProperty( Control::Property::BACKGROUND, colorMap );

  mErrorMessage.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH );
  mErrorMessage.SetParentOrigin( ParentOrigin::CENTER );
  mErrorMessage.SetAnchorPoint( AnchorPoint::CENTER );

  mUiRoot.Add( mErrorMessage );
}

void Scene3dLauncher::PlayAnimation( std::vector<Animation> animationList )
{
  for(std::vector<Animation>::iterator it = animationList.begin(); it != animationList.end(); ++it)
  {
    (*it).Play();
  }
}

#if defined SHADOWMAPS_CAST
void Scene3dLauncher::GatherShadowCasters(Actor actor)
{
  auto fn = [this](Actor a) {
    if(a.GetRendererCount() > 0)
    {
      ShadowCaster sc = { a, a.RegisterProperty("uNearFar", Vector2::ZERO) };
      mShadowCasters.push_back(sc);
    }
  };

  VisitActor(actor, fn);
}

void Scene3dLauncher::UpdateShadowCastersNearFar(CameraActor cam)
{
  Vector2 nearFar { cam.GetNearClippingPlane(),
    1.0f / (cam.GetFarClippingPlane() - cam.GetNearClippingPlane()) };
  for(auto& t: mShadowCasters)
  {
    t.actor.SetProperty(t.uNearFar, nearFar);
  }
}
#endif

#if defined SHADOWMAPS_RECEIVE
void Scene3dLauncher::GatherShadowReceivers(Actor actor)
{
  auto fn = [this](Actor a) {
    if(a.GetRendererCount() > 0)
    {
      ShadowReceiver sr = { a,
          a.RegisterProperty("uInverseShadowmapSize", Vector2::ONE),
          a.RegisterProperty("uLightSpaceTransform", Matrix::IDENTITY),
          a.RegisterProperty("uLightDir", -Vector3::YAXIS)};
      mShadowReceivers.push_back(sr);
    }
  };

  VisitActor(actor, fn);
}

void Scene3dLauncher::UpdateShadowReceiversLightSpaceTransform()
{
  Matrix mtxView = mShadowCameraWorld;
  mtxView.Invert();

  Matrix mtxLightSpaceTransform = mtxView;
  Matrix::Multiply(mtxLightSpaceTransform, mtxView, mShadowCameraProjection);

  Vector3 lightDir = Quaternion(mtxView).Rotate(Vector3::ZAXIS);

  Vector2 inverseTextureSize = Vector2(1.0f / mShadowRenderTargetTexture.GetWidth(),
      1.0f / mShadowRenderTargetTexture.GetHeight());

  for(auto& t: mShadowReceivers)
  {
    t.actor.SetProperty(t.uInverseShadowmapSize, inverseTextureSize);
    t.actor.SetProperty(t.uLightSpaceTransform, mtxLightSpaceTransform);
    t.actor.SetProperty(t.uLightDir, lightDir);
  }
}

void Scene3dLauncher::CalculateOrthographicProjection(float left, float right, float top, float bottom, float near, float far, Matrix& matrixOut)
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

void Scene3dLauncher::SetShadowCameraOrthographicProjection(float left, float right,
    float top, float bottom, float near, float far, bool flipY)
{
  mShadowCamera.SetInvertYAxis(flipY);
  mShadowCamera.SetOrthographicProjection(left, right, top, bottom, near, far);

  if (flipY)
  {
    std::swap(top, bottom);
  }
  CalculateOrthographicProjection(left, right, bottom, top, near, far, mShadowCameraProjection);
}

void Scene3dLauncher::SetShadowCameraPositionOrientation(Vector3 position, Quaternion orientation)
{
  mShadowCameraWorld.SetTransformComponents(Vector3::ONE, orientation, position);

  mShadowCamera.SetPosition(position);
  mShadowCamera.SetOrientation(orientation);
}
#endif

#if defined SHADOWMAPS_DEBUG
Geometry Scene3dLauncher::NewUnitQuadWithWholeTexture(bool flipY)
{
  // Create geometry -- unit square with whole of the texture mapped to it.
  struct Vertex
  {
    Vector3 aPosition;
    Vector2 aUv0;
  };

  Vertex vertexData[] = {
    { Vector3(-1.0f, 1.0f, .0f), Vector2(.0f, 1.0f) },
    { Vector3(1.0f, 1.0f, .0f), Vector2(1.0f, 1.0f) },
    { Vector3(-1.0f, -1.0f, .0f), Vector2(.0f, .0f) },
    { Vector3(1.0f, -1.0f, .0f), Vector2(1.0f, .0f) },
  };

  if (flipY)
  {
    std::swap(vertexData[0].aUv0, vertexData[2].aUv0);
    std::swap(vertexData[1].aUv0, vertexData[3].aUv0);
  }

  PropertyBuffer vbo = PropertyBuffer::New( Property::Map()
    .Add("aPosition", Property::VECTOR3)
    .Add("aUv0", Property::VECTOR2));
  vbo.SetData(vertexData, std::extent<decltype(vertexData)>::value);

  Geometry geom = Geometry::New();
  geom.AddVertexBuffer(vbo);
  geom.SetType( Geometry::TRIANGLE_STRIP );
  return geom;
}

Actor Scene3dLauncher::CreateTexturedQuadActor(Shader shader, Texture texture, float size, bool flipY)
{
  TextureSet textures = TextureSet::New();
  textures.SetTexture(0, texture);

  // Create geometry and renderer.
  Geometry geom = NewUnitQuadWithWholeTexture(flipY);
  Renderer renderer = Renderer::New( geom, shader );
  renderer.SetProperty(Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::BACK);
  renderer.SetTextures(textures);

  float w = size;
  float h = size * texture.GetHeight() / texture.GetWidth();

  // Create actor.
  Actor actor = Actor::New();
  actor.SetName("TexturedQuad");
  actor.SetSize( Vector3(w, h, 1.0f));
  actor.AddRenderer(renderer);
  actor.SetVisible(true);

  return actor;
}
#endif

// Entry point for Linux & Tizen applications
//
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv);

  Scene3dLauncher test( application );

  application.MainLoop();

  return 0;
}
