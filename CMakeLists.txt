cmake_minimum_required(VERSION 3.10)
project(MobRend)

set(MOBREND_USE_GLFW ON CACHE BOOL "Use GLFW windows")
set(MOBREND_USE_OPENGL ON CACHE BOOL "Use OpenGL for rendering")

file(
    GLOB SRC_FILES
    "src/*.cpp"
    "src/mr_platform/*.cpp"
)

set(BASE_INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
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

set(EXTERNAL_INC "${CMAKE_CURRENT_SOURCE_DIR}/external")

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
endif()

# GLFW STUFF
if(${MOBREND_USE_GLFW})
    add_compile_definitions(MOBREND_GLFW_WINDOW)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory("${EXTERNAL_INC}/glfw")
    target_link_libraries(app glfw)
endif()