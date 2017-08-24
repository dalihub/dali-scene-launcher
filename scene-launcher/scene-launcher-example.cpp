/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

// INTERNAL INCLUDES
#include "model-pbr.h"
#include "scene-file-parser.h"
#include "model-skybox.h"

using namespace Dali;
using namespace Toolkit;

namespace
{

const char* MODEL_DIR_URL = SCENE_LAUNCHER_MODEL_DIR "scenes";

const std::string ASSET_MODEL_DIR = SCENE_LAUNCHER_MODEL_DIR;

const Vector3 CAMERA_DEFAULT_POSITION( 0.0f, 0.0f, 3.5f );

const float TEXT_AUTO_SCROLL_SPEED = 200.f;

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

  Scene3dLauncher( Application& application )
  : mApplication( application ),
    mSceneParser(),
    mDoubleTapTime(),
    mCameraPosition( CAMERA_DEFAULT_POSITION ),
    mViewQuaternion( Dali::ANGLE_180, Vector3::YAXIS ),
    mModelOrientation(),
    mCubeOrientation(),
    mZoomLevel( 1.f ),
    mDoubleTap( false ),
    mRotateEnvironment( true )
  {
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
    stage.SetBackgroundColor( Color::BLACK );

    try
    {
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
    stage.GetRootLayer().TouchSignal().Connect( this, &Scene3dLauncher::OnTouch );

    // Respond to key events
    stage.KeyEventSignal().Connect( this, &Scene3dLauncher::OnKeyEvent );

    mDoubleTapTime = Timer::New(150);
    mDoubleTapTime.TickSignal().Connect( this, &Scene3dLauncher::OnDoubleTapTime );
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

          Actor skyboxActor = mSkybox.GetActor();
          if( skyboxActor )
          {
            skyboxActor.SetPosition( mCameraPosition * ( mZoomLevel + 0.1f ) );
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
            Quaternion cubeOrientation = mCubeOrientation;
            cubeOrientation.Conjugate();
            const Quaternion pitchRot( Radian( Degree( angle1 * -200.0f ) ), cubeOrientation.Rotate( Vector3::XAXIS ) );
            const Quaternion yawRot( Radian( Degree( angle2 * -200.0f ) ), cubeOrientation.Rotate( Vector3::YAXIS ) );

            mCubeOrientation = mCubeOrientation * pitchRot * yawRot;
            Quaternion ViewQuaternion(mViewQuaternion);
            ViewQuaternion.Conjugate();
            Actor skyboxActor = mSkybox.GetActor();
            if( skyboxActor )
            {
              skyboxActor.SetOrientation( ViewQuaternion * mCubeOrientation );
            }

            Matrix matCube(mCubeOrientation);
            matCube.Transpose();
            mModel.SetShaderUniform("uCubeMatrix" , matCube );
          }
          else
          {
            Quaternion modelOrientation = mModelOrientation;
            modelOrientation.Conjugate();
            const Quaternion pitchRot( Radian( Degree( angle1 * -200.0f ) ), modelOrientation.Rotate( Vector3::XAXIS ) );
            const Quaternion yawRot( Radian( Degree( angle2 * -200.0f ) ), modelOrientation.Rotate( Vector3::YAXIS ) );

            mModelOrientation = mModelOrientation * pitchRot * yawRot;
            Actor modelActor = mModel.GetActor();
            if( modelActor )
            {
              modelActor.SetOrientation( mModelOrientation );
            }
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

  /**
   * @brief Initialise model geometry, shader, position and orientation
   */
  void InitPbrActor()
  {
    const SceneLauncher::Asset& asset = mSceneParser.GetAsset();

    mModel.Init( ASSET_MODEL_DIR + asset.model, Vector3::ZERO, asset.modelScaleFactor, &mLoadAnimation );

    mModel.GetActor().SetOrientation( mModelOrientation );

    Stage::GetCurrent().Add( mModel.GetActor() );
    mModel.GetActor().SetProperty(DevelActor::Property::SIBLING_ORDER, 1 );
  }

  /**
   * @brief Creates scene actors and setup camera parameters
   */
  void InitActors()
  {
    Stage stage = Stage::GetCurrent();
    const SceneLauncher::Asset& asset = mSceneParser.GetAsset();

    mSkybox.InitTexture( mModel.GetCubeSpecularTexture() );
    mSkybox.Init();
    Quaternion ViewQuaternion(mViewQuaternion);
    ViewQuaternion.Conjugate();
    mSkybox.GetActor().SetOrientation( ViewQuaternion * mCubeOrientation );

    // Setting camera parameters for 3D Scene
    mSkybox.GetActor().SetPosition( asset.cameraPosition );
    mCameraPosition = asset.cameraPosition;
    CameraActor camera3d = stage.GetRenderTaskList().GetTask(0).GetCameraActor();
    camera3d.SetInvertYAxis( true );
    camera3d.SetPosition( asset.cameraPosition );
    camera3d.SetNearClippingPlane( asset.cameraNear );
    camera3d.SetFarClippingPlane( asset.cameraFar );
    camera3d.SetFieldOfView( Radian( Degree( asset.cameraFov ) ) );

    // CameraActors should face in the negative Z direction, towards the other actors
    // This is ViewMatrix;
    camera3d.SetOrientation(mViewQuaternion);

    stage.Add( mSkybox.GetActor() );
    mSkybox.GetActor().SetProperty(DevelActor::Property::SIBLING_ORDER, 0 );
  }

  /*
   * @brief Clear resources
   */
  void ClearModel()
  {
    mSkybox.Clear();
    mModel.Clear();
  }

  /*
   * @brief loads model and initialise textures.
   */
  void CreateModel()
  {
    UnparentAndReset( mErrorMessage );

    // Init Pbr actor
    InitPbrActor();


    // Initialise Main Actors
    InitActors();
    PlayAnimation( mLoadAnimation );
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

    Stage::GetCurrent().Add( mErrorMessage );
  }

  void PlayAnimation(std::vector<Animation> animationList )
  {
     for(std::vector<Animation>::iterator it = animationList.begin(); it != animationList.end(); ++it)
     {
       (*it).Play();
     }
  }

private:
  Application& mApplication;

  SceneLauncher::SceneFileParser mSceneParser;

  TextLabel mErrorMessage;
  Timer mDoubleTapTime;

  ModelSkybox mSkybox;
  SceneLauncher::ModelPbr mModel;

  Vector3 mCameraPosition;
  Vector2 mPointZ;
  Vector2 mStartTouch;

  //Quaternion of ViewMatrix
  Quaternion mViewQuaternion;
  Quaternion mModelOrientation;
  Quaternion mCubeOrientation;

  float mZoomLevel;
  bool mDoubleTap;
  bool mRotateEnvironment;
  std::vector<Animation> mLoadAnimation;
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
