cmake_minimum_required(VERSION 3.28)

project(idros LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SRC_PATH			"${CMAKE_SOURCE_DIR}/src")
set(LIB_INCLUDE_PATH	"${CMAKE_SOURCE_DIR}/ext/includes")
set(LIB_PATH			"${CMAKE_SOURCE_DIR}/ext/libs")

add_library(idros SHARED 
    "src/entry.cpp"
    "src/engine.cpp"
    "src/ecs/ecs.cpp"
    "src/ecs/entity_manager.cpp"
    "src/scene_manager.cpp"
    "src/resource_manager.cpp"

    "src/time.cpp"

    "src/sprite_animation.cpp"

    "src/serialization.cpp"

    "src/event_callback_handler.cpp"
    "src/callbacks/on_frame_start.cpp"
    "src/callbacks/on_frame_end.cpp"
)

target_include_directories(idros PRIVATE ${SRC_PATH} ${LIB_INCLUDE_PATH} ${LIB_INCLUDE_PATH}/pal ${LIB_INCLUDE_PATH}/tomlc17)

add_library(idros_pal STATIC IMPORTED)
set_target_properties(idros_pal PROPERTIES IMPORTED_LOCATION "${LIB_PATH}/libidros_pal.a")
target_link_libraries(idros PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,idros_pal>")
add_subdirectory(ext)

if (WIN32)
	if (MSVC)
		target_link_libraries(idros PRIVATE xinput.lib xaudio2_9.lib winmm.lib opengl32.lib)
	else()
		target_link_libraries(idros PRIVATE -lxinput -lxaudio2_9 -lwinmm -lopengl32)
	endif()
endif()

# Copies over necessary files to the test app
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
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SRC_PATH}/entry.h"
    "${SRC_PATH}/idros.h"
    "${SRC_PATH}/igame.h"
    "${SRC_PATH}/time.hpp"
    "${SRC_PATH}/sprite_animation.h"

    "${SRC_PATH}/ecs/ecs.h"
    "${SRC_PATH}/ecs/ecs.inl"
    "${SRC_PATH}/ecs/entity.h"
    "${SRC_PATH}/ecs/entity_manager.h"
    "${SRC_PATH}/ecs/component_manager.h"
    "${SRC_PATH}/ecs/component_manager.inl"
    "${SRC_PATH}/ecs/component_set.h"
    "${SRC_PATH}/ecs/component_set.inl"
    
    "${SRC_PATH}/event_callback_handler.h"
    "${SRC_PATH}/scene.h"
    "${SRC_PATH}/scene_manager.h"
    "${SRC_PATH}/scene_manager.inl"
    "${SRC_PATH}/resource_manager.h"

    "${SRC_PATH}/serialization.h"
    "C:\\Users\\Cameron\\Desktop\\idros-testapp\\ext\\includes\\idros"
)
add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${LIB_INCLUDE_PATH}/pal"
    "C:\\Users\\Cameron\\Desktop\\idros-testapp\\ext\\includes\\idros\\pal"
)
#