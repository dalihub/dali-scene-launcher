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
#include <dali/public-api/common/vector-wrapper.h>
#include <dali-toolkit/dali-toolkit.h>

#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>

// INTERNAL INCLUDES
#include "ktx-loader.h"
#include "model-pbr.h"
#include "model-skybox.h"
#include "scene-file-parser.h"

using namespace Dali;
using namespace Toolkit;

namespace
{

struct MaterialTextures
{
  MaterialTextures( const char* albedoMetalness, const char* normalRoughness )
  : albedoMetalness( albedoMetalness ),
    normalRoughness( normalRoughness )
  {}

  ~MaterialTextures()
  {}

  const char* albedoMetalness;
  const char* normalRoughness;
};

const char* MODEL_DIR_URL = SCENE_LAUNCHER_MODEL_DIR "scenes";

const std::string ASSET_MODEL_DIR = SCENE_LAUNCHER_MODEL_DIR;
const std::string ASSET_TEXTURE_DIR = SCENE_LAUNCHER_IMAGE_DIR;

const float CAMERA_DEFAULT_FOV(    60.0f );
const float CAMERA_DEFAULT_NEAR(    0.1f );
const float CAMERA_DEFAULT_FAR(  1000.0f );
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

class SceneLauncher : public ConnectionTracker
{
public:

  SceneLauncher( Application& application )
  : mApplication( application ),
    mSceneParser(),
    mDoubleTapTime(),
    mViewQuaternion(Dali::ANGLE_180, Vector3::YAXIS),
    mModelOrientation(),
    mCubeOrientation(),
    mZoomLevel( 1.f ),
    mDoubleTap( false ),
    mRotateEnvironment(false)
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect( this, &SceneLauncher::Create );
  }

  ~SceneLauncher()
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
    stage.GetRootLayer().TouchSignal().Connect( this, &SceneLauncher::OnTouch );

    // Respond to key events
    stage.KeyEventSignal().Connect( this, &SceneLauncher::OnKeyEvent );

    mDoubleTapTime = Timer::New(150);
    mDoubleTapTime.TickSignal().Connect( this, &SceneLauncher::OnDoubleTapTime );
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
            cam.SetPosition( CAMERA_DEFAULT_POSITION * ( mZoomLevel + 0.1f ) );
          }

          Actor skyboxActor = mSkybox.GetActor();
          if( skyboxActor )
          {
            skyboxActor.SetPosition( CAMERA_DEFAULT_POSITION * ( mZoomLevel + 0.1f ) );
          }

          mStartTouch = point;
        }
        //If the touch is not processed above, then change the model orientation
        else
        {
          const float angle1 = ( ( mPointZ.y - point.y ) / scaleY );
          const float angle2 = ( ( mPointZ.x - point.x ) / scaleX );

          if(mRotateEnvironment)
          {
            Quaternion cubeOrientation = mCubeOrientation;
            cubeOrientation.Conjugate();
            const Quaternion pitchRot( Radian( Degree( angle1 * -200.0f ) ), cubeOrientation.Rotate( Vector3::XAXIS ) );
            const Quaternion yawRot(Radian(Degree(angle2 * -200.0f)), cubeOrientation.Rotate( Vector3::YAXIS ) );

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
            if( mShader )
            {
              mShader.SetProperty( mShader.GetPropertyIndex( "uCubeMatrix" ), matCube );
            }
          }
          else
          {
            Quaternion modelOrientation = mModelOrientation;
            modelOrientation.Conjugate();
            const Quaternion pitchRot( Radian( Degree( angle1 * -200.0f ) ), modelOrientation.Rotate( Vector3::XAXIS ) );
            const Quaternion yawRot(Radian(Degree(angle2 * -200.0f)), modelOrientation.Rotate( Vector3::YAXIS ) );

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
    const PbrDemo::Asset& asset = mSceneParser.GetAsset();

    mModel.Init( mShader, ASSET_MODEL_DIR + asset.model, Vector3::ZERO, asset.modelScaleFactor );
    mModel.GetActor().SetOrientation( mModelOrientation );

    Stage::GetCurrent().Add( mModel.GetActor() );
  }

  /**
   * @brief Creates scene actors and setup camera parameters
   */
  void InitActors()
  {
    Stage stage = Stage::GetCurrent();

    const PbrDemo::Asset& asset = mSceneParser.GetAsset();

    mSkybox.InitTexture( mSpecularTexture );
    mSkybox.Init();
    Quaternion ViewQuaternion(mViewQuaternion);
    ViewQuaternion.Conjugate();
    mSkybox.GetActor().SetOrientation( ViewQuaternion * mCubeOrientation );

    // Setting camera parameters for 3D Scene
    mSkybox.GetActor().SetPosition( CAMERA_DEFAULT_POSITION );
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
  }

  /**
   * @brief Creates a shader using file path
   */
  void CreateModelShader()
  {
    const PbrDemo::Asset& asset = mSceneParser.GetAsset();

    mShader = LoadShaders( asset.vertexShader, asset.fragmentShader );

    if( !mShader )
    {
      throw DaliException( ASSERT_LOCATION, "Failed to load shader." );
    }

    // Initialise shader uniforms
    // Level 8 because the environment texture has 6 levels plus 2 are missing (2x2 and 1x1)
    mCubeOrientation = mViewQuaternion * mModelOrientation;
    Matrix matCube( mCubeOrientation );
    matCube.Transpose();
    mShader.RegisterProperty( "uMaxLOD", 8.0f );
    mShader.RegisterProperty( "uCubeMatrix" , matCube );
  }

  /**
    * @brief Loads diffuse and specular textures
    */
  void CreateEnvironmentTextures()
  {
    const PbrDemo::Asset& asset = mSceneParser.GetAsset();

    // This texture should have 6 faces and only one mipmap
    PbrDemo::CubeData diffuse;
    bool result = PbrDemo::LoadCubeMapFromKtxFile( ASSET_TEXTURE_DIR + asset.cubeDiffuse, diffuse );

    if( !result )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load cubemap textures." );
    }

    mDiffuseTexture = Texture::New( TextureType::TEXTURE_CUBE, diffuse.img[0][0].GetPixelFormat(), diffuse.img[0][0].GetWidth(), diffuse.img[0][0].GetHeight() );
    for( unsigned int midmapLevel = 0; midmapLevel < diffuse.img[0].size(); ++midmapLevel )
    {
      for( unsigned int i = 0; i < diffuse.img.size(); ++i )
      {
        mDiffuseTexture.Upload( diffuse.img[i][midmapLevel], CubeMapLayer::POSITIVE_X + i, midmapLevel, 0, 0, diffuse.img[i][midmapLevel].GetWidth(), diffuse.img[i][midmapLevel].GetHeight() );
      }
    }

    // This texture should have 6 faces and 6 mipmaps
    PbrDemo::CubeData specular;
    result = PbrDemo::LoadCubeMapFromKtxFile( ASSET_TEXTURE_DIR + asset.cubeSpecular, specular);

    if( !result )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load cubemap textures." );
    }

    mSpecularTexture = Texture::New( TextureType::TEXTURE_CUBE, specular.img[0][0].GetPixelFormat(), specular.img[0][0].GetWidth(), specular.img[0][0].GetHeight() );
    for( unsigned int midmapLevel = 0; midmapLevel < specular.img[0].size(); ++midmapLevel )
    {
      for( unsigned int i = 0; i < specular.img.size(); ++i )
      {
        mSpecularTexture.Upload( specular.img[i][midmapLevel], CubeMapLayer::POSITIVE_X + i, midmapLevel, 0, 0, specular.img[i][midmapLevel].GetWidth(), specular.img[i][midmapLevel].GetHeight() );
      }
    }
  }

  /*
   * @brief Create texture for the model
   */
  void CreateModelTexture()
  {
    const PbrDemo::Asset& asset = mSceneParser.GetAsset();

    PixelData modelPixelData;
    Texture textureAlbedoMetal;
    Texture textureNormalRough;

    modelPixelData = SyncImageLoader::Load( ASSET_TEXTURE_DIR + asset.albedoMetalness );

    if( !modelPixelData )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load albedo-metalness texture." );
    }

    textureAlbedoMetal = Texture::New( TextureType::TEXTURE_2D, modelPixelData.GetPixelFormat(), modelPixelData.GetWidth(), modelPixelData.GetHeight() );
    textureAlbedoMetal.Upload( modelPixelData, 0, 0, 0, 0, modelPixelData.GetWidth(), modelPixelData.GetHeight() );
    textureAlbedoMetal.GenerateMipmaps();

    modelPixelData = SyncImageLoader::Load( ASSET_TEXTURE_DIR + asset.normalRoughness );

    if( !modelPixelData )
    {
      throw Dali::DaliException( ASSERT_LOCATION, "Failed to load normal-roughness texture." );
    }

    textureNormalRough = Texture::New( TextureType::TEXTURE_2D, modelPixelData.GetPixelFormat(), modelPixelData.GetWidth(), modelPixelData.GetHeight() );
    textureNormalRough.Upload( modelPixelData, 0, 0, 0, 0, modelPixelData.GetWidth(), modelPixelData.GetHeight() );
    textureNormalRough.GenerateMipmaps();

    mModel.InitPbrTexture( textureAlbedoMetal, textureNormalRough, mDiffuseTexture, mSpecularTexture );
  }

  /*
   * @brief Clear resources
   */
  void ClearModel()
  {
    mShader.Reset();
    mDiffuseTexture.Reset();
    mSpecularTexture.Reset();
    mSkybox.Clear();
    mModel.Clear();
  }

  /*
   * @brief loads model and initialise textures.
   */
  void CreateModel()
  {
    UnparentAndReset( mErrorMessage );

    // Create shader
    CreateModelShader();

    // Creates the diffuse texture and the specular texture
    CreateEnvironmentTextures();

    // Initialise Main Actors
    InitActors();

    // Create texture for the pbr model
    CreateModelTexture();

    // Init Pbr actor
    InitPbrActor();
  }

  /**
  * @brief Load a shader source file
  * @param[in] The path of the source file
  * @param[out] The contents of file
  * @return True if the source was read successfully
  */
  bool LoadShaderCode( const std::string& fullpath, std::vector<char>& output )
  {
    FILE* f = fopen( fullpath.c_str(), "rb" );

    if( NULL == f )
    {
      return false;
    }

    fseek( f, 0, SEEK_END );
    size_t size = ftell( f );
    fseek( f, 0, SEEK_SET );
    output.resize( size + 1 );
    std::fill( output.begin(), output.end(), 0 );
    ssize_t result = fread( output.data(), size, 1, f );
    fclose( f );
    return ( result >= 0 );
  }

  /**
  * @brief Load vertex and fragment shader source
  * @param[in] shaderVertexFileName is the filepath of Vertex shader
  * @param[in] shaderFragFileName is the filepath of Fragment shader
  * @return the Dali::Shader object
  */
  Shader LoadShaders( const std::string& shaderVertexFileName, const std::string& shaderFragFileName )
  {
    Shader shader;
    std::vector<char> bufV, bufF;

    if( LoadShaderCode( shaderVertexFileName.c_str(), bufV ) )
    {
      if( LoadShaderCode( shaderFragFileName.c_str(), bufF ) )
      {
        shader = Shader::New( bufV.data() , bufF.data() );
      }
    }
    return shader;
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
    colorMap.Add( Visual::Property::TYPE, Visual::COLOR).
      Add( ColorVisual::Property::MIX_COLOR, Color::BLACK );
    mErrorMessage.SetProperty( Control::Property::BACKGROUND, colorMap );

    mErrorMessage.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH );
    mErrorMessage.SetParentOrigin( ParentOrigin::CENTER );
    mErrorMessage.SetAnchorPoint( AnchorPoint::CENTER );

    Stage::GetCurrent().Add( mErrorMessage );
  }

private:
  Application& mApplication;

  PbrDemo::SceneFileParser mSceneParser;

  TextLabel mErrorMessage;
  Shader mShader;
  Timer mDoubleTapTime;

  Texture mDiffuseTexture;
  Texture mSpecularTexture;
  ModelSkybox mSkybox;
  PbrDemo::ModelPbr mModel;

  Vector2 mPointZ;
  Vector2 mStartTouch;

  //Quaternion of ViewMatrix
  Quaternion mViewQuaternion;
  Quaternion mModelOrientation;
  Quaternion mCubeOrientation;

  float mZoomLevel;
  bool mDoubleTap;
  bool mRotateEnvironment;
};

// Entry point for Linux & Tizen applications
//
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv);

  SceneLauncher test( application );

  application.MainLoop();

  return 0;
}
