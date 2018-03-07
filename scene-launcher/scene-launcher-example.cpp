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
#include "application-resources.h"

namespace
{

const std::string SCENES_DIR( "scenes" );

const Vector3 CAMERA_DEFAULT_POSITION( 0.0f, 0.0f, 3.5f );

const float TEXT_AUTO_SCROLL_SPEED = 200.f;

#define MAKE_SHADER(A)#A

const char* VERTEX_SHADER_STR = MAKE_SHADER(
precision mediump float;

in vec3 aPosition;
in vec2 aTexCoord;

out vec2 vUV;

uniform vec3 uSize;
uniform mat4 uMvpMatrix;

void main()
{

  vec4 vPosition = vec4( aPosition * uSize, 1.0 );
  gl_Position = uMvpMatrix * vPosition;

  vUV = aTexCoord;
}
);

const char* FRAGMENT_SHADER_STR = MAKE_SHADER(
precision mediump float;

uniform sampler2D sAlbedo;

in vec2 vUV;

out vec4 FragColor;

void main()
{
  FragColor = texture( sAlbedo, vUV.st );
}
);

const std::string SHADER_VERSION("#version 300 es\n");
std::string VERTEX_SHADER = SHADER_VERSION + std::string( VERTEX_SHADER_STR );
std::string FRAGMENT_SHADER = SHADER_VERSION + std::string( FRAGMENT_SHADER_STR );

} // namespace

Scene3dLauncher::Scene3dLauncher( Application& application )
: mApplication( application ),
  mSceneFileParser(),
  mLua(),
  mLuaApplicationHelper( application, mLua ),
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
    mSceneFileParser.ReadModelFolder( ( ApplicationResources::Get().GetModelsPath() + SCENES_DIR ).c_str() );

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

  mDoubleTapTime = Timer::New(150);
  mDoubleTapTime.TickSignal().Connect( this, &Scene3dLauncher::OnDoubleTapTime );

  // Load lua scripts
  const std::string scriptDir( ApplicationResources::Get().GetLuaScriptsPath() );
  for( const auto& script : mScripts )
  {
    mLua.LoadScriptFile( ( scriptDir + script.url ).c_str() );
  }

  // Second stage initialization. It calls an OnConnect() function in Lua and does the KeyEventSignal connection to an OnKeyEvent() function in Lua.
  mLuaApplicationHelper.Initialize();

  // Load a skin.
  mSceneFileParser.ReadSkinFolder( ( ApplicationResources::Get().GetSkinsPath() ).c_str() );

  try
  {
    CreateSkin();
  }
  catch( DaliException& e )
  {
    // Nothing to do!
  }
  catch( ... )
  {
    DALI_LOG_ERROR( "Unknown error while loading %s\n", mSceneFileParser.GetSkinFile().c_str() );
  }
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

  Actor pbrModelActor = mModel.GetActor();
  m3dRoot.Add( pbrModelActor );

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
}

void Scene3dLauncher::CreateSkin()
{
  struct Vertex
  {
    Vector3 aPosition;
    Vector2 aTexCoord;
  };
  Vertex vertices[] =
  {
    { Vector3( -0.5f,-0.5f, 0.0f ), Vector2( 0.0f, 1.0f ) },
    { Vector3(  0.5f,-0.5f, 0.0f ), Vector2( 1.0f, 1.0f ) },
    { Vector3( -0.5f, 0.5f, 0.0f ), Vector2( 0.0f, 0.0f ) },
    { Vector3(  0.5f, 0.5f, 0.0f ), Vector2( 1.0f, 0.0f ) }
  };

  Property::Map property;
  property.Insert( "aPosition", Property::VECTOR3 );
  property.Insert( "aTexCoord", Property::VECTOR2 );

  PropertyBuffer vertexBuffer = PropertyBuffer::New( property );

  vertexBuffer.SetData( vertices, sizeof( vertices ) / sizeof( Vertex ) );

  Geometry geometry = Geometry::New();
  geometry.AddVertexBuffer( vertexBuffer );

  geometry.SetType( Geometry::TRIANGLE_STRIP );

  Shader shader = Shader::New( VERTEX_SHADER, FRAGMENT_SHADER );

  Renderer renderer = Renderer::New( geometry, shader );
  renderer.SetProperty( Renderer::Property::DEPTH_WRITE_MODE, DepthWriteMode::OFF );
  renderer.SetProperty( Renderer::Property::DEPTH_TEST_MODE, DepthTestMode::OFF );

  renderer.SetProperty( Renderer::Property::FACE_CULLING_MODE, FaceCullingMode::NONE );
  renderer.SetProperty( Renderer::Property::BLEND_MODE, BlendMode::ON );
  renderer.SetProperty( Renderer::Property::BLEND_FACTOR_SRC_ALPHA, BlendFactor::ONE_MINUS_DST_ALPHA );

  const SceneLauncher::Asset& asset = mSceneFileParser.GetAsset();
  PixelData pixelData = SyncImageLoader::Load( asset.skinFile );

  if( !pixelData )
  {
    throw Dali::DaliException( ASSERT_LOCATION, "Failed to load skin texture." );
  }

  Texture texture = Texture::New( TextureType::TEXTURE_2D, pixelData.GetPixelFormat(), pixelData.GetWidth(), pixelData.GetHeight() );
  texture.Upload( pixelData, 0, 0, 0, 0, pixelData.GetWidth(), pixelData.GetHeight() );

  TextureSet textureSet = TextureSet::New();
  textureSet.SetTexture( 0u , texture );

  renderer.SetTextures( textureSet );

  Actor actor = Actor::New();
  actor.AddRenderer( renderer );

  actor.SetAnchorPoint( AnchorPoint::CENTER );
  actor.SetParentOrigin( ParentOrigin::CENTER );
  actor.SetColor( Vector4::ONE );
  actor.SetPosition( Vector3::ZERO );

  actor.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS );

  mUiRoot.Add( actor );
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

// Entry point for Linux & Tizen applications
//
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv);

  Scene3dLauncher test( application );

  application.MainLoop();

  return 0;
}
