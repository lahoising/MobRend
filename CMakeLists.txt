cmake_minimum_required(VERSION 3.10)
project(MobRend)

set(MOBREND_USE_GLFW ON CACHE BOOL "Use GLFW windows")
set(MOBREND_USE_OPENGL ON CACHE BOOL "Use OpenGL for rendering")

set(BASE_INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
set(EXTERNAL_INC "${CMAKE_CURRENT_SOURCE_DIR}/external")

file(
    GLOB SRC_FILES
    "src/*.cpp"
    "src/mr_platform/*.cpp"
)

file(
    GLOB INCLUDE_FILES
    "${BASE_INC_PATH}/*.h"
    "${BASE_INC_PATH}/mr_platform/*.h"
)
    
add_executable(app ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(
    app PUBLIC 
    "${BASE_INC_PATH}"
)

set(IMGUI_PATH "${EXTERNAL_INC}/imgui")
set(IMGUI_BACKEND "${IMGUI_PATH}/backends/imgui_impl_")
file(
    GLOB IMGUI_SRC
    "${IMGUI_PATH}/*.cpp"
    "${IMGUI_PATH}/*.h"
)

#OpenGL STUFF
if(${MOBREND_USE_OPENGL})
    add_compile_definitions(MOBREND_GL_RENDERING)

    find_package(OpenGL REQUIRED)
    target_link_libraries(app OpenGL::GL)
    
    set(GLAD_GL_PATH "${EXTERNAL_INC}/glad/gl")
    file(
        GLOB_RECURSE GLAD_GL_SRC
        "${GLAD_GL_PATH}/src/*.c"
        "${GLAD_GL_PATH}/include/**.h"
    )
    message("glad source files: ${GLAD_GL_SRC}")
    
    add_library(glad_gl ${GLAD_GL_SRC})
    target_include_directories(
        glad_gl PUBLIC 
        "${GLAD_GL_PATH}/include"
    )
    target_link_libraries(app glad_gl)

    set(IMGUI_SRC ${IMGUI_SRC} "${IMGUI_BACKEND}opengl3.cpp" "${IMGUI_BACKEND}opengl3.h")
endif()

# GLFW STUFF
if(${MOBREND_USE_GLFW})
    add_compile_definitions(MOBREND_GLFW_WINDOW)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory("${EXTERNAL_INC}/glfw")
    target_link_libraries(app glfw)

    set(IMGUI_SRC ${IMGUI_SRC} "${IMGUI_BACKEND}glfw.h" "${IMGUI_BACKEND}glfw.cpp")
endif()

# ImGui
add_library(imgui ${IMGUI_SRC})

target_include_directories(
    imgui PUBLIC
    ${IMGUI_PATH} 
    "${IMGUI_PATH}/backends"
)
if(${MOBREND_USE_GLFW})
    target_link_libraries(imgui glfw)
endif()
if(${MOBREND_USE_OPENGL})
    target_link_libraries(imgui glad_gl)
endif()

target_link_libraries(app imgui)

# glm
add_subdirectory("${EXTERNAL_INC}/glm")
target_link_libraries(app glm)

# mr logger
add_subdirectory("${EXTERNAL_INC}/mobrend_logger")
target_link_libraries(app mr_logger)

# stb
set(MR_STB_PATH "${EXTERNAL_INC}/stb_image")
add_library(stb INTERFACE)
target_sources(stb INTERFACE "${MR_STB_PATH}/stb_image.h")
target_include_directories(stb INTERFACE "${MR_STB_PATH}")
target_link_libraries(app stb)

# assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("${EXTERNAL_INC}/assimp")
target_link_libraries(app assimp)