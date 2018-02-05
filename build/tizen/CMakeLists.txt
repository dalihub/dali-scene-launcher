CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(dali-scene-launcher C CXX)

SET(dali-scene-launcher_VERSION_MAJOR 1)
SET(dali-scene-launcher_VERSION_MINOR 0)

SET(ROOT_SRC_DIR ${CMAKE_SOURCE_DIR}/../..)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})

IF(DEFINED DALI_APP_DIR)
  SET(BINDIR ${DALI_APP_DIR}/bin)
ELSE()
  SET(BINDIR ${PREFIX}/bin)
ENDIF()

SET(DESKTOP_BUILD 0) 
IF(NOT DEFINED TIZEN_RESOURCES_DIR)
  # Define a resource folder for Desktop build.
  SET(APP_DATA_RES_DIR ${PREFIX}/share/com.samsung.dali-scene-launcher/resources)
  SET(DESKTOP_BUILD 1)
ENDIF()

MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )
IF(CMAKE_BUILD_TYPE MATCHES Debug)
  MESSAGE("Setting variable: DEBUG_ENABLED")
  SET(DEBUG_ENABLED "1")
ENDIF()

SET(IMAGES_DIR ${APP_DATA_RES_DIR}/images/)
SET(MODELS_DIR ${APP_DATA_RES_DIR}/models/)
SET(SHADERS_DIR ${APP_DATA_RES_DIR}/shaders/)
SET(LUA_SCRIPTS_DIR ${APP_DATA_RES_DIR}/lua-scripts/)

SET(SCENE_LAUNCHER_RESOURCE_DIR \\"${APP_DATA_RES_DIR}/\\")
SET(SCENE_LAUNCHER_IMAGE_DIR \\"${IMAGES_DIR}\\")
SET(SCENE_LAUNCHER_MODEL_DIR \\"${MODELS_DIR}\\")
SET(SCENE_LAUNCHER_SHADER_DIR \\"${SHADERS_DIR}\\")
SET(SCENE_LAUNCHER_LUA_SCRIPTS_DIR \\"${LUA_SCRIPTS_DIR}\\")

# Set required libraries specific for the tizen platform
IF( NOT ${DESKTOP_BUILD} )
  SET( TIZEN_PKG_LIST
         capi-appfw-application # Required to include app_common.h, used to retrieve the resources folder on the Tizen platform.
     )
ENDIF()

SET(PKG_LIST dali-core
             dali-adaptor
             dali-toolkit )

IF( ${DESKTOP_BUILD} )
SET(PKG_LIST ${PKG_LIST}
             lua5.2 )
ELSE()
SET(PKG_LIST ${PKG_LIST}
             lua )
ENDIF()

INCLUDE(FindPkgConfig)
pkg_check_modules(REQUIRED_PKGS REQUIRED ${TIZEN_PKG_LIST} ${PKG_LIST})

FOREACH(flag ${REQUIRED_PKGS_CFLAGS})
        SET(REQUIRED_CFLAGS "${REQUIRED_CFLAGS} ${flag}")
ENDFOREACH(flag)

IF( ${DESKTOP_BUILD} )
  SET( DEFAULT_UNIX_CFLAGS
         "-DSCENE_LAUNCHER_RESOURCE_DIR=${SCENE_LAUNCHER_RESOURCE_DIR} \
          -DSCENE_LAUNCHER_IMAGE_DIR=${SCENE_LAUNCHER_IMAGE_DIR} \
          -DSCENE_LAUNCHER_MODEL_DIR=${SCENE_LAUNCHER_MODEL_DIR} \
          -DSCENE_LAUNCHER_SHADER_DIR=${SCENE_LAUNCHER_SHADER_DIR} \
          -DSCENE_LAUNCHER_LUA_SCRIPTS_DIR=${SCENE_LAUNCHER_LUA_SCRIPTS_DIR}"
     ) # Used to retrieve the resources folder on a Default unix platform.
ENDIF()

SET(DALI_SCENE_LAUNCHER_CFLAGS "${DEFAULT_UNIX_CFLAGS} -fvisibility=hidden -DHIDE_DALI_INTERNALS")

###########################################################################

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${REQUIRED_CFLAGS} ${DALI_SCENE_LAUNCHER_CFLAGS} -Werror -Wall -fPIE -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

INCLUDE_DIRECTORIES(${ROOT_SRC_DIR})

ADD_SUBDIRECTORY(scene-launcher)
