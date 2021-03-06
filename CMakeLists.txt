cmake_minimum_required(VERSION 3.10)
project(MobRend)

set(MOBREND_LIB_TYPE SHARED CACHE STRING "Build as Shared or Static library")
set(MOBREND_ASSETS_DIR "${CMAKE_SOURCE_DIR}" CACHE PATH "Assets will be loaded relative to this path")
set(MOBREND_BACKEND OPENGL CACHE STRING "OPENGL or VULKAN")

file(
	GLOB_RECURSE src_files
	"${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp"
)

set(inc_dirs "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(
	GLOB inc_files
	"${inc_dirs}/**.h"
)

set(EXTERNAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(link_libs)
set(definitions)

set(MOBREND_USE_GLFW ON CACHE BOOL "Use GLFW3")
if(${MOBREND_USE_GLFW})
	add_subdirectory("${EXTERNAL_PATH}/glfw")
	set(link_libs ${link_libs} glfw)
	set(definitions MOBREND_GLFW_WINDOW)
endif()

# glm
add_subdirectory("${EXTERNAL_PATH}/glm")
set(link_libs ${link_libs} glm)

# mr logger
add_subdirectory("${EXTERNAL_PATH}/mobrend_logger")
set(link_libs ${link_libs} mr_logger)

# stb image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${EXTERNAL_PATH}/stb_image")
set(link_libs ${link_libs} stb)

# assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${EXTERNAL_PATH}/assimp")
set(link_libs ${link_libs} assimp)

# glad
if(MOBREND_BACKEND STREQUAL OPENGL)
	set(OpenGL_GL_PREFERENCE "GLVND")
	find_package(OpenGL REQUIRED)

	set(GLAD_INC_DIR "${EXTERNAL_PATH}/glad/gl/include")
	file(GLOB_RECURSE GLAD_SRC "${GLAD_INC_DIR}/**.h" "${EXTERNAL_PATH}/glad/gl/src/**.c")
	add_library(glad_gl ${MOBREND_LIB_TYPE} ${GLAD_SRC})
	target_include_directories(glad_gl PUBLIC ${GLAD_INC_DIR})
	target_link_libraries(glad_gl PUBLIC OpenGL::GL)

	# target_link_libraries(imgui PUBLIC glad_gl)
	set(link_libs ${link_libs} glad_gl ${CMAKE_DL_LIBS})
endif()

# imgui
file(GLOB imgui_src "${EXTERNAL_PATH}/imgui/**.cpp" "${EXTERNAL_PATH}/imgui/**.h")
if(${MOBREND_USE_GLFW})
	set(imgui_src ${imgui_src} "${EXTERNAL_PATH}/imgui/backends/imgui_impl_glfw.cpp")
	set(imgui_src ${imgui_src} "${EXTERNAL_PATH}/imgui/backends/imgui_impl_glfw.h")
endif()
set(imgui_src ${imgui_src} "${EXTERNAL_PATH}/imgui/backends/imgui_impl_opengl3.cpp")
set(imgui_src ${imgui_src} "${EXTERNAL_PATH}/imgui/backends/imgui_impl_opengl3.h")

add_library(imgui ${MOBREND_LIB_TYPE} ${imgui_src})
target_include_directories(imgui PUBLIC "${EXTERNAL_PATH}/imgui" "${EXTERNAL_PATH}/imgui/backends")
target_link_libraries(imgui PUBLIC glfw glad_gl)

set(link_libs ${link_libs} imgui)

# spirv
if(MOBREND_LIB_TYPE STREQUAL SHARED)
	option(SPIRV_CROSS_FORCE_PIC "" ON)
	message("spirv forced to PIC")
endif()
add_subdirectory("${EXTERNAL_PATH}/spirv-cross")
set(link_libs ${link_libs} spirv-cross-glsl)

# mobrend
set(BIN_NAME mobrend)
add_library(${BIN_NAME} ${MOBREND_LIB_TYPE} ${src_files} ${inc_files})
target_include_directories(${BIN_NAME} PUBLIC ${inc_dirs})
target_link_libraries(${BIN_NAME} PUBLIC ${link_libs})

target_compile_definitions(${BIN_NAME} PUBLIC MOBREND_ASSETS_RELATIVE_PATH="${MOBREND_ASSETS_DIR}/" ${definitions})
if(MOBREND_BACKEND STREQUAL OPENGL)
	target_compile_definitions(${BIN_NAME} PUBLIC MOBREND_GL_RENDERING)
endif()

if(MOBREND_LIB_TYPE STREQUAL SHARED)
	set_target_properties(${BIN_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# glslang
set(MOBREND_BUILD_GLSLANG ON CACHE BOOL "Build glslang, used to convert glsl shaders to SPIR-V")
if(${MOBREND_BUILD_GLSLANG})
	add_subdirectory("${EXTERNAL_PATH}/glslang")
endif()

# compile shaders command
set(MOBREND_SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders" CACHE PATH "Directories holding relevant shaders")
set(MOBREND_PY_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts")

set(resource_dirs)
foreach(RESOURCE_DIR ${MOBREND_SHADERS_DIR})
	set(resource_dirs "${resource_dirs} ${RESOURCE_DIR}")
endforeach()
add_custom_target(
	compile_shaders
	ALL
	${PYTHON_EXECUTABLE} "${MOBREND_PY_SCRIPTS_DIR}/compile_shaders.py" $<TARGET_FILE:glslangValidator> ${MOBREND_SHADERS_DIR})

# examples
set(MOBREND_BUILD_EXAMPLES ON CACHE BOOL "Build mobrend examples")

if(${MOBREND_BUILD_EXAMPLES})
	file(GLOB examples "${CMAKE_CURRENT_SOURCE_DIR}/examples/**.cpp")
	foreach(f ${examples})
		get_filename_component(sample "${f}" NAME_WE)
		add_executable(${sample} "${f}")
		target_link_libraries(${sample} PUBLIC ${BIN_NAME})
	endforeach()
endif()
