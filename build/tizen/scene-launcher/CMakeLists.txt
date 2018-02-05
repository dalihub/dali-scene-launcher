################################################################################
# Lua
################################################################################

SET(LUA_SRC_DIR ${ROOT_SRC_DIR}/third-party/lua-5.3.4/src)

file( GLOB LUA_SRCS
      ${LUA_SRC_DIR}/*.h
      ${LUA_SRC_DIR}/lapi.c
      ${LUA_SRC_DIR}/lauxlib.c
      ${LUA_SRC_DIR}/lbaselib.c
      ${LUA_SRC_DIR}/lbitlib.c
      ${LUA_SRC_DIR}/lcode.c
      ${LUA_SRC_DIR}/lcorolib.c
      ${LUA_SRC_DIR}/lctype.c
      ${LUA_SRC_DIR}/ldblib.c
      ${LUA_SRC_DIR}/ldebug.c
      ${LUA_SRC_DIR}/ldo.c
      ${LUA_SRC_DIR}/ldump.c
      ${LUA_SRC_DIR}/lfunc.c
      ${LUA_SRC_DIR}/lgc.c
      ${LUA_SRC_DIR}/linit.c
      ${LUA_SRC_DIR}/liolib.c
      ${LUA_SRC_DIR}/llex.c
      ${LUA_SRC_DIR}/lmathlib.c
      ${LUA_SRC_DIR}/lmem.c
      ${LUA_SRC_DIR}/loadlib.c
      ${LUA_SRC_DIR}/lobject.c
      ${LUA_SRC_DIR}/lopcodes.c
      ${LUA_SRC_DIR}/loslib.c
      ${LUA_SRC_DIR}/lparser.c
      ${LUA_SRC_DIR}/lstate.c
      ${LUA_SRC_DIR}/lstring.c
      ${LUA_SRC_DIR}/lstrlib.c
      ${LUA_SRC_DIR}/ltable.c
      ${LUA_SRC_DIR}/ltablib.c
      ${LUA_SRC_DIR}/ltm.c
      ${LUA_SRC_DIR}/lundump.c
      ${LUA_SRC_DIR}/lutf8lib.c
      ${LUA_SRC_DIR}/lvm.c
      ${LUA_SRC_DIR}/lzio.c )

SET(LUA_COMPILE_FLAGS "-DLUA_USE_LINUX")
set_source_files_properties(${LUA_SRCS} PROPERTIES COMPILE_FLAGS ${LUA_COMPILE_FLAGS})

SET(LUA_LINK_FLAGS "-Wl,-E -ldl -lreadline")

################################################################################
# scene-launcher
################################################################################

SET(SCENE_LAUNCHER_SRC_DIR ${ROOT_SRC_DIR}/scene-launcher)

AUX_SOURCE_DIRECTORY(${SCENE_LAUNCHER_SRC_DIR} SCENE_LAUNCHER_SRCS)

set_source_files_properties(${SCENE_LAUNCHER_SRCS} PROPERTIES COMPILE_FLAGS "-std=c++11" )

ADD_EXECUTABLE(${PROJECT_NAME} ${SCENE_LAUNCHER_SRCS} ${LUA_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${REQUIRED_PKGS_LDFLAGS} -pie ${LUA_LINK_FLAGS})
include_directories(${LUA_SRC_DIR})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
