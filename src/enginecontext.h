#pragma once

#include "core/window/window.h"
#include "core/graphics/renderer.h"
#include "core/audio/audio_engine.h"
#include "ecs/ecs.h"
#include "debug.h"
#include "random.hpp"
#include "time.hpp"
#include "resource_manager.h"
#include "scene_manager.h"

namespace idrs
{
    struct EngineContext
    {
        Window window = Window("Idros Engine", 1280, 720, Style::Default);
        Renderer renderer = Renderer();
        AudioEngine audio = AudioEngine();
        ECS ecs = ECS();
        Debug debug = Debug();
        Random random = Random();
        Time time = Time();
        ResourceManager resourceManager = ResourceManager();
        SceneManager sceneManager = SceneManager();
    };

    extern EngineContext *g_engine;
}