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
#include <dali/integration-api/debug.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/actors/actor-devel.h>

#include <iostream>

// INTERNAL INCLUDES
#include "model-pbr.h"
#include "scene-file-parser.h"
#include "model-skybox.h"
#include "utils.h"

using namespace Dali;
using namespace Toolkit;

#define ENABLE_CAST_SHADOWS // On its own it'll just render the scene with the shadow shader, with the main framebuffer
#define ENABLE_RECEIVE_SHADOWS

#if defined ENABLE_RECEIVE_SHADOWS
// If casting wasn't enabled, this will enable it now since it doesn't make much sense without that.
#if !defined ENABLE_CAST_SHADOWS
#define ENABLE_CAST_SHADOWS
#endif
#endif

namespace
{

const char* MODEL_DIR_URL = SCENE_LAUNCHER_MODEL_DIR "scenes";

const std::string ASSET_MODEL_DIR = SCENE_LAUNCHER_MODEL_DIR;

const Vector3 CAMERA_DEFAULT_POSITION( 0.0f, 0.0f, 3.5f );

const float TEXT_AUTO_SCROLL_SPEED = 200.f;

#if defined ENABLE_RECEIVE_SHADOWS
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

/*
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

  static Geometry NewUnitQuadWithWholeTexture(bool flipY)
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

  static Actor CreateTexturedQuadActor(Shader shader, Texture texture, bool flipY)
  {
    TextureSet textures = TextureSet::New();
    textures.SetTexture(0, texture);

    // Create geometry and renderer.
    Geometry geom = NewUnitQuadWithWholeTexture(flipY);
    Renderer renderer = Renderer::New( geom, shader );
    //renderer.SetProperty(Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::BACK);
    renderer.SetTextures(textures);

    // Figure out a sensible scale. We'll set this on the actor as size.
    float w = texture.GetWidth() / 4;
    float h = texture.GetHeight() / 4;

    // Create actor.
    Actor actor = Actor::New();
    actor.SetName("TexturedQuad");
    actor.SetSize( Vector3(w, h, 1.0f));
    actor.AddRenderer(renderer);
    actor.SetVisible(true);

    return actor;
  }

  Scene3dLauncher( Application& application )
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

  ~Scene3dLauncher()
  {
    // Nothing to do here;
  }

  /**
   * @brief The Init signal is received once (only) during the Application lifetime
   */
  void Create( Application& application )
  {
    // Disable indicator
    Dali::Window winHandle = application.GetWindow();
    winHandle.ShowIndicator( Dali::Window::INVISIBLE );

    // Get a handle to the stage
    Stage stage = Stage::GetCurrent();
    stage.SetBackgroundColor( Color::BLUE );

    // Creating root and camera actor for rendertask for 3D Scene rendering
    mUiRoot = Actor::New();
    mUiRoot.SetName("UI Root");
    mUiRoot.SetAnchorPoint(AnchorPoint::TOP_LEFT);
    mUiRoot.SetParentOrigin(ParentOrigin::TOP_LEFT);
    mUiRoot.SetSize(stage.GetSize());

    m3dRoot = Actor::New();
    m3dRoot.SetName("3D Root");
    m3dRoot.SetAnchorPoint(AnchorPoint::CENTER);
    m3dRoot.SetParentOrigin(ParentOrigin::CENTER);

#if defined ENABLE_RECEIVE_SHADOWS
    mShadowRoot = Actor::New();
    mShadowRoot.SetName("Shadow Root");
    mShadowRoot.SetAnchorPoint(AnchorPoint::CENTER);
    mShadowRoot.SetParentOrigin(ParentOrigin::CENTER);
#endif

    RenderTask mainPass = stage.GetRenderTaskList().GetTask(0);
    mMainCamera = mainPass.GetCameraActor();
    mainPass.SetSourceActor(m3dRoot);
    mainPass.SetExclusive(true);

    mUiCamera = CameraActor::New( stage.GetSize() );
    mUiCamera.SetAnchorPoint(AnchorPoint::CENTER);
    mUiCamera.SetParentOrigin(ParentOrigin::CENTER);
    RenderTask uiRendertask = stage.GetRenderTaskList().CreateTask();
    uiRendertask.SetCameraActor( mUiCamera );
    uiRendertask.SetSourceActor( mUiRoot );
    uiRendertask.SetExclusive( true );

#if defined ENABLE_RECEIVE_SHADOWS
    int shadowSize = 512;
    mShadowRenderTargetTexture = Texture::New(TextureType::TEXTURE_2D, Pixel::Format::RGB888, shadowSize, shadowSize);
    mShadowRenderTarget = FrameBuffer::New(shadowSize, shadowSize, FrameBuffer::Attachment::DEPTH);
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
    shadowPass.SetViewportSize(Vector2(shadowSize, shadowSize));
    shadowPass.SetClearEnabled(true);
    shadowPass.SetClearColor(Vector4::ONE);

    // debug quad
    Shader plainOldShader = Shader::New(VERTEX_SHADER_FLAT, FRAGMENT_SHADER_FLAT);
    Actor shadowDebugActor = CreateTexturedQuadActor(plainOldShader, mShadowRenderTargetTexture, false);
    shadowDebugActor.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER);
    shadowDebugActor.SetParentOrigin( ParentOrigin::BOTTOM_CENTER);

#endif

    stage.Add( mUiCamera );
#if defined ENABLE_RECEIVE_SHADOWS
    stage.Add( mShadowCamera );
#endif

    stage.Add( mUiRoot );
    stage.Add( m3dRoot );
#if defined ENABLE_RECEIVE_SHADOWS
    stage.Add( mShadowRoot );

    mUiRoot.Add( shadowDebugActor);
#endif

    try
    {
#if defined ENABLE_CAST_SHADOWS
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

  /*
   * @brief loads model and initialise textures.
   */
  void CreateModel()
  {
    UnparentAndReset( mModel.GetActor() );
#if defined ENABLE_RECEIVE_SHADOWS
    UnparentAndReset( mShadowModel.GetActor() );
#endif
    UnparentAndReset( mErrorMessage );

    mTiltables.clear();
#if defined ENABLE_CAST_SHADOWS
    mShadowCasters.clear();
#endif
#if defined ENABLE_RECEIVE_SHADOWS
    mShadowReceivers.clear();
#endif

    // Init Pbr actor
    InitPbrActor();

#if defined ENABLE_RECEIVE_SHADOWS
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

  /**
   * @brief Initialise model geometry, shader, position and orientation
   */
  void InitPbrActor()
  {
    const SceneLauncher::Asset& asset = mSceneParser.GetAsset();
    SceneLauncher::DliCameraParameters camera;
    mModel.Init( ASSET_MODEL_DIR + asset.model, Vector3::ZERO, asset.modelScaleFactor, &camera, &mAnimations, &mAnimationsName );

    mModel.GetActor().SetOrientation( mModelOrientation );
    mSceneParser.SetCameraParameters( camera );
  }

#if defined ENABLE_RECEIVE_SHADOWS
  void InitShadowActor()
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

  /**
   * @brief Creates scene actors and setup camera parameters
   */
  void InitActors()
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
    mMainCamera.SetNearClippingPlane( asset.cameraNear );
    mMainCamera.SetFarClippingPlane( asset.cameraFar );
    mMainCamera.SetFieldOfView( Radian( Degree( asset.cameraFov ) ) );

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
      cubeMatrix.Transpose();
      mCubeOrientation = camOrientation * Quaternion( cubeMatrix );
    }

    Actor skyBoxActor = mSkybox.GetActor();
    if( skyBoxActor )
    {
      skyBoxActor.SetOrientation( mCubeOrientation );
      m3dRoot.Add( skyBoxActor );
    }

    Actor pbrModelActor = mModel.GetActor();
    m3dRoot.Add( pbrModelActor );

    GatherTiltables(pbrModelActor);

    Matrix matCube( mCameraOrientationInv * mCubeOrientation );
    matCube.Transpose();
    mModel.SetShaderUniform("uCubeMatrix" , matCube );

#if defined ENABLE_RECEIVE_SHADOWS
    float size = 3.0f;
    SetShadowCameraOrthographicProjection(-size, size, size, -size, asset.cameraNear, asset.cameraFar, false);
//    SetShadowCameraPerspectiveProjection(asset.cameraNear, asset.cameraFar, asset.cameraFov * .5, true);

    // TODO: make these data-driven either via multi-camera support or using some named entity.
    SetShadowCameraPositionOrientation(Vector3(0, 5, 0),
        Quaternion(Dali::ANGLE_90, Vector3::XAXIS));

    UpdateShadowCastersNearFar(mShadowCamera);

    Sampler sampler = Sampler::New();
    sampler.SetFilterMode(FilterMode::NEAREST, FilterMode::NEAREST);

    mModel.AttachTexture(mShadowRenderTargetTexture, sampler);
    GatherShadowReceivers(pbrModelActor);
    UpdateShadowReceiversLightSpaceTransform();
#elif defined ENABLE_CAST_SHADOWS
    // <REMOVEME> This is to enable rendering the original actors with the shadow shader, to the front buffer.
    GatherShadowCasters(pbrModelActor);
    UpdateShadowCastersNearFar(mMainCamera);
    VisitActor(mModel.GetActor(), [this](Actor a) {
      for(unsigned int i = 0; i < a.GetRendererCount(); ++i)
      {
        a.GetRendererAt(i).SetShader(mShadowShader);
      }
    });
    // </REMOVEME>
#endif
  }

  /*
   * @brief Clear resources
   */
  void ClearModel()
  {
    mSkybox.Clear();
    mModel.Clear();
    mShadowModel.Clear();
  }


  void DisplayError( const std::string& errorMessage )
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

  void PlayAnimation( std::vector<Animation> animationList )
  {
     for(std::vector<Animation>::iterator it = animationList.begin(); it != animationList.end(); ++it)
     {
       (*it).Play();
     }
  }

  void SetTilt(Vector2 tilt)
  {
    tilt *= .01f;
    tilt.x *= -1.0f;
    for(auto& t: mTiltables)
    {
      t.actor.SetProperty(t.uTilt, tilt);
    }
  }


  /*
   * @brief Function triggered by timer, to calculate the double tap event
   */
  bool OnDoubleTapTime()
  {
    mDoubleTap = false;
    return true;
  }

  /**
   * @brief This function will zoom of model, and orientation or model or environment according to mode.
   *
   * The mode can be changed double tap the screen bottom, if double tap
   * in other place, will change model.
   */
  bool OnTouch( Actor actor, const TouchData& touch )
  {
    const PointState::Type state = touch.GetState( 0 );

// TODO: Parallax integration.
//    switch( state )
//    {
//    case PointState::DOWN:
//      mStartTouch = touch.GetScreenPosition(0);
//      break;
//
//    case PointState::MOTION:
//      SetTilt(mStartTouch - touch.GetScreenPosition(0));
//      break;
//
//    case PointState::UP:
//      SetTilt(Vector2(.0f, .0f));
//      break;
//
//    default:
//      break;
//    }

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

            // NOTE: we're applying the same modelOrientation to the shadow actor, thereby
            // simulating a link between the two. Ideally these should be constrained and so
            // should all animated parts, which is a TODO for later.
            Actor shadowActor = mShadowModel.GetActor();
            if( shadowActor )
            {
              shadowActor.SetOrientation( mModelOrientation );
            }

            std::cout << "######## MATRICES #########" << std::endl;
            std::cout << "DALi Proj: " << mShadowCamera.GetProperty(CameraActor::Property::PROJECTION_MATRIX) << std::endl;
            std::cout << "Handmade Proj: " << mShadowCameraProjection << std::endl;
            std::cout << "DALi World: " << mShadowCamera.GetProperty(Actor::Property::WORLD_MATRIX) << std::endl;
            std::cout << "Handmade World: " << mShadowCameraModel << std::endl;
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

  /**
   * @brief Called when any key event is received
   *
   * Will use this to quit the application if Back or the Escape key is received
   * @param[in] event The key event information
   */
  void OnKeyEvent( const KeyEvent& event )
  {
    if( event.state == KeyEvent::Down )
    {
      if( IsKey( event, Dali::DALI_KEY_ESCAPE ) || IsKey( event, Dali::DALI_KEY_BACK ) )
      {
        mApplication.Quit();
      }
    }
  }

private:
  void GatherTiltables(Actor actor)
  {
    auto fn = [this](Actor a) {
      if(a.GetRendererCount() > 0)
      {
        Tiltable t = { a, a.RegisterProperty("uTilt", Vector2::ZERO) };

        mTiltables.push_back(t);
      }
    };

    VisitActor(actor, fn);
  }

#if defined ENABLE_CAST_SHADOWS
  void GatherShadowCasters(Actor actor)
  {
    auto fn = [this](Actor a) {
      if(a.GetRendererCount() > 0)
      {
        printf("Caster: %s\n", a.GetName().c_str());
        ShadowCaster sc = { a, a.RegisterProperty("uNearFar", Vector2::ZERO) };
        mShadowCasters.push_back(sc);
      }
    };

    VisitActor(actor, fn);
  }

  void UpdateShadowCastersNearFar(CameraActor cam)
  {
    Vector2 nearFar { cam.GetNearClippingPlane(), 1.0f / (cam.GetFarClippingPlane() - cam.GetNearClippingPlane()) };
    printf("Updating casters with near: %.2f, far: %.2f\n", nearFar.x, nearFar.y);
    for(auto& t: mShadowCasters)
    {
      t.actor.SetProperty(t.uNearFar, nearFar);
    }
  }
#endif

#if defined ENABLE_RECEIVE_SHADOWS
  void GatherShadowReceivers(Actor actor)
  {
    auto fn = [this](Actor a) {
      if(a.GetRendererCount() > 0)
      {
        printf("Receiver: %s\n", a.GetName().c_str());
        ShadowReceiver sr = { a,
            a.RegisterProperty("uLightSpaceTransform", Matrix::IDENTITY),
            a.RegisterProperty("uLightDir", -Vector3::YAXIS)};
        mShadowReceivers.push_back(sr);
      }
    };

    VisitActor(actor, fn);
  }

  void UpdateShadowReceiversLightSpaceTransform()
  {
    Matrix mtxView = mShadowCameraModel;
    std::cout << "UpdateShadowReceiversLightSpace" << std::endl;
    std::cout << "view:" << mtxView << std::endl;
    mtxView.Invert();
    std::cout << "inverse view: " << mtxView << std::endl;
    std::cout << "projection: " << mShadowCameraProjection << std::endl;

    Matrix mtxLightSpaceTransform = mtxView;
    Matrix::Multiply(mtxLightSpaceTransform, mtxView, mShadowCameraProjection);
    std::cout << "lightspace: " << mtxLightSpaceTransform << std::endl;

    Vector3 lightDir = Quaternion(mtxView).Rotate(-Vector3::ZAXIS);
    std::cout << "light dir: " << lightDir << std::endl;

    for(auto& t: mShadowReceivers)
    {
      t.actor.SetProperty(t.uLightSpaceTransform, mtxLightSpaceTransform);
      t.actor.SetProperty(t.uLightDir, lightDir);
    }
  }

  static void CalculateOrthographicProjection(float left, float right, float top, float bottom, float near, float far, Matrix& matrixOut)
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

  void SetShadowCameraOrthographicProjection(float left, float right,
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

  void SetShadowCameraPerspectiveProjection(float near, float far, float fovDegrees, bool flipY)
  {
    float fovRadians = Radian(Degree(fovDegrees));

    float* data = mShadowCameraProjection.AsFloat();
    float invTanHalfFov = -1.0f / std::tan(.5f * fovRadians);
    float deltaNearFar = near - far;
    data[0] = invTanHalfFov;  // Square shadow map -- aspect ratio 1:1.
    data[1] = .0;
    data[2] = .0;
    data[3] = .0;
    data[4] = .0;
    data[5] = invTanHalfFov * (flipY ? -1.0f : 1.0f);
    data[6] = .0;
    data[7] = .0;
    data[8] = .0;
    data[9] = .0;
    data[10] = (-near - far) / deltaNearFar;
    data[11] = 1.0f;
    data[12] = .0;
    data[13] = .0;
    data[14] = 2 * near * far / deltaNearFar;
    data[15] = .0;

    mShadowCamera.SetInvertYAxis(flipY);
    mShadowCamera.SetPerspectiveProjection(Vector2(
        mShadowRenderTargetTexture.GetWidth(),
        mShadowRenderTargetTexture.GetHeight()));
    mShadowCamera.SetNearClippingPlane(near);
    mShadowCamera.SetFarClippingPlane(far);
    mShadowCamera.SetFarClippingPlane(far);
    mShadowCamera.SetFieldOfView( fovRadians );
  }

  void SetShadowCameraPositionOrientation(Vector3 position, Quaternion orientation)
  {
    mShadowCameraModel.SetTransformComponents(Vector3::ONE, orientation, position);

    mShadowCamera.SetPosition(position);
    mShadowCamera.SetOrientation(orientation);
  }
#endif

  Application& mApplication;

  SceneLauncher::SceneFileParser mSceneParser;

  TextLabel mErrorMessage;
  Timer mDoubleTapTime;

  Actor m3dRoot;
  Actor mUiRoot;
#if defined ENABLE_RECEIVE_SHADOWS
  Actor mShadowRoot;
#endif

  CameraActor mMainCamera;
  CameraActor mUiCamera;
#if defined ENABLE_RECEIVE_SHADOWS
  CameraActor mShadowCamera;

  Matrix mShadowCameraModel;
  Matrix mShadowCameraProjection;
#endif

  ModelSkybox mSkybox;

  SceneLauncher::ModelPbr mModel;
#if defined ENABLE_RECEIVE_SHADOWS
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

  struct Tiltable
  {
    Actor actor;
    Property::Index uTilt;
  };

  std::vector<Tiltable> mTiltables;

#if defined ENABLE_CAST_SHADOWS
  Shader mShadowShader;

  struct ShadowCaster
  {
    Actor actor;
    Property::Index uNearFar;
  };

  std::vector<ShadowCaster> mShadowCasters;
#endif

#if defined ENABLE_RECEIVE_SHADOWS
  FrameBuffer mShadowRenderTarget;
  Texture mShadowRenderTargetTexture;

  struct ShadowReceiver
  {
    Actor actor;
    Property::Index uLightSpaceTransform;
    Property::Index uLightDir;
  };

  std::vector<ShadowReceiver> mShadowReceivers;
#endif
};

// Entry point for Linux & Tizen applications
//
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv);

  Scene3dLauncher test( application );

  application.MainLoop();

  return 0;
}
