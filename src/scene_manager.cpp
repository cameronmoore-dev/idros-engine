#include "scene_manager.h"

namespace idrs
{
    SceneManager &SceneManager::get()
    {
        static SceneManager instance;
        return instance;
    }

    void SceneManager::removeScene()
    {
        SceneManager &instance = get();
        if (instance.m_scenes.size() == 1)
        {
            return;
        }

        instance.m_scenes.top()->shutdown();
        instance.m_scenes.pop();
    }

    const std::unique_ptr<Scene> &SceneManager::getActiveScene()
    {
        return get().m_scenes.top();
    }
}