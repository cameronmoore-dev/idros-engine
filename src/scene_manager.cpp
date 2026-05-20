#include "scene_manager.h"

namespace idrs
{
    SceneManager &SceneManager::get()
    {
        static SceneManager instance;
        return instance;
    }

    Scene &SceneManager::currentScene()
    {
        return *get().m_scene;
    }
}