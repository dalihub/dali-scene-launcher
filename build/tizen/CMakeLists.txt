cmake_minimum_required(VERSION 2.6)
project("dali-scene-launcher" CXX)

set(scene_launcher_prj_name "dali-scene-launcher")

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions("-DDEBUG_ENABLED")
endif()

foreach(flag ${PKGS_CFLAGS})
	set(extra_flags "${extra_flags} ${flag}")
endforeach(flag)

set(prj_cxx_std c++17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(extra_flags "${extra_flags} -fPIE -std=${prj_cxx_std}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(extra_flags "${extra_flags} -fPIE -std=${prj_cxx_std}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(extra_flags "${extra_flags} /std:${prj_cxx_std} /vmg /D_USE_MATH_DEFINES /D_CRT_SECURE_NO_WARNINGS /MP /GS /Oi /GL /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${extra_flags}")

set(prefix "${CMAKE_INSTALL_PREFIX}/")

set(scene_launcher_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../")

set(scene_launcher_src_dir "${scene_launcher_dir}src/")
file(GLOB scene_launcher_src_files "${scene_launcher_src_dir}*.cpp")

set(scene_launcher_include_dir "${scene_launcher_dir}include/")
set(prefix_include_dir "${prefix}include/")
include_directories(${scene_launcher_include_dir}
	"${prefix_include_dir}"
)

add_executable(${scene_launcher_prj_name} ${scene_launcher_src_files})

set(prefix_lib_dir "${prefix}lib/")
find_library(dli dli
	PATHS ${prefix_lib_dir}
)
find_library(dali2-core dali2-core
	PATHS ${prefix_lib_dir}
)
find_library(dali2-adaptor dali2-adaptor
	PATHS ${prefix_lib_dir}
)
find_library(dali2-toolkit dali2-toolkit
	PATHS ${prefix_lib_dir}
)

target_link_libraries(${scene_launcher_prj_name}
	${dli}
	${dali2-core}
	${dali2-adaptor}
	${dali2-toolkit}
	-pie
)


if(DEFINED DALI_APP_DIR)
	set(bin_dir ${DALI_APP_DIR})
else()
	set(bin_dir "${prefix}bin/")
endif()

if(DEFINED DALI_APP_RES_DIR)
	set(res_dir ${DALI_APP_RES_DIR})
else()
	set(res_dir "${prefix}share/com.samsung.dali-scene-launcher/")
endif()

install(TARGETS ${scene_launcher_prj_name}
	DESTINATION ${bin_dir}
)

install(DIRECTORY "${scene_launcher_dir}res/"
        DESTINATION ${res_dir}
)
