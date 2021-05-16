cmake_minimum_required(VERSION 3.10)
project(MobRend)

file(
    GLOB SRC_FILES
    "src/*.cpp"
)

set(BASE_INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
file(
    GLOB INCLUDE_FILES
    "${BASE_INC_PATH}/*.h"
)
    
add_executable(app ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(
    app PUBLIC 
    "${BASE_INC_PATH}"
)

# GLFW STUFF
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(EXTERNAL_INC "${CMAKE_CURRENT_SOURCE_DIR}/external")
add_subdirectory("${EXTERNAL_INC}/glfw")
target_link_libraries(app glfw)