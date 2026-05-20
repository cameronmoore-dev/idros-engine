template<typename T>
inline void SceneManager::loadScene(Window &window)
{
    SceneManager &instance = get();
    if (instance.m_scene)
    {
        instance.m_scene->shutdown();
        delete(instance.m_scene);
    }

    instance.m_scene = new T(window);
    instance.m_scene->start();
}