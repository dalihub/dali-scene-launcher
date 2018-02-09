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

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include "dli-loader.h"

namespace
{

const char* MODEL_DIR_URL = SCENE_LAUNCHER_MODEL_DIR "scenes";

const Vector3 CAMERA_DEFAULT_POSITION( 0.0f, 0.0f, 3.5f );

const float TEXT_AUTO_SCROLL_SPEED = 200.f;

const std::string HOUR_0_11_MINUTE_STR( "hour0-11.minute" );
const std::string HOUR_0_23_MINUTE_STR( "hour0-23.minute" );
const std::string MINUTE_SECOND_STR( "minute.second" );
const std::string SECOND_MILLISECOND_STR( "second.millisecond" );
} // namespace

Scene3dLauncher::Scene3dLauncher( Application& application )
: mApplication( application ),
  mSceneFileParser(),
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
  mRotateEnvironment( true )
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
  stage.SetBackgroundColor( Color::BLACK );

  // Creating root and camera actor for rendertask for 3D Scene rendering
  mUiRoot = Actor::New();
  m3dRoot = Actor::New();
  CameraActor cameraUi = CameraActor::New( stage.GetSize() );
  cameraUi.SetAnchorPoint(AnchorPoint::CENTER);
  cameraUi.SetParentOrigin(ParentOrigin::CENTER);
  RenderTask rendertask = Stage::GetCurrent().GetRenderTaskList().CreateTask();
  rendertask.SetCameraActor( cameraUi );
  rendertask.SetSourceActor( mUiRoot );
  rendertask.SetExclusive( true );

  mUiRoot.SetAnchorPoint(AnchorPoint::TOP_LEFT);
  mUiRoot.SetParentOrigin(ParentOrigin::TOP_LEFT);
  mUiRoot.SetSize(stage.GetSize());

  m3dRoot.SetAnchorPoint(AnchorPoint::CENTER);
  m3dRoot.SetParentOrigin(ParentOrigin::CENTER);

  stage.Add( cameraUi );
  stage.Add( mUiRoot );
  stage.Add( m3dRoot );

  try
  {
    // Read models from the filesystem
    mSceneFileParser.ReadModelFolder( MODEL_DIR_URL );

    CreateModel();
  }
  catch( DaliException& e )
  {
    std::stringstream stream;
    stream << "Error while loading " << mSceneFileParser.GetModelFile() << ". Error : " << std::string( e.condition );

    DisplayError( stream.str() );
  }
  catch( ... )
  {
    DALI_LOG_ERROR( "Unknown error while loading %s\n", mSceneFileParser.GetModelFile().c_str() );
  }

  // Respond to a click anywhere on the stage
  mUiRoot.TouchSignal().Connect( this, &Scene3dLauncher::OnTouch );
  // Respond to key events
  stage.KeyEventSignal().Connect( this, &Scene3dLauncher::OnKeyEvent );

  mDoubleTapTime = Timer::New(150);
  mDoubleTapTime.TickSignal().Connect( this, &Scene3dLauncher::OnDoubleTapTime );

  // Load lua scripts
  const std::string scriptDir( SCENE_LAUNCHER_LUA_SCRIPTS_DIR );
  for( const auto& script : mScripts )
  {
    mLua.LoadScriptFile( ( scriptDir + script.url ).c_str() );
  }

  // Call the function OnCreate implemented in lua.
  mLua.FetchFunction( "OnCreate" );

  mLua.PushParameter( static_cast<void*>( this ) );

  mLua.ExecuteFunction( 0 ); // 0 is the number of returned parameters.

  // Starts the data provider.
  mDataProvider.Start();
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
  // Call the OnKeyEvent function in lua.
  mLua.FetchFunction( "OnKeyEvent" );

  mLua.PushParameter( event.keyCode );
  mLua.PushParameter( event.keyModifier );
  mLua.PushParameter( event.state );

  mLua.ExecuteFunction( 0 ); // 0 is the number of returned parameters.
}

void Scene3dLauncher::SetUpEvents( const std::vector<SceneLauncher::DliLoader::Script>& scripts )
{
  unsigned int scriptIndex = 0u;
  for( const auto& script : scripts )
  {
    unsigned int eventIndex = 0u;
    for( const auto& event : script.events )
    {
      // Find the data provider id.
      unsigned int dataProviderId = 0u;
      if( HOUR_0_11_MINUTE_STR == event.source )
      {
        dataProviderId = SceneLauncher::DataProvider::Notification::HOUR_0_11_MINUTE;
      }
      else if( HOUR_0_23_MINUTE_STR == event.source )
      {
        dataProviderId = SceneLauncher::DataProvider::Notification::HOUR_0_23_MINUTE;
      }
      else if( MINUTE_SECOND_STR == event.source )
      {
        dataProviderId = SceneLauncher::DataProvider::Notification::MINUTE_SECOND;
      }
      else if( SECOND_MILLISECOND_STR == event.source )
      {
        dataProviderId = SceneLauncher::DataProvider::Notification::SECOND_MILLISECOND;
      }
      else
      {
        // do nothing
        continue;
      }

      // Find the item in the vector of data providers.
      unsigned int dataProviderVectorIndex = 0u;
      bool found = false;
      for( auto& observer : mDataProviderObservers )
      {
        if( dataProviderId == observer.dataProviderId )
        {
          found = true;
          break;
        }

        ++dataProviderVectorIndex;
      }

      if( !found )
      {
        dataProviderVectorIndex = mDataProviderObservers.size();
        mDataProviderObservers.push_back( DataProviderObserver() );
      }

      DataProviderObserver& dataProviderObserver = *( mDataProviderObservers.begin() + dataProviderVectorIndex );

      // Fill the DataProviderObserver

      // Set the data provider id
      dataProviderObserver.dataProviderId = dataProviderId;

      // Push the callback index to retrieve it when there is a notification.
      DataProviderObserver::Index callbackIndex;
      callbackIndex.scriptIndex = scriptIndex;
      callbackIndex.eventIndex = eventIndex;

      dataProviderObserver.callbackIndices.PushBack( callbackIndex );

      ++eventIndex;
    }
    ++scriptIndex;
  }

  mDataProvider.Register( this );
}

void Scene3dLauncher::InitPbrActor()
{
  SceneLauncher::Asset& asset = mSceneFileParser.GetAsset();
  mModel.Init( asset,
               Vector3::ZERO,
               mAnimations,
               mAnimationsName,
               mScripts );

  mModel.GetActor().SetOrientation( mModelOrientation );
}

void Scene3dLauncher::InitActors()
{
  Stage stage = Stage::GetCurrent();
  const SceneLauncher::Asset& asset = mSceneFileParser.GetAsset();

  if( mModel.GetSkyboxTexture() )
  {
    mSkybox.InitTexture( mModel.GetSkyboxTexture() );
    mSkybox.Init();
  }

  mCameraPosition = asset.camera.cameraMatrix.GetTranslation3();

  CameraActor camera3d = stage.GetRenderTaskList().GetTask(0).GetCameraActor();
  camera3d.SetInvertYAxis( true );
  camera3d.SetPosition( mCameraPosition );
  if( asset.camera.enablePerspective )
  {
    camera3d.SetNearClippingPlane( asset.camera.cameraNear );
    camera3d.SetFarClippingPlane( asset.camera.cameraFar );
    camera3d.SetFieldOfView( Radian( Degree( asset.camera.cameraFov ) ) );
  }
  else
  {
    camera3d.SetOrthographicProjection( asset.camera.cameraOrthographicSize.x,
                                        asset.camera.cameraOrthographicSize.y,
                                        asset.camera.cameraOrthographicSize.z,
                                        asset.camera.cameraOrthographicSize.w,
                                        asset.camera.cameraNear,
                                        asset.camera.cameraFar );
  }

  // Setting camera parameters for 3D Scene
  // CameraActors should face in the negative Z direction, towards the other actors
  //viewQuaternion is the viewMatrix used by camera in DALI
  Quaternion viewQuaternion( Dali::ANGLE_180, Vector3::YAXIS );
  Quaternion camOrientation( asset.camera.cameraMatrix );
  camOrientation = camOrientation * viewQuaternion;
  camera3d.SetOrientation( camOrientation );
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

  m3dRoot.Add( mModel.GetActor() );

  Matrix matCube( mCameraOrientationInv * mCubeOrientation );
  matCube.Transpose();
  mModel.SetShaderUniform("uCubeMatrix" , matCube );
}

void Scene3dLauncher::ClearModel()
{
  mSkybox.Clear();
  mModel.Clear();
}

void Scene3dLauncher::CreateModel()
{
  UnparentAndReset( mErrorMessage );

  // Init Pbr actor
  InitPbrActor();


  // Initialise Main Actors
  InitActors();
  if( mAnimations.size() > 0)
  {
    PlayAnimation( mAnimations[0] );
  }

  SetUpEvents( mScripts );
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

void Scene3dLauncher::PlayAnimation( std::vector<Animation>& animationList )
{
  for(std::vector<Animation>::iterator it = animationList.begin(); it != animationList.end(); ++it)
  {
    (*it).Play();
  }
}

void Scene3dLauncher::ApplicationQuit()
{
  mApplication.Quit();
}

void Scene3dLauncher::OnNotification( const SceneLauncher::DataProvider::Notification& notification )
{
  for( const auto& item : mDataProviderObservers )
  {
    if( item.dataProviderId == notification.mType )
    {
      // Execute the script for this notification.

      for( Vector<DataProviderObserver::Index>::ConstIterator it = item.callbackIndices.Begin(), endIt = item.callbackIndices.End(); it != endIt; ++it )
      {
        const DataProviderObserver::Index& callbackIndex = *it;

        const SceneLauncher::DliLoader::Script& script = *( mScripts.begin() + callbackIndex.scriptIndex );

        const SceneLauncher::DliLoader::Event& event = *( script.events.begin() + callbackIndex.eventIndex );

        // Fetch the lua function
        mLua.FetchFunction( event.callback.c_str() );

        // Push the notification value as a parameter for the lua script.
        mLua.PushParameter( notification.mValue );

        // Execute the script.
        mLua.ExecuteFunction( 0u );
      }
    }
  }
}

// Entry point for Linux & Tizen applications
//
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv);

  Scene3dLauncher test( application );

  application.MainLoop();

  return 0;
}
