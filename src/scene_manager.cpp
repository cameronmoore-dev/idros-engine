#include "scene_manager.h"

namespace idrs
{
    SceneManager &SceneManager::get()
    {
        static SceneManager instance;
        return instance;
    }

    void SceneManager::loadScene(const std::string &name)
    {
        SceneManager &instance = get();

        if (instance.m_scene)
        {
            instance.m_scene->shutdown();
        }

        u64 key = instance.hash(name);
        auto it = instance.m_sceneTable.find(key);
        if (it != instance.m_sceneTable.end())
        {
            instance.m_scene = it->second;
            instance.m_scene->start();
        }
        else
        {
            printf("No scene could be found with name: %s\n", name.c_str());
        } 
    }

    Scene &SceneManager::currentScene()
    {
        return *get().m_scene;
    }

    u64 SceneManager::hash(const std::string &name)
    {
        return std::hash<std::string>{}(name);   
    }
}