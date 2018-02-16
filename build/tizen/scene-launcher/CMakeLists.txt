SET(SCENE_LAUNCHER_SRC_DIR ${ROOT_SRC_DIR}/scene-launcher)

SET( COMMON_SRCS
       ${SCENE_LAUNCHER_SRC_DIR}/dli-loader.cpp
       ${SCENE_LAUNCHER_SRC_DIR}/ktx-loader.cpp
       ${SCENE_LAUNCHER_SRC_DIR}/model-pbr.cpp
       ${SCENE_LAUNCHER_SRC_DIR}/model-skybox.cpp
       ${SCENE_LAUNCHER_SRC_DIR}/scene-file-parser.cpp
       ${SCENE_LAUNCHER_SRC_DIR}/scene-launcher-example.cpp
       ${SCENE_LAUNCHER_SRC_DIR}/utils.cpp
   )

IF( DEFINED TIZEN_RESOURCES_DIR )
  SET( PLATFORM_SRCS
       ${SCENE_LAUNCHER_SRC_DIR}/application-resources-tizen.cpp
     )
ELSE()
  SET( PLATFORM_SRCS
       ${SCENE_LAUNCHER_SRC_DIR}/application-resources-default-unix.cpp
     )
ENDIF()

ADD_EXECUTABLE(${PROJECT_NAME} ${PLATFORM_SRCS} ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${REQUIRED_PKGS_LDFLAGS} -pie)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})

