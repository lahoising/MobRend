cmake_minimum_required(VERSION 3.10)
project(MobRend)

set(MOBREND_USE_GLFW ON CACHE BOOL "Use GLFW windows")

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

# GLFW STUFF
if(${MOBREND_USE_GLFW})
    add_compile_definitions(MOBREND_GLFW_WINDOW)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    set(EXTERNAL_INC "${CMAKE_CURRENT_SOURCE_DIR}/external")
    add_subdirectory("${EXTERNAL_INC}/glfw")
    target_link_libraries(app glfw)
endif()