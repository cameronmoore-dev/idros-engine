cmake_minimum_required(VERSION 3.28)

project(idros LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_INCLUDE_PATH	"${CMAKE_SOURCE_DIR}/ext/includes")
set(LIB_PATH			"${CMAKE_SOURCE_DIR}/ext/libs")
set(RENDER_BACKEND "OpenGL" CACHE STRING "Idros's rendering backend")

set(APP_NAME "idros-testapp")
set(APP_DIR "C:\\Users\\Cameron\\Desktop\\${APP_NAME}")

add_library(idros SHARED
    "src/core/clock.cpp"
    "src/core/math.cpp"
    "src/core/audio/audio_engine.cpp"
    "src/core/audio/sound.cpp"
    "src/core/audio/music.cpp"
    "src/core/graphics/image.cpp"
    "src/core/window/input.cpp"
    "src/core/window/window.cpp"

    "src/entry.cpp"
    "src/engine.cpp"
    "src/enginecontext.cpp"
    "src/debug.cpp"
    "src/time.cpp"
    "src/random.cpp"
    "src/scene_manager.cpp"
    "src/resource_manager.cpp"
    "src/sprite_animation.cpp"
    "src/serialization.cpp"
    "src/ecs/ecs.cpp"
    "src/ecs/entity_manager.cpp"
    "src/ecs/systems/sprite_animation_system.cpp"
    "src/ecs/systems/pathfindingsystem.cpp"
    "src/utils/sprite_utils.cpp"
    "src/utils/physicsutils.cpp"
    "src/utils/mouseutils.cpp"
    "src/utils/textutils.cpp"
    "src/utils/uiutils.cpp"
    "src/utils/tagutils.cpp"
)

if (WIN32)
    target_sources(idros PUBLIC 
        "src/core/audio/win32/win32_audio_engine.cpp"
        "src/core/window/win32/win32_window.cpp"
        "src/core/window/win32/win32_input.cpp"
        "src/core/window/win32/wgl.c"
    )
endif()

if (RENDER_BACKEND STREQUAL "OpenGL")
    target_sources(idros PUBLIC 
        "src/core/graphics/opengl/gl.c"
		"src/core/graphics/opengl/opengl.cpp"
		"src/core/graphics/opengl/glvertexarray.cpp"
		"src/core/graphics/opengl/glfont.cpp"
		"src/core/graphics/opengl/gltext.cpp"
		"src/core/graphics/opengl/glshader.cpp"
		"src/core/graphics/opengl/gltexture.cpp"
		"src/core/graphics/opengl/glblendmode.cpp"
		"src/core/graphics/opengl/glframebuffer.cpp"
        "src/core/graphics/opengl/glview2d.cpp"
	    "src/core/graphics/opengl/glview3d.cpp"
        "src/core/graphics/opengl/gluniformbuffer.cpp"
    )
endif()

target_include_directories(idros PUBLIC "src" ${LIB_INCLUDE_PATH})
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
    "${APP_DIR}\\build"
)
add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE_DIR:idros>/libidros.dll.a"
    "${APP_DIR}\\ext\\libs"
)

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\core"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/core/clock.h"
    "${CMAKE_SOURCE_DIR}/src/core/typedefs.h"
    "${CMAKE_SOURCE_DIR}/src/core/math.hpp"

    "${APP_DIR}\\ext\\includes\\idros\\core"
)

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\core\\graphics"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/core/graphics/colour.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/font.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/framebuffer.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/glyph.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/image.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/renderer.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/shader.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/text.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/texture.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/blendmode.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/vertex.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/vertexarray.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/view2d.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/view3d.h"
    "${CMAKE_SOURCE_DIR}/src/core/graphics/uniformbuffer.h"

    "${APP_DIR}\\ext\\includes\\idros\\core\\graphics"
)

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\core\\window"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/core/window/window.h"
    "${CMAKE_SOURCE_DIR}/src/core/window/input.h"
    "${CMAKE_SOURCE_DIR}/src/core/window/event.h"

    "${APP_DIR}\\ext\\includes\\idros\\core\\window"
)

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\core\\audio"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/core/audio/audio_engine.h"
    "${CMAKE_SOURCE_DIR}/src/core/audio/sound.h"
    "${CMAKE_SOURCE_DIR}/src/core/audio/music.h"
    "${CMAKE_SOURCE_DIR}/src/core/audio/audioinfo.h"
    "${CMAKE_SOURCE_DIR}/src/core/audio/format_headers.h"

    "${APP_DIR}\\ext\\includes\\idros\\core\\audio"
)

if (WIN32)
    add_custom_command(
        TARGET idros
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\core\\audio\\win32"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/core/audio/win32/win32_audio_engine.h"

        "${APP_DIR}\\ext\\includes\\idros\\core\\audio\\win32"
    )
    add_custom_command(
        TARGET idros
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\core\\window\\win32"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/core/window/win32/win32_input.h"
        "${CMAKE_SOURCE_DIR}/src/core/window/win32/win32_window.h"
        "${CMAKE_SOURCE_DIR}/src/core/window/win32/wgl.h"

        "${APP_DIR}\\ext\\includes\\idros\\core\\window\\win32"
    )
endif()

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/entry.h"
    "${CMAKE_SOURCE_DIR}/src/idros.h"
    "${CMAKE_SOURCE_DIR}/src/igame.h"
    "${CMAKE_SOURCE_DIR}/src/enginecontext.h"
    "${CMAKE_SOURCE_DIR}/src/debug.h"
    "${CMAKE_SOURCE_DIR}/src/time.hpp"
    "${CMAKE_SOURCE_DIR}/src/random.hpp"
    "${CMAKE_SOURCE_DIR}/src/sprite_animation.h"
    "${CMAKE_SOURCE_DIR}/src/scene.h"
    "${CMAKE_SOURCE_DIR}/src/scene_manager.h"
    "${CMAKE_SOURCE_DIR}/src/scene_manager.inl"
    "${CMAKE_SOURCE_DIR}/src/resource_manager.h"
    "${CMAKE_SOURCE_DIR}/src/serialization.h"

    "${APP_DIR}\\ext\\includes\\idros"
)
add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\ecs"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/ecs/ecs.h"
    "${CMAKE_SOURCE_DIR}/src/ecs/ecs.inl"
    "${CMAKE_SOURCE_DIR}/src/ecs/entity.h"
    "${CMAKE_SOURCE_DIR}/src/ecs/entity_manager.h"
    "${CMAKE_SOURCE_DIR}/src/ecs/component_manager.h"
    "${CMAKE_SOURCE_DIR}/src/ecs/component_manager.inl"
    "${CMAKE_SOURCE_DIR}/src/ecs/component_set.h"
    "${CMAKE_SOURCE_DIR}/src/ecs/component_set.inl"
    "${CMAKE_SOURCE_DIR}/src/ecs/components.h"

    "${APP_DIR}\\ext\\includes\\idros\\ecs"
)

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\ecs\\systems"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/ecs/systems/sprite_animation_system.h"
    "${CMAKE_SOURCE_DIR}/src/ecs/systems/pathfindingsystem.h"

    "${APP_DIR}\\ext\\includes\\idros\\ecs\\systems"
)

add_custom_command(
    TARGET idros
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_DIR}\\ext\\includes\\idros\\utils"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/src/utils/sprite_utils.h"
    "${CMAKE_SOURCE_DIR}/src/utils/physicsutils.h"
    "${CMAKE_SOURCE_DIR}/src/utils/mouseutils.h"
    "${CMAKE_SOURCE_DIR}/src/utils/textutils.h"
    "${CMAKE_SOURCE_DIR}/src/utils/uiutils.h"
    "${CMAKE_SOURCE_DIR}/src/utils/tagutils.h"

    "${APP_DIR}\\ext\\includes\\idros\\utils"
)