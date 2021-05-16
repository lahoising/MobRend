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
target_include_directories(app PUBLIC "${BASE_INC_PATH}")