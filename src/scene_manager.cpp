#include "scene_manager.h"

namespace idrs
{
    SceneManager::SceneManager() :
        m_scene(nullptr)
    {
    }

    void SceneManager::loadScene(const std::string &name)
    {
        if (m_scene)
        {
            m_scene->shutdown();
        }

        u64 key = hash(name);
        auto it = m_sceneTable.find(key);
        if (it != m_sceneTable.end())
        {
            m_scene = it->second;
            m_scene->start();
        }
        else
        {
            printf("No scene could be found with name: %s\n", name.c_str());
        } 
    }

    Scene &SceneManager::currentScene()
    {
        return *m_scene;
    }

    u64 SceneManager::hash(const std::string &name)
    {
        return std::hash<std::string>{}(name);   
    }
}