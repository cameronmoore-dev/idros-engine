cmake_minimum_required(VERSION 3.28)

project(idros LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_PATH			"${CMAKE_SOURCE_DIR}/src")
set(LIB_INCLUDE_PATH	"${CMAKE_SOURCE_DIR}/ext/includes")
set(LIB_PATH			"${CMAKE_SOURCE_DIR}/ext/libs")

add_library(idros SHARED EXCLUDE_FROM_ALL 
    "src/entry.cpp"
    "src/engine.cpp"
)

target_include_directories(idros PUBLIC ${SRC_PATH} ${LIB_INCLUDE_PATH} ${LIB_INCLUDE_PATH}/stryde)
target_link_libraries(idros PRIVATE ${LIB_PATH}/libstryde.dll.a)


add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE_DIR:idros>/libidros.dll"
    "C:\\Users\\Cameron\\Desktop\\idros-testapp\\build"
)
add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE_DIR:idros>/libidros.dll.a"
    "C:\\Users\\Cameron\\Desktop\\idros-testapp\\ext\\libs"
)
add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "C:\\Users\\Cameron\\Desktop\\idros-testapp\\ext\\includes\\idros"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SRC_PATH}/entry.h"
    "${SRC_PATH}/igame.h"
    "${SRC_PATH}/scene.h"
    "${SRC_PATH}/scene_manager.h"
    "C:\\Users\\Cameron\\Desktop\\idros-testapp\\ext\\includes\\idros"
)